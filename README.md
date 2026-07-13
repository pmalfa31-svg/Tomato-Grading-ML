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