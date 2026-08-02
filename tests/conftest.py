"""
Configurazione condivisa per pytest: aggiunge src/ al path di import, cosi'
i file di test possono fare 'from train_model import ...' senza percorsi
relativi ripetuti in ogni file.
"""
import sys
import os

sys.path.insert(0, os.path.join(os.path.dirname(__file__), "..", "src"))
