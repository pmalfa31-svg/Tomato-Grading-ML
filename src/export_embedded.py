import os
import re
import m2cgen as m2c

# Importiamo dal modulo di training la configurazione lotto, le soglie di
# coerenza, e le funzioni di persistenza del modello
from train_model import train_and_evaluate_model, BATCH_CONFIG, load_model, save_model

# Soglie usate dal controllo di coerenza embedded (transit_len vs lotto dichiarato).
# Fonte: docs/sorting_classes_taxonomy.md - "Regola di Identificazione del Calibro".
STANDARD_MIN_TRANSIT_LEN = 12
CHERRY_MAX_TRANSIT_LEN = 12


def _namespace_c_code(code: str, suffix: str) -> str:
    """
    m2cgen genera sempre le stesse 3 funzioni top-level (score, add_vectors,
    mul_vector_number). Dato che ora includiamo DUE modelli (standard e
    cherry) nello stesso header, dobbiamo rinominarle per evitare simboli
    duplicati in fase di compilazione C.
    """
    for name in ("score", "add_vectors", "mul_vector_number"):
        code = re.sub(rf"\b{name}\b", f"{name}_{suffix}", code)
    return code


def _export_single_model(batch_type, data_dir="data/raw", use_cached=True, model_dir="models"):
    """
    Prepara tutti i pezzi C necessari (codice namespaced, elenco feature,
    mappa classi reali) per UN modello.

    Se use_cached=True (default) prova prima a caricare un modello gia'
    salvato con save_model() -- evita di riallenare da zero ad ogni export
    e rende tracciabile QUALE modello e' stato effettivamente esportato
    (vedi models/model_<batch_type>_metadata.json). Se non esiste ancora
    nessun modello salvato, fa fallback automatico al training e lo salva
    per la prossima volta.
    """
    cfg = BATCH_CONFIG[batch_type]

    model = feature_cols = None
    if use_cached:
        try:
            model, feature_cols, metadata = load_model(batch_type, model_dir=model_dir)
            print(f"[INFO] Modello '{batch_type}' caricato da cache "
                  f"(allenato il {metadata['trained_at_utc']}, {metadata['n_samples']} campioni). "
                  f"Uso --retrain per riallenare da zero.")
        except FileNotFoundError:
            print(f"[INFO] Nessun modello in cache per '{batch_type}', alleno da zero...")

    if model is None:
        model, feature_cols, df = train_and_evaluate_model(data_dir, batch_type=batch_type, verbose=True)
        save_model(model, feature_cols, batch_type, df, model_dir=model_dir)

    c_code_raw = m2c.export_to_c(model)
    c_code = _namespace_c_code(c_code_raw, batch_type)

    # model.classes_ e' l'ordine REALE con cui m2cgen indicizza l'array di output:
    # es. per il lotto cherry, output[0] potrebbe non essere la classe "5".
    # Costruiamo una tabella di mapping esplicita per non doverci fidare a memoria.
    real_labels = [int(c) for c in model.classes_]

    return {
        "batch_type": batch_type,
        "cfg": cfg,
        "feature_cols": feature_cols,
        "c_code": c_code,
        "real_labels": real_labels,
        "n_classes": len(real_labels),
    }


