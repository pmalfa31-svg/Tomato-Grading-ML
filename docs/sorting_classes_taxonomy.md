# 🍅 Tassonomia delle Classi e Ricette di Smistamento (Sorting Recipes)

Questo documento definisce la mappatura ufficiale tra i codici hardware di campo (prefissi CSV), le etichette predittive del modello di Machine Learning (`Target Label`) e le proprietà fisiche dei frutti (calibro, stadio di maturazione e firma spettrale).

> **Nota di validazione:** i range in questo documento sono stati verificati contro i 291 frutti reali in `data/processed/tomatoes_features.csv` (campagne del 2026-07-09/10), non solo dichiarati. Dove il dato osservato si discosta da una regola semplice, è segnalato esplicitamente invece di essere arrotondato per far tornare i conti.

## 📊 Matrice di Classificazione Ufficiale

| Target Label (ML) | Prefisso File | Tipo / Calibro | Nome Commerciale | Stadio di Maturazione | HUE — range tipico (10°-90° percentile) | Azione Pneumatica (Eject Trigger) |
| :---: | :---: | :--- | :--- | :--- | :---: | :--- |
| **`0`** | `01_` | Standard (> 50mm) | **Pomodoro Verde Chiaro** | Acerbo / Pre-invaiatura | `~1420 - 2410` | Scarto (Linea acerbi) |
| **`1`** | `02_` | Standard (> 50mm) | **Pomodoro Giallo-Verde** | Inizio Invaiatura | `~800 - 1420` | Scarto / Seconda scelta |
| **`2`** | `03_` | Standard (> 50mm) | **Pomodoro Arancio-Giallo**| Invaiatura Avanzata | `~130 - 860` ⚠️ | Mantenere (Matura a scaffale) |
| **`3`** | `04_` | Standard (> 50mm) | **Pomodoro Rosso-Arancio** | Pre-maturazione | `~30 - 215` ⚠️ | Mantenere (Prodotto Premium) |
| **`4`** | `05_` | Standard (> 50mm) | **Pomodoro Rosso** | Maturazione Completa | `~25 - 115` ⚠️ | Mantenere (Pronto consumo) |
| **`5`** | `13_` | Cherry (< 35mm) | **Pomodorino Giallo** | Maturazione Gialla | `~560 - 685` | Mantenere (Linea Cherry Gialli) |
| **`6`** | `15_` | Cherry (< 35mm) | **Pomodorino Rosso** | Maturazione Rossa | `~95 - 180` | Mantenere (Linea Cherry Rossi) |
| **`7`** | `16_` | Cherry (< 35mm) | **Pomodorino Verde Scuro Sfumato** | *Case Study (Hard)* | `~480 - 770` ⚠️ (sovrapposto a classe 5) | Selezione Speciale / Test |

⚠️ = classe con sovrapposizione osservata rispetto a una classe adiacente (vedi "Limiti Noti" sotto). Il range HUE da solo non è sufficiente a distinguerle in modo affidabile: la classificazione finale si basa sull'intero vettore di feature (canali spettrali completi), non sul solo HUE.

---

## 🔬 Regola di Identificazione del Calibro (Standard vs. Cherry)

Il sistema non ha bisogno di una bilancia per capire la grandezza del pomodoro. Usa in combinazione due segnali calcolati in streaming — `transit_len` (step di encoder) e `valid_slices` (numero di fette rilevate) — con affidabilità diversa:

* **Pomodorini Cherry (Prefisso `1x_`):** occupano **≤ 12 step encoder** di transito. Regola verificata sul 100% dei 23 campioni cherry disponibili (0 eccezioni) — è il segnale più affidabile per identificare un cherry. Generano tipicamente **3-5 fette** (`valid_slices`), non le 2-3 di una stima precedente — il minimo osservato è 2, ma il grosso dei casi è 3-5.

* **Pomodori Standard (Prefisso `0x_`):** occupano tipicamente **18-33 step encoder**, ma **non è una regola rigida**: il 23.5% dei pomodori standard nel dataset (63 su 268) ha `transit_len` inferiore a 18, e il 14.6% (39 su 268) rientra interamente nel range tipico dei cherry (≤ 12 step). Generano tipicamente **2-11 fette**, con una piccola quota (7%) sotto le 4 fette.

> ✅ **Aggiornamento:** questo problema è stato risolto unificando la soglia standard a 12 step encoder (invece dei 18 originari), lo stesso valore usato per i cherry. Poiché nessun cherry nel dataset supera i 12 step, questa soglia unica cattura ancora il 100% dei cherry, riducendo però i falsi positivi sui pomodori standard dal 23.5% al 14.6% (39/268). Non è ancora zero: c'è una sovrapposizione fisica reale tra i pomodori standard più piccoli/corti e i cherry più grandi che una soglia singola su `transit_len` non può eliminare del tutto — combinare `transit_len` con `valid_slices` è stato testato e **peggiora** il risultato (fa perdere il riconoscimento di molti cherry veri), quindi non è stato adottato. Vedi `tomato_check_batch_anomaly()` in `include/tomato_classifier.h` e `check_batch_consistency()` in `src/train_model.py` per l'implementazione.

---

## Limiti Noti di Questa Tassonomia

- **HUE non è un discriminante da solo per le classi 2, 3, 4**: i range tipici si sovrappongono (es. classe 3 arriva fino a ~215, classe 4 fino a ~115 — la separazione netta esiste solo guardando l'intera firma spettrale, non il solo HUE).
- **Sovrapposizione nota tra classe 5 e classe 7** (entrambe cherry, HUE tipico nella fascia 480-770): la classe 7 è esplicitamente segnalata come *case study difficile* fin dalla prima stesura di questo documento, e i dati confermano che l'avviso era fondato.
- **`transit_len` come regola di calibro**: affidabile al 100% per escludere i cherry sopra i 12 step, non affidabile come soglia unica per confermare "è standard" sotto i 18 step (vedi sopra).
- Questi numeri sono calcolati su **291 frutti raccolti in 2 sole giornate di campagna**: vanno considerati indicativi, da confermare/aggiornare quando saranno disponibili più dati di campo.
