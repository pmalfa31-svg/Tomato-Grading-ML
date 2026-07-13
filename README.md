# Real-Time Optical Sorting ML Pipeline
**Automated Tomato Grading & Classification System for High-Speed Conveyor Lines**

---

## Executive Summary
Questo repository contiene l'intera pipeline di ingegneria dei dati, addestramento Machine Learning ed esportazione firmware per un sistema di smistamento ottico industriale a spettro multiplo (*Line-Scan RGB/NIR*). 

Il sistema è progettato per analizzare flussi di dati ad alta frequenza generati dal passaggio di prodotti agricoli (pomodori standard e cherry) su nastri trasportatori operanti a velocità $> 1\text{ m/s}$. L'obiettivo del software è elaborare i profili di riflettanza ottica, determinare lo stadio di maturazione e il calibro fisico del frutto, e inviare un comando di espulsione pneumatica in **Hard Real-Time** con una latenza di inferenza inferiore al microsecondo ($< 1\,\mu\text{s}$).

---

## System Architecture & Workflow

L'architettura del software è divisa in tre stadi disaccoppiati, che accompagnano il dato dall'acquisizione elettronica grezza fino alla micro-istruzione compilata sul microcontrollore o PLC di linea:

```text
[ Line-Scan Optical Sensor ]
             │
             ▼
  (Raw CSV / Encoder Steps)
             │
             ▼
[ 1. Data Processing & O(1) Streaming Math ] ──> src/data_loader.py
             │
             ▼
   (Structured Feature Table)
             │
             ▼
[ 2. ML Training & Out-of-Fold Validation  ] ──> src/train_model.py
             │
             ▼
    (Random Forest Model)
             │
             ▼
[ 3. Embedded C++ Firmware Translation     ] ──> src/export_embedded.py
             │
             ▼
[ Pure C/C++ Header: tomato_classifier.h   ] ──> Esecuzione su STM32 / PLC

---

## Repository Structure

TOMATO-GRADING-ML/
│
├── .gitignore                              # Esclusione di ambienti virtuali e file di sistema
├── requirements.txt                        # Dipendenze e librerie Python di progetto
├── README.md                               # Documentazione principale di architettura
│
├── data/
│   ├── raw/                                # Dataset CSV grezzi (Campagne di acquisizione sul campo)
│   ├── processed/                          # Dataset tabellare pulito (tomatoes_features.csv)
│   ├── schema/                             # Data Dictionary (optical_sensor_data_dictionary.xlsx)
│   └── photos/                             # Riferimenti visivi e campioni per classificazione
│
├── docs/
│   └── sorting_classes_taxonomy.md         # Tassonomia delle 8 classi e logica di scarto pneumatico
│
├── include/
│   └── tomato_classifier.h                 # Firmware C++ generato automaticamente (Zero-Dependency)
│
└── src/
    ├── data_loader.py                      # Pipeline di pulizia, filtrazione hardware e sessionization
    ├── train_model.py                      # Addestramento Random Forest e validazione GroupKFold
    └── export_embedded.py                  # Traduttore da modello in memoria a codice embedded C

---

```markdown
## Data Processing & Streaming Math

I dati grezzi provengono da sensori che scansionano il passaggio dei frutti a fette trasversali lungo l'asse longitudinale. Il modulo `src/data_loader.py` implementa le seguenti logiche di elaborazione ottimizzate per non saturare i buffer di memoria:

1. **Hardware Gatekeeper Filtering:** Eliminazione istantanea del rumore del nastro trasportatore tramite verifica del registro di validità ottica (`validity_flag == 0` $\rightarrow$ Valido).
2. **Fruit Sessionization:** Intercettazione del fronte di salita del contatore di scansione (`frame_id == 1`) per raggruppare dinamicamente le letture lineari in un unico vettore di feature per ogni frutto fisico.
3. **Spettrometria in Streaming ($O(1)$):** Calcolo al volo delle medie e degli indici chimici combinati:
   * **Indice di Invaiatura (Licopene vs. Clorofilla):** $$R_{\text{green}} = \frac{\text{Red}}{\text{Green} + \epsilon}$$
   * **Compensazione Geometrica Infrarossa (Ombra e Curvatura):** $$R_{\text{ir}} = \frac{\text{IR1}}{\text{IR2} + \epsilon}$$
   * **Stima del Calibro Meccanico:** Misurazione del differenziale degli impulsi di encoder lungo il transito (`transit_len`).

