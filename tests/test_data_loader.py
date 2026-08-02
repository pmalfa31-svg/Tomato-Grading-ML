"""
Test sul modulo data_loader.py: verificano che il dataset processato abbia
la forma e le proprietà attese, sui dati reali del repo (data/raw/).
"""
import os
import pytest

from data_loader import load_and_process_dataset

DATA_DIR = os.path.join(os.path.dirname(__file__), "..", "data", "raw")


@pytest.fixture(scope="module")
def df():
    return load_and_process_dataset(DATA_DIR)


def test_dataset_not_empty(df):
    assert len(df) > 0, "Il dataset processato e' vuoto: controlla data/raw/"


def test_all_8_classes_present(df):
    """Le 8 classi della tassonomia (docs/sorting_classes_taxonomy.md) devono
    essere tutte rappresentate nel dataset processato."""
    labels_found = set(df["label"].unique())
    assert labels_found == set(range(8)), (
        f"Attese le classi 0-7, trovate: {sorted(labels_found)}"
    )


def test_is_cherry_matches_label_range(df):
    """is_cherry deve essere coerente con la classe: 0-4 standard, 5-7 cherry.
    Guardia contro un disallineamento silenzioso tra CLASS_MAPPING e la
    logica di filtro usata in train_model.py."""
    standard_rows = df[df["label"].between(0, 4)]
    cherry_rows = df[df["label"].between(5, 7)]
    assert (standard_rows["is_cherry"] == 0).all(), "Trovate righe standard con is_cherry=1"
    assert (cherry_rows["is_cherry"] == 1).all(), "Trovate righe cherry con is_cherry=0"


def test_no_missing_values_in_key_columns(df):
    key_cols = ["tomato_id", "label", "class_name", "is_cherry", "transit_len", "valid_slices"]
    for col in key_cols:
        assert df[col].isna().sum() == 0, f"Trovati valori mancanti in '{col}'"


def test_tomato_id_is_unique(df):
    """Ogni riga rappresenta un frutto fisico distinto (nessuna scansione
    duplicata) -- e' l'assunzione alla base della scelta di GroupKFold
    per-pomodoro invece che per-giornata (vedi README, sezione ML)."""
    assert df["tomato_id"].is_unique, (
        "tomato_id non e' univoco: la scelta di raggruppare la GroupKFold "
        "per tomato_id andrebbe rivista se esistono scansioni duplicate."
    )


def test_transit_len_positive(df):
    assert (df["transit_len"] > 0).all(), "transit_len non puo' essere <= 0"
