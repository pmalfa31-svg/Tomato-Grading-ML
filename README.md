Real-Time Optical Sorting ML Pipeline
Automated Tomato Grading & Classification System for High-Speed Conveyor Lines

Executive Summary
Questo repository implementa una pipeline di ingegneria dei dati, addestramento di Machine Learning ed esportazione firmware per un sistema di smistamento ottico industriale a spettro multiplo (Line-Scan RGB/NIR/HSI).

Il software elabora in tempo reale i profili spettrali di frutti (pomodori standard e cherry) in transito ad alta frequenza su nastri trasportatori a velocità superiori a 1 m/s. L'obiettivo dell'algoritmo è classificare lo stadio di maturazione e il calibro fisico del prodotto, inviando un comando pneumatico di espulsione in Hard Real-Time con una latenza di inferenza inferiore al microsecondo (< 1µs).

System Architecture & Workflow
L'architettura software è strutturata in tre stadi modulari e disaccoppiati:

Plaintext
[ Line-Scan Optical Sensor ]
             |
             v
  (Raw CSV / Encoder Steps)
             |
             v
[ 1. Data Processing ] ──> src/data_loader.py
             |
             v
   (Structured Feature Table)
             |
             v
[ 2. ML Training ] ──> src/train_model.py
             |
             v
    (Random Forest Model)
             |
             v
[ 3. Embedded C++ Firmware ] ──> src/export_embedded.py
             |
             v
[ Header: tomato_classifier.h ] ──> Esecuzione su STM32 / PLC
Repository Structure
Plaintext
TOMATO-GRADING-ML/
│
├── .gitignore
├── requirements.txt
├── README.md
│
├── data/
│   ├── raw/
│   ├── processed/
│   ├── schema/
│   └── photos/
│
├── docs/
│   └── sorting_classes_taxonomy.md
│
├── include/
│   └── tomato_classifier.h
│
└── src/
    ├── data_loader.py
    ├── train_model.py
    └── export_embedded.py
Data Processing & Streaming Math
I dati grezzi provengono da sensori che scansionano il passaggio dei frutti a fette trasversali lungo l'asse longitudinale. Il modulo src/data_loader.py implementa le seguenti logiche:

Hardware Gatekeeper Filtering: Eliminazione istantanea del rumore del nastro trasportatore tramite verifica del registro di validità ottica.

Fruit Sessionization: Intercettazione del fronte di salita del contatore di scansione per raggruppare dinamicamente le letture lineari in un unico vettore di feature per ogni frutto fisico.

Spettrometria in Streaming (O(1)): Calcolo al volo delle medie e degli indici chimici combinati.

Machine Learning & Validation Protocol
Per soddisfare i vincoli operativi di una linea ad alta velocità, la classificazione si affida a un modello alberato leggero ma robusto.

1. Model Selection & Hardware Constraints
Il motore di inferenza è un Random Forest Classifier (n_estimators = 35, max_depth = 6). La profondità dell'albero è stata bloccata a 6 per garantire che il microcontrollore esegua al massimo 6 salti condizionali durante la valutazione, mantenendo la latenza entro i limiti temporali di espulsione pneumatica.

2. Prevention of Data Leakage (GroupKFold)
Il sistema implementa una validazione GroupKFold a 5 split basata sull'identificativo univoco del frutto (tomato_id), misurando le performance del modello esclusivamente su frutti mai visti prima durante il training.

3. Operational Performance (Out-of-Fold)
Le valutazioni rigorose su dati intatti confermano un'affidabilità di grado industriale:

Accuratezza Granulare sulle 8 Classi: ~75%.

Accuratezza Operativa Binaria (Scarto vs. Prodotto Conforme): 97.25%.

Embedded Firmware Export (C++)
Per eliminare la necessità di un computer di bordo, il modulo src/export_embedded.py converte l'intera foresta di alberi decisionali in codice sorgente C nativo.

Il file generato, salvato in include/tomato_classifier.h, presenta specifiche industriali rigorose:

Zero Dependencies: Assenza totale di librerie esterne.

Microsecond Latency: Trasformazione matematica in istruzioni condizionali statiche pre-compilabili in memoria Flash.

Quick Start & Execution Pipeline
1. Environment Preparation
Bash
python3 -m venv venv
source venv/bin/activate
pip install -r requirements.txt
2. Pipeline Execution
Bash
python3 src/data_loader.py
python3 src/train_model.py
python3 src/export_embedded.py
Industrial AI Architecture — Engineered for High-Speed Conveyor Automation.