def export_model_to_cpp(data_dir="data/raw", header_dir="include", use_cached=True):
    """
    Addestra i DUE modelli (standard e cherry) e li converte in un unico
    header C/C++ nativo (senza dipendenze) per l'esecuzione in tempo reale
    su microcontrollori e PLC industriali.

    Il tipo di lotto (TomatoBatchMode) e' un parametro esplicito passato
    dal chiamante -- va impostato UNA VOLTA per turno/lotto, non calcolato
    per ogni singolo frutto. Non e' piu' una feature nascosta nel vettore
    di input, come lo era "is_cherry" nella versione precedente.
    """
    print("==========================================================")
    print("  AVVIO ESPORTAZIONE FIRMWARE C++ (2 modelli: standard/cherry)")
    print("==========================================================\n")

    standard = _export_single_model("standard", data_dir, use_cached=use_cached)
    cherry = _export_single_model("cherry", data_dir, use_cached=use_cached)

    # Le due pipeline (standard/cherry) derivano dalle stesse colonne del
    # dataset processato, quindi lo schema di feature deve combaciare:
    # verifichiamolo esplicitamente invece di darlo per scontato.
    if standard["feature_cols"] != cherry["feature_cols"]:
        raise RuntimeError(
            "Lo schema delle feature differisce tra lotto standard e cherry: "
            f"{standard['feature_cols']} vs {cherry['feature_cols']}. "
            "Controllare data_loader.py prima di generare il firmware."
        )
    feature_cols = standard["feature_cols"]
    n_features = len(feature_cols)

    os.makedirs(header_dir, exist_ok=True)
    output_path = os.path.join(header_dir, "tomato_classifier.h")
    print(f"[INFO] Formattazione del file di intestazione: {output_path}...")

    # --- Documentazione ordine feature (identico per entrambi i modelli) ---
    feature_doc = "/**\n * @brief ORDINE DELLE FEATURE DA PASSARE ALL'ARRAY 'input' (uguale per entrambi i lotti):\n"
    for idx, col in enumerate(feature_cols):
        feature_doc += f" *   input[{idx}] = {col};\n"
    feature_doc += " *\n * NOTA: 'is_cherry' NON e' piu' tra le feature. Il tipo di lotto va\n"
    feature_doc += " * dichiarato esplicitamente scegliendo BATCH_STANDARD o BATCH_CHERRY.\n"
    feature_doc += " */\n"

    # --- Mappatura commentata delle etichette per ciascun modello ---
    labels_doc = """/**
 * @brief CLASSI DI OUTPUT GLOBALI:
 *   0 = Pomodoro Verde Standard        | 4 = Pomodoro Rosso Standard      (lotto STANDARD)
 *   1 = Pomodoro Giallo-Verde          | 5 = Pomodorino Giallo (Cherry)   (lotto CHERRY)
 *   2 = Pomodoro Arancio-Giallo        | 6 = Pomodorino Rosso (Cherry)    (lotto CHERRY)
 *   3 = Pomodoro Rosso-Arancio         | 7 = Pomodorino Verde Scuro Sfumato (lotto CHERRY)
 *
 * predict_tomato_class() restituisce sempre l'ID GLOBALE (0-7) qui sopra,
 * indipendentemente da come m2cgen ha ordinato internamente le classi.
 */
"""

    labels_array_standard = ", ".join(str(v) for v in standard["real_labels"])
    labels_array_cherry = ", ".join(str(v) for v in cherry["real_labels"])

    header_content = f"""#ifndef TOMATO_CLASSIFIER_H
#define TOMATO_CLASSIFIER_H

/**
 * @warning COMPILAZIONE: le funzioni score_standard()/score_cherry() qui sotto
 * sono generate da m2cgen e usano compound literal in stile C99
 * (es. "(double[]){{...}}" dentro memcpy). Questa sintassi e' C valido ma
 * NON e' C++ standard: un compilatore C++ conforme allo standard la
 * rifiuta con errori "taking address of temporary array", anche dentro
 * un blocco extern "C" (che cambia solo il name mangling, non la
 * grammatica del linguaggio).
 *
 * -> Se il progetto e' in puro C: nessun problema, compila cosi' com'e'.
 * -> Se il progetto e' in C++ (tipico con STM32CubeIDE/HAL misto C/C++):
 *    isolare questo file (o la parte generata da m2cgen) in una unita' di
 *    compilazione .c separata, compilarla con il compilatore C, e linkarla
 *    al resto del progetto C++ tramite gli usuali extern "C". Solo la
 *    funzione helper predict_tomato_class() sotto e' pensata per essere
 *    inclusa direttamente in codice C++.
 */

#ifdef __cplusplus
extern "C" {{
#endif

// --- DOCUMENTAZIONE SPECIFICA SENSORE ---
{feature_doc}
{labels_doc}

// ==========================================================================
// CONFIGURAZIONE LOTTO (BATCH MODE)
// --------------------------------------------------------------------------
// Il tipo di lotto va impostato UNA VOLTA per turno/campagna di raccolta,
// da chi integra questo header (operatore linea / sistema a monte), non
// calcolato frutto per frutto. Non esiste un modo per dedurlo in modo
// affidabile dal solo segnale ottico di un singolo frutto isolato.
// ==========================================================================
typedef enum {{
    BATCH_STANDARD = 0,
    BATCH_CHERRY = 1
}} TomatoBatchMode;

// Soglia UNIFICATA di coerenza (dati verificati in docs/sorting_classes_taxonomy.md):
//  - lotto CHERRY: 0/23 cherry nel dataset superano transit_len = {CHERRY_MAX_TRANSIT_LEN} -> soglia affidabile al 100%
//  - lotto STANDARD: usiamo la STESSA soglia ({STANDARD_MIN_TRANSIT_LEN}) invece di un valore
//    scelto ad hoc, cosi' un frutto con transit_len <= {STANDARD_MIN_TRANSIT_LEN} in un lotto
//    standard e' fisicamente indistinguibile da un cherry vero e va segnalato.
//    NOTA: questo non azzera i falsi positivi (14.6% dei pomodori standard reali
//    ha transit_len <= {STANDARD_MIN_TRANSIT_LEN}) -- c'e' una sovrapposizione fisica
//    reale che una soglia singola su transit_len non puo' eliminare del tutto.
#define TOMATO_STANDARD_MIN_TRANSIT_LEN {STANDARD_MIN_TRANSIT_LEN}.0
#define TOMATO_CHERRY_MAX_TRANSIT_LEN {CHERRY_MAX_TRANSIT_LEN}.0

/**
 * @brief Controllo di coerenza tra il lotto dichiarato e la misura fisica
 *        reale del frutto. Da chiamare PRIMA di fidarsi della classificazione:
 *        se ritorna 1, il frutto e' un'anomalia (es. un cherry finito per
 *        errore in un lotto standard, o viceversa) e andrebbe segnalato/
 *        gestito a parte invece di essere smistato ciecamente.
 * @param mode        Lotto dichiarato (BATCH_STANDARD o BATCH_CHERRY).
 * @param transit_len Lunghezza di transito misurata (input[0] nell'array feature).
 * @return int 1 se e' un'ANOMALIA (incoerenza), 0 se e' coerente.
 *
 * NOTA: non e' dichiarata 'inline'. In C99 puro un 'inline' senza una
 * definizione esterna altrove puo' non generare alcun simbolo collegabile
 * da altre unita' di compilazione (es. un file .ino/.cpp separato che la
 * chiama tramite extern "C") -- un bug di linking subdolo su toolchain
 * embedded rigorosamente C99 (xtensa-esp32, arm-none-eabi-gcc, ecc.).
 */
int tomato_check_batch_anomaly(TomatoBatchMode mode, double transit_len) {{
    if (mode == BATCH_STANDARD) {{
        // Confronto <= (non <): 10 dei 23 cherry nel dataset hanno
        // transit_len ESATTAMENTE uguale alla soglia -- un confronto stretto
        // < li lascerebbe passare come "coerenti standard" per errore.
        return transit_len <= TOMATO_STANDARD_MIN_TRANSIT_LEN;
    }} else {{
        return transit_len > TOMATO_CHERRY_MAX_TRANSIT_LEN;
    }}
}}

// --- CODICE GENERATO AUTOMATICAMENTE DA M2CGEN - MODELLO LOTTO STANDARD (5 classi) ---
// input:  Array di {n_features} double con i valori letti dal sensore ottico
// output: Array di {standard["n_classes"]} double, punteggio per ciascuna classe standard
{standard["c_code"]}

// --- CODICE GENERATO AUTOMATICAMENTE DA M2CGEN - MODELLO LOTTO CHERRY (3 classi) ---
// input:  Array di {n_features} double con i valori letti dal sensore ottico
// output: Array di {cherry["n_classes"]} double, punteggio per ciascuna classe cherry
{cherry["c_code"]}

#ifdef __cplusplus
}}

// --- FUNZIONE HELPER C++ PER INFERENZA REAL-TIME ---

// Mappa: indice interno del modello -> ID di classe globale (0-7), come da
// tabella "CLASSI DI OUTPUT GLOBALI" sopra. Necessaria perche' m2cgen ordina
// le classi internamente e l'ordine puo' non coincidere con l'ID globale
// (es. nel modello cherry, l'indice 0 del suo output corrisponde alla
// classe globale {cherry["real_labels"][0]}, non alla classe 0).
static const int TOMATO_LABELS_STANDARD[{standard["n_classes"]}] = {{ {labels_array_standard} }};
static const int TOMATO_LABELS_CHERRY[{cherry["n_classes"]}]   = {{ {labels_array_cherry} }};

/**
 * @brief Esegue l'inferenza per il lotto indicato e restituisce l'ID di
 *        classe GLOBALE (0-7) vincente.
 * @param mode  Lotto dichiarato per questo turno/linea (BATCH_STANDARD o BATCH_CHERRY).
 * @param input Array con le {n_features} feature calcolate in streaming (senza is_cherry!).
 *              NOTA: non 'const' perche' le funzioni score_* generate da
 *              m2cgen richiedono un puntatore non-const (anche se non
 *              modificano il contenuto).
 * @return int ID GLOBALE della classe predetta (da 0 a 7).
 */
inline int predict_tomato_class(TomatoBatchMode mode, double* input) {{
    if (mode == BATCH_STANDARD) {{
        double class_scores[{standard["n_classes"]}] = {{0.0}};
        score_standard(input, class_scores);
        int best = 0;
        double max_score = class_scores[0];
        for (int i = 1; i < {standard["n_classes"]}; ++i) {{
            if (class_scores[i] > max_score) {{ max_score = class_scores[i]; best = i; }}
        }}
        return TOMATO_LABELS_STANDARD[best];
    }} else {{
        double class_scores[{cherry["n_classes"]}] = {{0.0}};
        score_cherry(input, class_scores);
        int best = 0;
        double max_score = class_scores[0];
        for (int i = 1; i < {cherry["n_classes"]}; ++i) {{
            if (class_scores[i] > max_score) {{ max_score = class_scores[i]; best = i; }}
        }}
        return TOMATO_LABELS_CHERRY[best];
    }}
}}
#endif

#endif // TOMATO_CLASSIFIER_H
"""

    with open(output_path, "w", encoding="utf-8") as f:
        f.write(header_content)

    print(f"\n[SUCCESS] Firmware generato! File salvato in: {output_path}")
    print(f" -> 2 modelli inclusi (standard: {standard['n_classes']} classi, cherry: {cherry['n_classes']} classi)")
    print(f" -> Il chiamante deve dichiarare BATCH_STANDARD o BATCH_CHERRY esplicitamente")
    print("==========================================================\n")

    return output_path


if __name__ == "__main__":
    import argparse

    parser = argparse.ArgumentParser(
        description="Esporta i modelli standard/cherry in un header C/C++ per firmware embedded."
    )
    parser.add_argument(
        "--retrain", action="store_true",
        help="Riallena da zero invece di usare i modelli in cache (models/*.pkl), e sovrascrive la cache."
    )
    args = parser.parse_args()

    export_model_to_cpp(use_cached=not args.retrain)
