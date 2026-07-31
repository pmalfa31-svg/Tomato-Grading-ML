import os
import pandas as pd
import numpy as np
from sklearn.ensemble import RandomForestClassifier
from sklearn.metrics import classification_report, confusion_matrix
from sklearn.model_selection import GroupKFold, cross_val_score, cross_val_predict

# Importiamo la funzione di caricamento dal nostro modulo data_loader
from data_loader import load_and_process_dataset

# ==========================================================================
# CONFIGURAZIONE LOTTO (BATCH MODE)
# --------------------------------------------------------------------------
# Il campo non raccoglie mai pomodori standard e cherry insieme: ogni lotto
# è omogeneo e il tipo e' noto A PRIORI (impostato dall'operatore/monte linea)
# PRIMA che un frutto passi sotto il sensore.
#
# Per questo motivo NON alleniamo piu' un unico modello a 8 classi che usa
# "is_cherry" come fosse una misura del sensore (era un data leak: quel
# valore non e' mai calcolabile in tempo reale da un frutto isolato).
# Alleniamo invece DUE modelli distinti, uno per tipo di lotto, selezionati
# da un parametro esplicito "batch_type" definito qui sotto.
# ==========================================================================
BATCH_CONFIG = {
    "standard": {
        "is_cherry_value": 0,
        "classes": [0, 1, 2, 3, 4],
        "classes_scarto": [0, 1],          # sottoinsieme di "classes" da scartare
        "label_name": "STANDARD",
    },
    "cherry": {
        "is_cherry_value": 1,
        "classes": [5, 6, 7],
        "classes_scarto": [7],             # Verde Scuro Sfumato = scarto anche tra i cherry
        "label_name": "CHERRY",
    },
}

# Colonne che non sono feature di input per il sensore ottico.
# "is_cherry" e' qui perche', all'interno di un singolo batch_type, e'
# costante per costruzione (non porta piu' nessuna informazione predittiva
# ne' nessun leak, dato che filtriamo il dataset PRIMA di addestrare).
COLS_TO_EXCLUDE = ["tomato_id", "label", "class_name", "source_file", "is_cherry"]


def train_and_evaluate_model(data_dir="data/raw", batch_type="standard", n_splits=5, verbose=True):
    """
    Addestra il modello di smistamento ottico su base Random Forest per UN SOLO
    tipo di lotto (batch_type in {"standard", "cherry"}), valutando le
    performance con GroupKFold.

    batch_type e' un parametro esplicito di configurazione linea, non una
    feature del sensore: va impostato una volta per turno/lotto da chi
    integra il firmware, non calcolato frutto per frutto.
    """
    if batch_type not in BATCH_CONFIG:
        raise ValueError(f"batch_type deve essere uno tra {list(BATCH_CONFIG)}, ricevuto: {batch_type!r}")

    cfg = BATCH_CONFIG[batch_type]

    if verbose:
        print("==================================================")
        print(f" AVVIO PIPELINE DI TRAINING - LOTTO {cfg['label_name']}")
        print("==================================================\n")

    # 1. Carichiamo il dataset pulito completo (tutti i tipi di frutto)
    df_full = load_and_process_dataset(data_dir)

    # 2. Filtriamo SOLO le righe del tipo di lotto richiesto.
    #    Questo e' il punto chiave: "is_cherry" seleziona il sottoinsieme di
    #    dati e quindi IL MODELLO da usare, non e' piu' un input del modello.
    df = df_full[df_full["is_cherry"] == cfg["is_cherry_value"]].reset_index(drop=True)

    if df.empty:
        raise ValueError(f"Nessun campione trovato per batch_type={batch_type!r} in {data_dir}")

    feature_cols = [col for col in df.columns if col not in COLS_TO_EXCLUDE]

    X = df[feature_cols]
    y = df["label"]
    groups = df["tomato_id"]  # Fondamentale per la GroupKFold!

    if verbose:
        print(f"\n[INFO] Configurazione Dataset (lotto {cfg['label_name']}):")
        print(f" -> Frutti totali (campioni): {len(df)}")
        print(f" -> Feature spettrali/fisiche calcolate: {len(feature_cols)}")
        print(f" -> Classi di smistamento uniche: {y.nunique()} -> {sorted(y.unique().tolist())}")
        counts = y.value_counts().sort_index()
        print(f" -> Distribuzione classi: {counts.to_dict()}")

    # GroupKFold richiede n_splits <= numero di gruppi disponibili E, per
    # avere una stima sensata, meno split del conteggio della classe piu' rara.
    min_class_count = int(y.value_counts().min())
    n_groups = groups.nunique()
    effective_splits = max(2, min(n_splits, min_class_count, n_groups))
    if effective_splits < n_splits and verbose:
        print(f"[WARNING] n_splits ridotto da {n_splits} a {effective_splits}: "
              f"la classe piu' rara ha solo {min_class_count} campioni.")

    # 3. Configurazione dell'Albero di Decisione Industriale
    # Teniamo una profondita' massima (max_depth) pari a 6 per limitare i salti
    # logici PER SINGOLO ALBERO. Con n_estimators=35 il costo reale di
    # inferenza e' fino a 35 alberi x 6 livelli, non 6 salti totali: la stima
    # di latenza va verificata con un benchmark sull'hardware target, non
    # dedotta solo da max_depth.
    model = RandomForestClassifier(
        n_estimators=35,
        max_depth=6,
        random_state=42,
        class_weight="balanced"  # Rilancia le classi con meno campioni (es. i casi difficili)
    )

    # 4. Validazione incrociata con GroupKFold
    if verbose:
        print(f"\n[INFO] Esecuzione Cross-Validation (GroupKFold a {effective_splits} split)...")
    gkf = GroupKFold(n_splits=effective_splits)
    cv_scores = cross_val_score(model, X, y, cv=gkf, groups=groups, scoring="accuracy")

    if verbose:
        print("\n================= RISULTATI VALIDAZIONE =================")
        print(f" Accuratezza Fold singole: {np.round(cv_scores * 100, 2)} %")
        print(f" ACCURATEZZA MEDIA:     {cv_scores.mean() * 100:.2f}% (+/- {cv_scores.std() * 100:.2f}%)")
        print("=========================================================\n")

    # 5. Addestramento finale sull'intero sottoinsieme e analisi delle Feature
    if verbose:
        print("[INFO] Addestramento del modello finale su tutti i dati disponibili...")
    model.fit(X, y)

    importances = pd.Series(model.feature_importances_, index=feature_cols).sort_values(ascending=False)

    if verbose:
        print(f"\n CLASSIFICA FEATURE PIÙ DISCRIMINANTI ({cfg['label_name']}, Top 8):")
        print("---------------------------------------------------------")
        for idx, (feature_name, importance_val) in enumerate(importances.head(8).items(), 1):
            bar_len = int(importance_val * 40)
            bar_str = "█" * bar_len
            print(f" {idx}. {feature_name:<18} | {importance_val*100:5.2f}% | {bar_str}")
        print("---------------------------------------------------------\n")

    # 6. ANALISI OPERATIVA RIGOROSA SU DATI MAI VISTI (OUT-OF-FOLD)
    if verbose:
        print("================ PROVA DEL NOVE ================")
        print("[INFO] Calcolo predizioni out-of-fold...")
    y_pred_oof = cross_val_predict(model, X, y, cv=gkf, groups=groups)

    cm = confusion_matrix(y, y_pred_oof, labels=cfg["classes"])
    if verbose:
        print(f"\n[MATRICE DI CONFUSIONE - lotto {cfg['label_name']}] (ordine classi: {cfg['classes']})")
        print(cm)

    # Accuratezza operativa binaria SCARTO vs BUONO, calcolata solo sulle
    # classi presenti in questo lotto (classes_scarto e' un sottoinsieme di cfg["classes"])
    classes_scarto = set(cfg["classes_scarto"])
    y_real_binary = y.apply(lambda val: 0 if val in classes_scarto else 1)
    y_pred_binary = pd.Series(y_pred_oof).apply(lambda val: 0 if val in classes_scarto else 1)
    binary_accuracy = (y_real_binary == y_pred_binary).mean() * 100

    if verbose:
        print("-----------------------------------------------------------------")
        print(f" 🎯 ACCURATEZZA OPERATIVA BINARIA - lotto {cfg['label_name']} (dati mai visti):")
        print(f"    👉 {binary_accuracy:.2f}% di decisioni pneumatiche corrette!")
        if min_class_count < 10:
            print(f"    ⚠️  Attenzione: la classe piu' rara ha solo {min_class_count} campioni.")
            print(f"       Questa percentuale ha un margine d'errore ampio, non e' una stima solida.")
        print("-----------------------------------------------------------------\n")

    return model, feature_cols, df


