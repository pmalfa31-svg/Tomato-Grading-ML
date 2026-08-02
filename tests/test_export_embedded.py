"""
Test sul modulo export_embedded.py.

Il test piu' importante qui (test_generated_header_compiles_as_c) compila
davvero l'header generato con gcc: e' un test di regressione diretto sui
bug reali trovati durante lo sviluppo (link C99 'inline' rotto, soglia di
coerenza con l'operatore di confronto sbagliato al bordo). Un errore di
sintassi o di linking nell'header verrebbe scoperto qui, non su una scheda
ESP32 dopo un upload.
"""
import os
import shutil
import subprocess
import tempfile
import pytest

from export_embedded import _namespace_c_code, export_model_to_cpp

DATA_DIR = os.path.join(os.path.dirname(__file__), "..", "data", "raw")

GCC_AVAILABLE = shutil.which("gcc") is not None


def test_namespace_c_code_renames_all_three_functions():
    code = "void score(double* a) { add_vectors(a); mul_vector_number(a); }"
    out = _namespace_c_code(code, "standard")
    assert "score_standard" in out
    assert "add_vectors_standard" in out
    assert "mul_vector_number_standard" in out
    # Non deve rimanere nessun riferimento al nome originale non suffissato
    assert "void score(" not in out


def test_namespace_c_code_does_not_collide_between_batches():
    code = "void score(double* a) { add_vectors(a); }"
    out_std = _namespace_c_code(code, "standard")
    out_cherry = _namespace_c_code(code, "cherry")
    assert out_std != out_cherry
    assert "score_standard" in out_std
    assert "score_cherry" in out_cherry


@pytest.mark.skipif(not GCC_AVAILABLE, reason="gcc non disponibile in questo ambiente")
def test_generated_header_compiles_as_c(tmp_path):
    """
    Genera l'header vero (riallenando se serve) e lo compila con gcc in
    modalita' C99 -- proprio come avverrebbe su un firmware embedded reale.
    Se questo test fallisce, il firmware non funzionerebbe su hardware vero.
    """
    header_dir = tmp_path / "include"
    export_model_to_cpp(data_dir=DATA_DIR, header_dir=str(header_dir), use_cached=True)

    header_path = header_dir / "tomato_classifier.h"
    assert header_path.exists()

    test_c = tmp_path / "compile_check.c"
    test_c.write_text(f"""
        #include "{header_path.name}"
        int main() {{
            double input[18] = {{0}};
            double out_std[5] = {{0}};
            double out_cherry[3] = {{0}};
            score_standard(input, out_std);
            score_cherry(input, out_cherry);
            int a1 = tomato_check_batch_anomaly(BATCH_STANDARD, 12.0);
            int a2 = tomato_check_batch_anomaly(BATCH_CHERRY, 12.0);
            return 0;
        }}
    """)

    result = subprocess.run(
        ["gcc", "-std=gnu99", "-I", str(header_dir), str(test_c), "-o", str(tmp_path / "a.out"), "-lm"],
        capture_output=True, text=True,
    )
    assert result.returncode == 0, f"Compilazione C fallita:\n{result.stderr}"
