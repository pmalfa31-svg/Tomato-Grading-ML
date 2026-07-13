import os
import m2cgen as m2c

# Importiamo la funzione di addestramento dal nostro modulo
from train_model import train_and_evaluate_model

def export_model_to_cpp():
    """
    Addestra il modello finale e lo converte in codice C/C++ nativo (senza dipendenze)
    per l'esecuzione in tempo reale su microcontrollori e PLC industriali.
    """
    print("==========================================================")
    print("  AVVIO ESPORTAZIONE FIRMWARE C++      ")
    print("==========================================================\n")
    
    # 1. Addestriamo/Recuperiamo il modello definitivo e i nomi delle feature
    model, feature_cols, _ = train_and_evaluate_model("data/raw")
    
    # 2. Generazione del codice C grezzo tramite m2cgen
    print("\n[INFO] Traduzione dell'albero Random Forest in codice C nativo...")
    c_code_raw = m2c.export_to_c(model)
    
    # 3. Creazione del file Header C++ professionale
    header_dir = "include"
    os.makedirs(header_dir, exist_ok=True)
    output_path = os.path.join(header_dir, "tomato_classifier.h")
    
    print(f"[INFO] Formattazione del file di intestazione: {output_path}...")
    
    # Costruiamo il commento di documentazione con l'ordine esatto delle feature
    feature_doc = "/**\n * @brief ORDINE DELLE FEATURE DA PASSARE ALL'ARRAY 'input':\n"
    for idx, col in enumerate(feature_cols):
        feature_doc += f" *   input[{idx}] = {col};\n"
    feature_doc += " */\n"
    
    # Mappatura commentata delle etichette di output per il programmatore C++
    labels_doc = """/**
 * @brief CLASSI DI OUTPUT (Indice del valore massimo nell'array 'output'):
 *   0 = Pomodoro Verde Standard        | 4 = Pomodoro Rosso Standard
 *   1 = Pomodoro Giallo-Verde          | 5 = Pomodorino Giallo (Cherry)
 *   2 = Pomodoro Arancio-Giallo        | 6 = Pomodorino Rosso (Cherry)
 *   3 = Pomodoro Rosso-Arancio         | 7 = Pomodorino Verde Scuro Sfumato
 */
"""

    # Assembliamo il file .h completo di guardie di inclusione e helper C++
    header_content = f"""#ifndef TOMATO_CLASSIFIER_H
#define TOMATO_CLASSIFIER_H

#ifdef __cplusplus
extern "C" {{
#endif

// --- DOCUMENTAZIONE SPECIFICA SENSORE ---
{feature_doc}
{labels_doc}

// --- CODICE GENERATO AUTOMATICAMENTE DA M2CGEN (ALBERO DI DECISIONE) ---
// Questa funzione calcola il punteggio di voto per tutte le 8 classi.
// Parametri:
//   input:  Array di {len(feature_cols)} double con i valori letti dal sensore ottico
//   output: Array di 8 double dove verrà salvato il punteggio di ogni classe
{c_code_raw}

#ifdef __cplusplus
}}

// --- FUNZIONE HELPER C++ PER INFERENZA REAL-TIME ---
/**
 * @brief Esegue l'inferenza e restituisce direttamente l'ID della classe vincente.
 * @param input Array con le {len(feature_cols)} feature calcolate in streaming.
 * @return int ID della classe predetta (da 0 a 7).
 */
inline int predict_tomato_class(const double* input) {{
    double class_scores[8] = {{0.0}};
    
    // Chiamata alla funzione di inferenza pura generata da m2cgen
    score(input, class_scores);
    
    // Trova la classe con il punteggio più alto (ArgMax)
    int best_class = 0;
    double max_score = class_scores[0];
    for (int i = 1; i < 8; ++i) {{
        if (class_scores[i] > max_score) {{
            max_score = class_scores[i];
            best_class = i;
        }}
    }}
    return best_class;
}}
#endif

#endif // TOMATO_CLASSIFIER_H
"""

    # Salva il file sul disco
    with open(output_path, "w", encoding="utf-8") as f:
        f.write(header_content)
        
    print(f"\n[SUCCESS] Firmware generato! File salvato in: {output_path}")
    print(f" -> Il microcontrollore può ora eseguire l'inferenza")
    print("==========================================================\n")

if __name__ == "__main__":
    export_model_to_cpp()