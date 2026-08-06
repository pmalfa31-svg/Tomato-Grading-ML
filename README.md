# Real-Time Optical Sorting ML Pipeline
**Automated Tomato Grading & Classification System for High-Speed Conveyor Lines**

![Tests](https://github.com/pmalfa31-svg/Tomato-Grading-ML/actions/workflows/tests.yml/badge.svg)

---

## Executive Summary

This repository implements a data engineering, Machine Learning training, and firmware export pipeline for an industrial multi-spectral optical sorting system (Line-Scan RGB/NIR/HSI).

The software processes in real time the spectral profiles of fruit (standard and cherry tomatoes) as they transit at high frequency on conveyor belts, classifying ripeness stage and product size in order to send a pneumatic ejection command.

The system is designed around a real operational constraint of the line: **every processed batch is homogeneous** — either all standard tomatoes, or all cherry tomatoes — and the batch type is **known in advance**, set by the operator or by the upstream system before a single fruit passes under the sensor. This constraint drives the entire architecture: two dedicated classification models, selected via an explicit configuration parameter, with a physical consistency check to guard against setup errors.

---

## System Architecture & Workflow

```text
[ Line-Scan Optical Sensor ]
             |
             v
  (Raw CSV / Encoder Steps)
             |
             v
[ 1. Data Processing & O(1) Streaming Math ] ──> src/data_loader.py
             |
             v
   (Structured Feature Table, incl. is_cherry per lotto)
             |
             v
[ 2. Training PER LOTTO (batch_type esplicito)  ] ──> src/train_model.py
             |             |
             v             v
   (Modello STANDARD)  (Modello CHERRY)
       5 classi            3 classi
             |             |
             v             v
[ 3. Embedded C++ Firmware Translation (2 modelli in 1 header) ] ──> src/export_embedded.py
             |
             v
[ include/tomato_classifier.h ]
   - enum TomatoBatchMode { BATCH_STANDARD, BATCH_CHERRY }
   - tomato_check_batch_anomaly(mode, transit_len)
   - predict_tomato_class(mode, input)  ──> Execution on STM32 / PLC / ESP32
```

`TomatoBatchMode` is a line configuration, set once per shift/batch by whoever integrates the firmware — exactly as one would set a "program" on industrial machinery — not a feature computed by the sensor fruit by fruit.

---

## Repository Structure

```text
TOMATO-GRADING-ML/
│
├── .github/
│   └── workflows/
│       └── tests.yml                       # CI: runs tests/ on every push/PR
│
├── .gitignore
├── LICENSE
├── requirements.txt
├── requirements-dev.txt                    # Adds pytest, for development/testing only
├── README.md                               # This document
│
├── data/
│   ├── raw/                                # Raw CSV datasets (field acquisition campaigns)
│   ├── processed/                          # Clean tabular dataset (tomatoes_features.csv)
│   ├── schema/                             # Data Dictionary (optical_sensor_data_dictionary.xlsx)
│   └── photos/                             # Visual references and samples for classification
│
├── docs/
│   ├── sorting_classes_taxonomy.md         # Taxonomy of the 8 classes and pneumatic ejection logic
│   └── images/                             # Charts (confusion matrix, feature importance) used in the README
│
├── firmware/
│   └── tomato_esp32_test/                  # Benchmark sketch on ESP32 (real hardware latency)
│       ├── tomato_esp32_test.cpp
│       ├── tomato_core.c
│       └── tomato_classifier.h
│
├── include/
│   └── tomato_classifier.h                 # Generated C/C++ firmware (2 models, explicit batch mode)
│
├── models/
│   ├── model_standard_metadata.json        # Model metadata (training date, n. samples) -- the .pkl is gitignored
│   └── model_cherry_metadata.json
│
├── src/
│   ├── data_loader.py                      # Cleaning, hardware filtering, and sessionization
│   ├── train_model.py                      # Per-batch training, GroupKFold validation, model persistence
│   └── export_embedded.py                  # Model-to-embedded-C-code translator
│
└── tests/
    ├── conftest.py
    ├── test_data_loader.py
    ├── test_train_model.py
    └── test_export_embedded.py             # Includes a test that compiles the C header with gcc
```

---

## Data Processing & Streaming Math

The raw data comes from sensors that scan fruit passage in cross-sectional slices along the longitudinal axis. The `src/data_loader.py` module implements:

1. **Hardware Gatekeeper Filtering** — removes belt noise via the optical validity register (`validity_flag == 0` -> valid).
2. **Fruit Sessionization** — intercepts the rising edge of the scan counter (`frame_id == 1`) to group linear readings into a single feature vector for each physical fruit.
3. **Streaming Spectrometry (O(1))** — on-the-fly computation of means, standard deviations, and combined chemical indices (ratios between channels).

The output is `data/processed/tomatoes_features.csv`, with one row per fruit and an `is_cherry` column identifying the batch of origin — used to route each fruit to the correct model during training (see next section).

---

## Class Taxonomy

| ID | Class | Batch Type | Outcome |
|----|--------|:----------:|-------|
| 0 | Green Standard Tomato | STANDARD | Reject |
| 1 | Yellow-Green Tomato | STANDARD | Reject |
| 2 | Orange-Yellow Tomato | STANDARD | Pass |
| 3 | Red-Orange Tomato | STANDARD | Pass |
| 4 | Red Standard Tomato | STANDARD | Pass |
| 5 | Yellow Cherry Tomato | CHERRY | Pass |
| 6 | Red Cherry Tomato | CHERRY | Pass |
| 7 | Dark Green Mottled Cherry Tomato | CHERRY | Reject |

Full details of the taxonomy and the size identification rule (based on `transit_len`) are in `docs/sorting_classes_taxonomy.md`.

---

## Two-Model Architecture (Batch Mode)

Since in the field batches are always homogeneous and the type is known in advance, the system does not treat `is_cherry` as a measurement to be inferred fruit by fruit: it treats it as an **explicit configuration parameter** chosen upstream, which selects which of the two dedicated models to use.

```python
# src/train_model.py
BATCH_CONFIG = {
    "standard": {"is_cherry_value": 0, "classes": [0, 1, 2, 3, 4]},
    "cherry":   {"is_cherry_value": 1, "classes": [5, 6, 7]},
}
```

`train_and_evaluate_model(data_dir, batch_type="standard")` filters the dataset by batch type before training, and produces a dedicated model with 5 or 3 classes. This separation has two concrete advantages:

- **Each model solves a simpler problem** (5 or 3 classes instead of 8), instead of also having to relearn the standard/cherry distinction on every prediction.
- **The firmware interface is explicit**: whoever integrates `tomato_classifier.h` declares `BATCH_STANDARD` or `BATCH_CHERRY` just once per shift, with the exact same optical feature schema in both cases (no "special" value hidden among the sensor measurements).

### Consistency check (anomaly detection)

Blindly trusting the batch parameter is a risk: if an anomalous fruit ends up in the wrong bin (e.g., a cherry tomato among the standard ones), the system would not notice. For this reason the pipeline includes a cross-check based on `transit_len` — a real physical measurement (transit length over the sensor, correlated with fruit size):

```python
# src/train_model.py
def check_batch_consistency(batch_type, transit_len,
                             standard_min_transit_len=12,
                             cherry_max_transit_len=12):
    if batch_type == "standard":
        return transit_len > standard_min_transit_len
    elif batch_type == "cherry":
        return transit_len <= cherry_max_transit_len
```

The same thresholds (12 / 12 encoder steps, data verified in `docs/sorting_classes_taxonomy.md`) are replicated in the C firmware via `tomato_check_batch_anomaly()` (see Embedded Firmware section). The threshold is unified at 12 because no cherry tomato in the dataset exceeds that value: using the same limit on the standard side as well captures 100% of cherry tomatoes, reducing false positives on standard tomatoes from 23.5% to 14.6% compared to a threshold chosen without data verification.

> **Note:** a residual 14.6% of false positives on standard tomatoes remains, because there is a real physical overlap between the smaller/shorter standard tomatoes and the larger cherry tomatoes — not eliminable with a single threshold on `transit_len`. Combining `transit_len` with `valid_slices` was tested and discarded: it reduces false positives but causes many true cherry tomatoes to go unrecognized.

---

## Machine Learning & Validation Protocol

### Model Selection & Hardware Constraints

The inference engine is a `RandomForestClassifier` (`n_estimators=35`, `max_depth=6`) for each batch, chosen for its direct translatability into native C code with no dependencies — a key requirement for execution on resource-constrained microcontrollers. The maximum depth of 6 limits the conditional branches **per individual tree**; the actual computational cost of an inference is the sum across all 35 trees in the forest, not 6 total branches — see the latency note below.

### Cross-Validation Protocol

Validation uses 5-split `GroupKFold`, grouped by `tomato_id`. Since each physical fruit is scanned only once, `tomato_id` is unique per row: in practice this GroupKFold is equivalent to a standard KFold.

This is a deliberate choice, not a limitation to be fixed: there is no risk of classic leakage (no physical fruit appears twice in the dataset), and grouping by collection day — the more rigorous alternative in theory — **is not applicable to this dataset**: 6 out of 8 classes exist on only one of the two collection days (the three cherry classes exist only on day 2, three intermediate standard classes only on day 1). Holding out an entire day for validation would zero out the training set for precisely those classes.

The actual limitation is not the cross-validation formula, it's that the model has never been validated under different collection conditions (light, calibration, batch) for most classes — more field collection campaigns are needed, not another split.

### Operational Performance (Out-of-Fold, real data)

| Batch | Samples | Classes | Average Accuracy (CV) |
|---|---|---|---|
| **STANDARD** | 268 | 5 | 77.98% (± 4.03%) |
| **CHERRY** | 23 | 3 | 100.00% (± 0.00%) |

> ⚠️ **The CHERRY batch figure should be interpreted with caution.** With only 23 total observations (7-8 per class), the 100% figure is consistent both with a model that is genuinely very effective on a simple problem, and with a sample too small to reliably estimate accuracy. More field data for the cherry batch is needed before validating it for production.

Run `python3 src/train_model.py` to regenerate these numbers on the current dataset — the script also prints the confusion matrix and the ranking of the most discriminative features for each batch.

![Confusion Matrices](docs/images/confusion_matrices.png)

The largest source of error in the standard model is between **Red-Orange and Red Standard** (19 mix-ups out of 38 real Red-Orange cases): the model mainly errs between *adjacent* ripeness stages, not by chance — consistent with the fact that ripening is a continuous spectrum, not sharply separated categories.

![Feature Importance](docs/images/feature_importance.png)

---

## Embedded Firmware Export (C/C++)

`src/export_embedded.py` trains both models and generates a single header, `include/tomato_classifier.h`, containing:

- **Two namespaced scoring functions** (`score_standard`, `score_cherry`) — native C code generated by [m2cgen](https://github.com/BayesWitnesses/m2cgen), zero external dependencies.
- **A `TomatoBatchMode` enum** (`BATCH_STANDARD`, `BATCH_CHERRY`) — the batch configuration parameter.
- **`tomato_check_batch_anomaly(mode, transit_len)`** — the consistency check described above.
- **`predict_tomato_class(mode, input)`** — helper function that dispatches to the correct model and always returns the **global** class ID (0-7), regardless of the internal class ordering chosen by m2cgen.

### Firmware usage example

```c
TomatoBatchMode current_batch = BATCH_STANDARD;  // set ONCE per shift

// for each fruit in transit:
double input[18] = { /* transit_len, valid_slices, IR1_mean, ... */ };

if (tomato_check_batch_anomaly(current_batch, input[0])) {
    // fruit physically incompatible with the declared batch -> flag/handle separately
} else {
    int classe = predict_tomato_class(current_batch, input);
    // -> pneumatic command based on `classe`
}
```

### ⚠️ C vs C++ Compatibility — read before integrating

The `score_standard()` / `score_cherry()` functions, auto-generated by m2cgen, use **C99** syntax (compound literals, e.g. `(double[]){...}` inside `memcpy`). This syntax is **valid C but not standard C++**: a conforming C++ compiler rejects it with an error such as `taking address of temporary array`, even if the code is wrapped in an `extern "C"` block (which only governs *name mangling*, not the language grammar the compiler must accept).

**How to integrate it correctly:**

| Scenario | Action |
|---|---|
| Project entirely in **C** | No action needed: the file compiles as-is. |
| Project in **C++** (e.g., STM32CubeIDE with mixed C/C++ HAL, or Arduino/ESP32) | Isolate `tomato_classifier.h` in a **separate `.c` compilation unit**, compile it with the C compiler, and link it to the rest of the C++ project via `extern "C"` declarations. Only `predict_tomato_class()` and `tomato_check_batch_anomaly()` are meant for direct use in C++ code. |

A working example of this pattern, verified end-to-end on real ESP32 hardware, is in `firmware/tomato_esp32_test/`:

```text
firmware/tomato_esp32_test/
├── tomato_esp32_test.cpp   # C++ sketch (not .ino — see note below) — extern "C" declarations only
├── tomato_core.c           # Pure C compilation unit — #include "tomato_classifier.h"
└── tomato_classifier.h     # Generated firmware
```

> **Why `.cpp` and not `.ino`:** the Arduino/PlatformIO "sketch" preprocessor automatically generates function prototypes and inserts them at the top of the file, *before* custom types (such as `struct TestCase`) have been defined — with parameters passed by reference to a struct, this produces compilation errors (`'TestCase' was not declared in this scope`). Using `.cpp` disables this preprocessor and the file compiles in the order it is written, without surprises.

**Complete hardware demo:** in addition to the latency benchmark, the sketch drives two LEDs (red = reject, green = pass) and a small 128x64 I2C OLED that animates the tomato's passage on the belt up to the "sensor," where the actual inference kicks in. It includes a compile-time switch (`FAST_VERIFY_MODE`) to toggle between a fast verification mode (via serial, for debugging) and a slower, more readable mode designed to be filmed.

**Try the demo from your browser, no hardware needed:** [wokwi.com/projects/471166580602582017](https://wokwi.com/projects/471166580602582017) — full interactive simulation (ESP32 + OLED + LED), same code as the real firmware. Useful for exploring the project without owning a board, but the latency times shown in the simulation **are not representative** of real ones (see below) — for that, refer only to the numbers measured on physical hardware.

### ⚠️ Stack size on FreeRTOS/ESP32

The functions generated by m2cgen use hundreds of temporary arrays allocated on the stack (one for each C99 compound literal, ~1145 in the current header). The default task running `setup()`/`loop()` on Arduino-ESP32 (`loopTask`) has only **8KB** of stack — insufficient, it stack-overflows on the very first call to `score_standard()`. `tomato_esp32_test.cpp` shows the solution: run the inference in a dedicated FreeRTOS task with an explicit larger stack (`xTaskCreatePinnedToCore(..., 32768, ...)`), instead of relying on the default stack.

### Latency — measured, not stated

The `max_depth=6` constraint guarantees at most 6 conditional comparisons **per individual tree**; with `n_estimators=35`, a single inference evaluates up to 35 trees and sums their votes. Rather than inferring latency from tree depth alone, `firmware/tomato_esp32_test/` runs a real benchmark on ESP32 hardware: 16 real fruits (2 for each of the 8 classes), predictions verified against the Python model's output, average latency measured over 2000 repetitions per sample.

**Real results (ESP32, measured with `esp_timer_get_time()`):**

| Model | Average Latency | Notes |
|---|---|---|
| STANDARD (5 classes) | ~220-550 µs | Trees always at maximum depth (6) — 28.6 average leaves |
| CHERRY (3 classes) | ~43-49 µs | Much shorter trees (average depth 2.77) — training dataset too small (23 samples) for the algorithm to find anything else to split beyond 2-3 levels |

Even the worst measured case (~550 µs) is well within margins for a real-time conveyor-belt system — inference alone would use a minimal fraction of any realistic cycle budget for this application.

---

## Testing

```bash
pip install -r requirements-dev.txt
python3 -m pytest tests/ -v
```

19 tests, run automatically on every push/PR (see badge at the top). These are not generic tests: several explicitly encode real bugs found during the development of this project, so that if they are accidentally reintroduced, the test fails immediately instead of being discovered on hardware:

- `is_cherry` must never reappear among the training features (the original data leakage bug)
- the consistency threshold (12 encoder steps) must capture 100% of the real cherry tomatoes in the dataset
- the exact threshold boundary (`transit_len == 12`) must land on the right side — it's the edge that in the first version would have halved cherry tomato detection
- the generated C header must **actually compile** with `gcc`, not just "look" correct

## Known Limitations

An honest list of the known limitations of this version, useful for planning next steps:

- **Small dataset**: 291 total fruits collected over just 2 days of field campaigns; the cherry batch has only 23 samples (7-8 per class).
- **Incomplete multi-day coverage**: the data covers only 2 collection days, and 6 out of 8 classes exist on only one of the two — so the model has never been validated under different conditions (light, calibration, batch, seasonality) for most of the classes. Not solvable by reorganizing cross-validation, more field campaigns are needed (see ML section above).
- **Residual false positives in the consistency check**: the unified 12-encoder-step threshold captures 100% of cherry tomatoes but still generates 14.6% false positives on standard tomatoes, due to real physical overlap in the data — not solvable with `transit_len` alone (see `docs/sorting_classes_taxonomy.md`).
- **ESP32 benchmark in "replay" mode**: the measured latency covers only classification inference, not the entire acquisition pipeline (the physical optical sensor is not yet connected).

---

## Quick Start & Execution Pipeline

### 1. Environment Preparation

```bash
python3 -m venv venv
source venv/bin/activate

pip install --upgrade pip
pip install -r requirements.txt
```

### 2. Pipeline Execution

```bash
# 1. Load, clean raw data, and compute O(1) features -> data/processed/
python3 src/data_loader.py

# 2. Train and Out-of-Fold validation FOR BOTH BATCHES (standard + cherry)
python3 src/train_model.py

# 3. Convert both models into a single C/C++ header -> include/tomato_classifier.h
python3 src/export_embedded.py
```

To train/evaluate a single batch in a custom Python script:

```python
from src.train_model import train_and_evaluate_model

model, feature_cols, df = train_and_evaluate_model("data/raw", batch_type="standard")
# or batch_type="cherry"
```

### 3. Real Hardware Benchmark (ESP32)

```bash
# PlatformIO project: place the 3 files from firmware/tomato_esp32_test/ into src/ and include/
# required lib_deps in platformio.ini:
#   adafruit/Adafruit SSD1306@^2.5.7
#   adafruit/Adafruit GFX Library@^1.11.5
pio run --target upload
pio device monitor   # 115200 baud

# Alternatively, try the demo from your browser without hardware:
# https://wokwi.com/projects/471166580602582017
```

---

*Industrial AI Architecture — Engineered for High-Speed Conveyor Automation.*

