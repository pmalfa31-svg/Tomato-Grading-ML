# 🍅 Tassonomia delle Classi e Ricette di Smistamento (Sorting Recipes)

Questo documento definisce la mappatura ufficiale tra i codici hardware di campo (prefissi CSV), le etichette predittive del modello di Machine Learning (`Target Label`) e le proprietà fisiche dei frutti (calibro, stadio di maturazione e firma spettrale).

## 📊 Matrice di Classificazione Ufficiale

| Target Label (ML) | Prefisso File | Tipo / Calibro | Nome Commerciale | Stadio di Maturazione | Angolo Cromatico (HUE medio) | Azione Pneumatica (Eject Trigger) |
| :---: | :---: | :--- | :--- | :--- | :---: | :--- |
| **`0`** | `01_` | Standard (> 50mm) | **Pomodoro Verde Chiaro** | Acerbo / Pre-invaiatura | `~1300 - 2200` | Scarto (Linea acerbi) |
| **`1`** | `02_` | Standard (> 50mm) | **Pomodoro Giallo-Verde** | Inizio Invaiatura | `~1000 - 1200` | Scarto / Seconda scelta |
| **`2`** | `03_` | Standard (> 50mm) | **Pomodoro Arancio-Giallo**| Invaiatura Avanzata | `~450 - 600` | Mantenere (Matura a scaffale) |
| **`3`** | `04_` | Standard (> 50mm) | **Pomodoro Rosso-Arancio** | Pre-maturazione | `~100 - 200` | Mantenere (Prodotto Premium) |
| **`4`** | `05_` | Standard (> 50mm) | **Pomodoro Rosso** | Maturazione Completa | `< 80` | Mantenere (Pronto consumo) |
| **`5`** | `13_` | Cherry (< 35mm) | **Pomodorino Giallo** | Maturazione Gialla | `~600 - 650` | Mantenere (Linea Cherry Gialli) |
| **`6`** | `15_` | Cherry (< 35mm) | **Pomodorino Rosso** | Maturazione Rossa | `< 130` | Mantenere (Linea Cherry Rossi) |
| **`7`** | `16_` | Cherry (< 35mm) | **Pomodorino Verde Scuro Sfumato** | *Case Study (Hard)* | `~600` (⚠️ Simile a Giallo) | Selezione Speciale / Test |

---

## 🔬 Regola di Identificazione del Calibro (Standard vs. Cherry)

Il sistema non ha bisogno di una bilancia per capire la grandezza del pomodoro. Utilizza la colonna `transit_len` (lunghezza di transito in step di encoder) calcolata in streaming:
* **Pomodori Standard (Prefisso `0x_`):** Generano tipicamente tra **4 e 12 fette** (`total_frames`) e occupano **> 18 step encoder** di transito.
* **Pomodorini Cherry (Prefisso `1x_`):** Generano tipicamente **2 o 3 fette** e occupano **< 12 step encoder** di transito.