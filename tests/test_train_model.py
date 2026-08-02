"""
Test sul modulo train_model.py.

Diversi di questi test sono "test di regressione" mirati: codificano
esplicitamente i bug reali trovati durante lo sviluppo di questo progetto
(vedi README, sezioni "Architettura a Due Modelli" e "Regola di
Identificazione del Calibro"), cosi' che se qualcuno li reintroduce per
errore in futuro, il test fallisce invece di scoprirlo in produzione.
"""
import os
import pytest

from train_model import (
    train_and_evaluate_model,
    check_batch_consistency,
    BATCH_CONFIG,
    COLS_TO_EXCLUDE,
)

DATA_DIR = os.path.join(os.path.dirname(__file__), "..", "data", "raw")


def test_batch_config_classes_are_disjoint():
    std_classes = set(BATCH_CONFIG["standard"]["classes"])
    cherry_classes = set(BATCH_CONFIG["cherry"]["classes"])
    assert std_classes.isdisjoint(cherry_classes)
    assert std_classes | cherry_classes == set(range(8))


def test_is_cherry_never_in_feature_columns():
    """
    Test di regressione sul problema principale di questo progetto: is_cherry
    era originariamente inclusa come feature di input, causando data leakage
    (il modello 'leggeva l'etichetta' invece di interpretare il segnale
    ottico -- vedi README). Deve restare sempre esclusa.
    """
    assert "is_cherry" in COLS_TO_EXCLUDE


@pytest.mark.parametrize("batch_type,expected_n_classes", [
    ("standard", 5),
    ("cherry", 3),
])
def test_model_has_expected_number_of_classes(batch_type, expected_n_classes):
    model, feature_cols, df = train_and_evaluate_model(DATA_DIR, batch_type=batch_type, verbose=False)
    assert len(model.classes_) == expected_n_classes
    assert "is_cherry" not in feature_cols, "is_cherry non deve mai essere una feature di training"


def test_feature_columns_identical_across_batches():
    """I due modelli devono usare lo stesso schema di feature (stesso ordine),
    altrimenti l'header C generato avrebbe un array di input incoerente tra
    i due modelli (vedi export_embedded.py, il controllo esplicito che fa
    la stessa verifica prima di generare il firmware)."""
    _, feat_std, _ = train_and_evaluate_model(DATA_DIR, batch_type="standard", verbose=False)
    _, feat_cherry, _ = train_and_evaluate_model(DATA_DIR, batch_type="cherry", verbose=False)
    assert feat_std == feat_cherry


# --- Test sulla soglia di coerenza (transit_len <= 12) ---
# Vedi docs/sorting_classes_taxonomy.md, "Regola di Identificazione del Calibro":
# soglia unificata a 12 step encoder, scelta perche' cattura il 100% dei
# cherry osservati nel dataset (0 eccezioni su 23 campioni).

def test_cherry_at_exact_threshold_is_coherent():
    """Un cherry con transit_len esattamente 12 (10 dei 23 campioni reali
    sono esattamente a questo valore) deve risultare COERENTE per un lotto
    cherry."""
    assert check_batch_consistency("cherry", 12.0) is True


def test_standard_at_exact_threshold_is_anomaly():
    """Lo stesso valore di confine (12) deve risultare un'ANOMALIA se il
    lotto dichiarato e' standard -- il confronto deve essere '>', non '>=',
    altrimenti si perderebbero meta' dei cherry rilevabili (vedi commento
    in check_batch_consistency())."""
    assert check_batch_consistency("standard", 12.0) is False


def test_standard_above_threshold_is_coherent():
    assert check_batch_consistency("standard", 12.1) is True


def test_cherry_above_threshold_is_anomaly():
    assert check_batch_consistency("cherry", 12.1) is False


def test_threshold_catches_100_percent_of_real_cherries():
    """Verifica end-to-end, sui dati reali: la soglia deve catturare TUTTI
    i cherry del dataset se dichiarati per errore in un lotto standard."""
    from data_loader import load_and_process_dataset
    df = load_and_process_dataset(DATA_DIR)
    cherry_rows = df[df["is_cherry"] == 1]
    assert len(cherry_rows) > 0, "Nessun campione cherry trovato nel dataset di test"

    detected = cherry_rows["transit_len"].apply(
        lambda t: not check_batch_consistency("standard", t)
    )
    assert detected.all(), (
        f"{(~detected).sum()} cherry su {len(cherry_rows)} NON verrebbero "
        f"rilevati come anomalia se dichiarati in un lotto standard."
    )