def check_batch_consistency(batch_type, transit_len,
                             standard_min_transit_len=12,
                             cherry_max_transit_len=12):
    """
    Controllo di coerenza in Python (specchio di quello generato in C):
    verifica che la lunghezza di transito fisica misurata dal sensore sia
    compatibile con il tipo di lotto dichiarato. Se non lo e', il frutto e'
    un'anomalia (es. un cherry finito per errore in un lotto standard) e
    andrebbe segnalato invece di essere classificato ciecamente.

    Soglia UNIFICATA a 12 step encoder (sia per standard che per cherry),
    scelta perche' e' il valore massimo osservato per i cherry nel dataset
    (0 eccezioni su 23 campioni): qualunque frutto con transit_len <= 12 in
    un lotto STANDARD e' fisicamente indistinguibile da un cherry vero e va
    trattato come sospetto.

    NOTA: questa soglia riduce ma non azzera i falsi positivi sul lato
    standard. Sui dati attuali, il 14.6% dei pomodori standard (39/268) ha
    transit_len <= 12 pur essendo standard: c'e' una sovrapposizione fisica
    reale tra i pomodori standard piu' piccoli/corti e i cherry piu' grandi,
    che transit_len da solo non puo' risolvere del tutto. Vedi
    docs/sorting_classes_taxonomy.md, sezione "Regola di Identificazione
    del Calibro", per i dettagli e i numeri completi.
    """
    if batch_type == "standard":
        # Confronto stretto (>): un frutto con transit_len ESATTAMENTE 12 va
        # trattato come "coerente lato cherry", non "coerente lato standard" --
        # 10 dei 23 cherry nel dataset hanno transit_len == 12 esatto, quindi
        # un confronto >= qui perderebbe la meta' dei cherry rilevabili.
        return transit_len > standard_min_transit_len
    elif batch_type == "cherry":
        return transit_len <= cherry_max_transit_len
    raise ValueError(f"batch_type sconosciuto: {batch_type!r}")


if __name__ == "__main__":
    # Eseguiamo l'addestramento per ENTRAMBI i tipi di lotto da terminale
    results = {}
    for bt in BATCH_CONFIG:
        model, features, dataset = train_and_evaluate_model("data/raw", batch_type=bt)
        results[bt] = (model, features, dataset)

    print("[SUCCESS] Addestramento completato per entrambi i lotti (standard + cherry).")
