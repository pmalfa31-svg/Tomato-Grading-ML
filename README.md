# Tomato Grading ML

Machine learning model for real-time classification of tomatoes (ripe / unripe / stone / soil / debris) from optical sensor data, designed for integration into an industrial sorting line.

## Overview

Industrial sorting lines for fruit and vegetables use optical sensors to detect specific chromatic wavelengths, distinguishing conforming product from non-conforming product (foreign bodies, unripe produce, debris) and driving a mechanical rejection system on a conveyor belt running at speeds up to 1 m/s.

This project evaluates whether a neural network, trained on real sensor data, can improve classification accuracy over the currently deployed algorithm while meeting the strict latency requirements of a real-time sorting system.

## Approach

Raw sensor readings are aggregated into a fixed-size statistical feature vector per item, then classified using a lightweight Multi-Layer Perceptron (MLP). This keeps inference time in the microsecond-to-millisecond range, which is required given the conveyor speed and mechanical rejection window.

Model evaluation focuses on standard classification metrics (accuracy, precision, recall, F1, confusion matrix), with particular attention to minimizing false negatives on non-conforming product.

For production deployment, the model is trained in Python and exported (ONNX) for inference through a lightweight C/C++ runtime, keeping the training/experimentation stack decoupled from the real-time control loop. See [`docs/architecture.md`](docs/architecture.md) for details.

## Project status

| Stage | Status |
|---|---|
| Data ingestion & feature engineering | Done |
| Model training (MLP) | In progress |
| Evaluation & benchmark vs. current algorithm | Planned |
| Real-time deployment notes | Planned |

## Repository structure

```
.
├── src/
│   └── data_preparation.py      # Data ingestion and feature aggregation
├── docs/
│   └── architecture.md          # Training vs. real-time deployment architecture
├── data/                        # Input CSVs (gitignored)
└── output/
    └── dataset.csv              # Processed dataset ready for training
```

## Getting started

```bash
pip install -r requirements.txt
python src/data_preparation.py
```

By default the script runs against synthetic data for testing; point it at `data/` to use real sensor exports.

## Requirements

- Python 3.9+
- numpy, pandas
- pytorch (model training, coming soon)

## Background

Developed as a personal project applying machine learning coursework to a real industrial use case, in collaboration with an electronics company specializing in optical sorting systems.

## License

TBD
