import os
import glob
import pandas as pd
import numpy as np

# Mappatura ufficiale delle classi in base al prefisso del file CSV
CLASS_MAPPING = {
    "01": {"label": 0, "name": "Pomodoro Verde Standard", "is_cherry": 0},
    "02": {"label": 1, "name": "Pomodoro Giallo-Verde", "is_cherry": 0},
    "03": {"label": 2, "name": "Pomodoro Arancio-Giallo", "is_cherry": 0},
    "04": {"label": 3, "name": "Pomodoro Rosso-Arancio", "is_cherry": 0},
    "05": {"label": 4, "name": "Pomodoro Rosso Standard", "is_cherry": 0},
    "13": {"label": 5, "name": "Pomodorino Giallo (Cherry)", "is_cherry": 1},
    "15": {"label": 6, "name": "Pomodorino Rosso (Cherry)", "is_cherry": 1},
    "16": {"label": 7, "name": "Pomodorino Verde Scuro Sfumato", "is_cherry": 1}
}

def load_and_process_dataset(raw_data_dir="data/raw"):
    """
    Scansiona le cartelle dei dati grezzi, pulisce il rumore del sensore ottico
    e aggrega le misurazioni line-scan in feature tabellari per singolo frutto.
    """
    print(f"[INFO] Scansione file CSV in corso nella cartella: {raw_data_dir}...")
    
    # Trova tutti i file .csv all'interno di data/raw/ e delle sue sottocartelle
    csv_files = glob.glob(os.path.join(raw_data_dir, "**", "*.csv"), recursive=True)
    
    if not csv_files:
        raise FileNotFoundError(f"Nessun file CSV trovato in {raw_data_dir}. Controlla il percorso!")
        
    all_tomatoes = []
    global_tomato_counter = 0
    
    for file_path in sorted(csv_files):
        file_name = os.path.basename(file_path)
        prefix = file_name[:2]
        
        # Se il prefisso non è nella nostra lista standard (es. file di prova strani), lo saltiamo con un avviso
        if prefix not in CLASS_MAPPING:
            print(f"[WARNING] File ignorato (prefisso '{prefix}' non riconosciuto): {file_name}")
            continue
            
        class_info = CLASS_MAPPING[prefix]
        print(f" -> Caricamento lotto: {file_name} | Classe: {class_info['name']}")
        
        try:
            # Leggiamo il CSV grezzo (supporta sia ; che , come separatore grazie a sep=None)
            df = pd.read_csv(file_path, sep=None, engine='python', header=None)
        except Exception as e:
            print(f"[ERROR] Impossibile leggere {file_name}: {e}")
            continue
            
        # Controlliamo il numero di colonne per identificare il semaforo di validità
        num_cols = df.shape[1]
        if num_cols >= 17:
            validity_col = 16
        else:
            # Se è uno dei file esportati a 16 colonne (senza flag), assumiamo che siano già state pulite
            validity_col = None
            
        # Rinominiamo le colonne chiave per facilitare la lettura del codice
        # Col 1: frame_id | Col 4: transit_len | Col 8: IR1 | Col 9: IR2 | Col 10: Green
        # Col 11: Red | Col 12: Blue | Col 13: IR3 | Col 14: SAT | Col 15: HUE
        
        # REGOLA D'ORO #2: Identifichiamo l'inizio di ogni singolo frutto
        # Quando la colonna 1 (frame_id) vale 1, significa che la fotocellula ha visto un nuovo frutto
        df['is_new_fruit'] = (df[1] == 1).astype(int)
        df['local_fruit_id'] = df['is_new_fruit'].cumsum()
        
        # Iteriamo su ogni singolo frutto trovato in questo file
        for fruit_id, fruit_group in df.groupby('local_fruit_id'):
            # REGOLA D'ORO #1: Filtriamo via la spazzatura e il fondo nastro
            if validity_col is not None:
                valid_frames = fruit_group[fruit_group[validity_col] == 0]
            else:
                valid_frames = fruit_group
                
            # Se un frutto ha generato solo frame invalidi (rumore puro), lo scartiamo
            if len(valid_frames) == 0:
                continue
                
            global_tomato_counter += 1
            unique_id = f"tomato_{global_tomato_counter:04d}"
            
            # Calcoliamo le medie e deviazioni standard dei canali (Simulazione streaming O(1))
            mean_vals = valid_frames[[8, 9, 10, 11, 12, 13, 14, 15]].mean()
            std_vals = valid_frames[[8, 9, 10, 11, 12, 13, 14, 15]].std().fillna(0)
            
            # Calibro fisico (lunghezza di transito in step encoder dalla colonna 4)
            transit_length = valid_frames[4].iloc[0]
            num_valid_slices = len(valid_frames)
            
            # Costruiamo la "Carta d'Identità" di questo singolo pomodoro
            tomato_feature_row = {
                "tomato_id": unique_id,
                "label": class_info["label"],
                "class_name": class_info["name"],
                "is_cherry": class_info["is_cherry"],
                "source_file": file_name,
                "transit_len": transit_length,
                "valid_slices": num_valid_slices,
                
                # Medie spettrali
                "IR1_mean": mean_vals[8],
                "IR2_mean": mean_vals[9],
                "Green_mean": mean_vals[10],
                "Red_mean": mean_vals[11],
                "Blue_mean": mean_vals[12],
                "IR3_mean": mean_vals[13],
                "SAT_mean": mean_vals[14],
                "HUE_mean": mean_vals[15],
                
                # Deviazioni standard (disomogeneità della buccia / macchie)
                "IR1_std": std_vals[8],
                "Green_std": std_vals[10],
                "Red_std": std_vals[11],
                "SAT_std": std_vals[14],
                "HUE_std": std_vals[15],
            }
            
            # Calcolo degli Indici di Maturazione e Lesione di Dominio
            # Aggiungiamo 1e-5 (un numero piccolissimo) al denominatore per evitare divisioni per zero
            tomato_feature_row["Red_Green_ratio"] = tomato_feature_row["Red_mean"] / (tomato_feature_row["Green_mean"] + 1e-5)
            tomato_feature_row["IR1_IR2_ratio"] = tomato_feature_row["IR1_mean"] / (tomato_feature_row["IR2_mean"] + 1e-5)
            tomato_feature_row["IR3_IR2_ratio"] = tomato_feature_row["IR3_mean"] / (tomato_feature_row["IR2_mean"] + 1e-5)
            
            all_tomatoes.append(tomato_feature_row)
            
    # Trasformiamo la lista in un DataFrame pandas
    features_df = pd.DataFrame(all_tomatoes)
    print(f"\n[SUCCESS] Elaborazione completata! Estratti {len(features_df)} pomodori puliti da {len(csv_files)} file CSV.")
    return features_df

# Blocco di test per verificare che lo script funzioni quando lo esegui da terminale
if __name__ == "__main__":
    # Eseguiamo il test di caricamento
    dataset = load_and_process_dataset("data/raw")
    
    print("\n=== ANTEPRIMA DEL DATASET ELABORATO ===")
    print(dataset[["tomato_id", "class_name", "transit_len", "HUE_mean", "Red_Green_ratio"]].head(5))
    
    print("\n=== DISTRIBUZIONE POMODORI PER CLASSE ===")
    print(dataset["class_name"].value_counts())