---

## Machine Learning & Validation Protocol

Per soddisfare i vincoli operativi di una linea ad alta velocità, la classificazione si affida a un modello alberato leggero ma estremamente robusto alle variazioni naturali del prodotto.

### 1. Model Selection & Hardware Constraints
Il motore di inferenza è un **Random Forest Classifier** (`n_estimators = 35`, `max_depth = 6`). La profondità dell'albero è stata bloccata a **6** per garantire che il microcontrollore esegua al massimo 6 salti condizionali `if / else` durante la valutazione, mantenendo la latenza entro i limiti temporali di espulsione pneumatica.

### 2. Prevention of Data Leakage (GroupKFold)
Poiché ogni pomodoro campionata produce frame sequenziali multipli nel tempo, una normale cross-validation causerebbe un grave overfitting. Il sistema implementa una validazione **GroupKFold a 5 split** basata sull'identificativo univoco del frutto (`tomato_id`), misurando le performance del modello esclusivamente su frutti mai visti prima durante il training.

### 3. Operational Performance (Out-of-Fold)
Le valutazioni rigorose su dati intatti confermano un'affidabilità di grado industriale:
* **Accuratezza Granulare sulle 8 Classi:** **~75%**. Gli scostamenti si verificano unicamente lungo i confini contigui del gradiente di invaiatura, dove la separazione visiva è sfumata anche per un operatore umano.
* **Accuratezza Operativa Binaria (Scarto vs. Prodotto Conforme):** **97.25%**. Sulla decisione elettromeccanica reale (azionare o meno l'elettrovalvola per espellere il frutto), la macchina raggiunge una precisione quasi assoluta, azzerando i falsi scarti sul prodotto idoneo alla vendita.

### 4. Top Discriminant Features
L'analisi di importanza delle feature conferma che il modello ha appreso correttamente i fenomeni biochimici e fisici:
1. `Red_Green_ratio` (**18.16%**): Dominatore della separazione chimica clorofilla-licopene.
2. `HUE_mean` (**14.27%**): Angolo cromatico medio nello spazio HSI.
3. `Green_mean` (**9.04%**): Parametro di isolamento per gli stadi acerbi.
4. `is_cherry` (**8.02%**): Classificatore dimensionale per l'adattamento dinamico delle soglie colore.

---

## Embedded Firmware Export (C++)

Per eliminare la necessità di un computer di bordo sulla macchina selezionatrice, il modulo `src/export_embedded.py` converte l'intera foresta di alberi decisionali in codice sorgente C nativo, avvalendosi della libreria `m2cgen`.

Il file generato, salvato in **`include/tomato_classifier.h`**, presenta specifiche industriali rigorose:
* **Zero Dependencies:** Assenza totale di librerie esterne, chiamate al sistema operativo o allocazioni dinamiche di memoria (`malloc / free`).
* **Microsecond Latency:** Trasformazione matematica in istruzioni condizionali statiche pre-compilabili in memoria Flash (STM32, ESP32, PLC).
* **C++ Inline Wrapper:** Funzione nativa `predict_tomato_class(const double* input)` immediatamente integrabile nel firmware principale di controllo degli attuatori.

---

## Quick Start & Execution Pipeline

### 1. Environment Preparation
```bash
# Creazione e attivazione ambiente virtuale
python3 -m venv venv
source venv/bin/activate

# Installazione delle librerie necessarie
pip install --upgrade pip
pip install -r requirements.txt

### 2. Pipeline Execution
Lanciare i moduli in ordine progressivo dalla directory principale del progetto per completare l'intero ciclo MLOps:
```bash
# 1. Caricamento, pulizia grezzi, calcolo feature O(1) e salvataggio in data/processed/
python3 src/data_loader.py

# 2. Addestramento del modello e validazione rigorosa Out-of-Fold al 97.25%
python3 src/train_model.py

# 3. Conversione del modello in codice embedded C++ nativo in include/tomato_classifier.h
python3 src/export_embedded.py