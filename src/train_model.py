import os
import pandas as pd
import numpy as np
from sklearn.ensemble import RandomForestClassifier
from sklearn.metrics import classification_report, confusion_matrix
from sklearn.model_selection import GroupKFold, cross_val_score, cross_val_predict

# Importiamo la funzione di caricamento dal nostro modulo data_loader
from data_loader import load_and_process_dataset

def train_and_evaluate_model(data_dir="data/raw"):
    """
    Addestra il modello di smistamento ottico su base Random Forest,
    valutando le performance con GroupKFold per evitare il Data Leakage.
    """
    print("==================================================")
    print(" AVVIO PIPELINE DI TRAINING - OPTICAL SORTING  ")
    print("==================================================\n")
    
    # 1. Carichiamo il dataset pulito in streaming O(1)
    df = load_and_process_dataset(data_dir)
    
    # Escludiamo le colonne di servizio o di testo che non sono feature numeriche
    cols_to_exclude = ["tomato_id", "label", "class_name", "source_file"]
    feature_cols = [col for col in df.columns if col not in cols_to_exclude]
    
    X = df[feature_cols]
    y = df["label"]
    groups = df["tomato_id"] # Fondamentale per la GroupKFold!
    
    print(f"\n[INFO] Configurazione Dataset:")
    print(f" -> Frutti totali (campioni): {len(df)}")
    print(f" -> Feature spettrali/fisiche calcolate: {len(feature_cols)}")
    print(f" -> Classi di smistamento uniche: {y.nunique()}")
    
    # 2. Configurazione dell'Albero di Decisione Industriale
    # Teniamo una profondità massima (max_depth) pari a 6 per garantire che l'inferenza
    # in C++ sul microcontrollore richieda pochissimi salti logici (< 1 microsecondo).
    model = RandomForestClassifier(
        n_estimators=35,
        max_depth=6,
        random_state=42,
        class_weight="balanced" # Rilancia le classi con meno campioni (es. i casi difficili)
    )
    
    # 3. Validazione incrociata rigorosa con GroupKFold (5 Folds)
    print("\n[INFO] Esecuzione Cross-Validation rigorosa (GroupKFold a 5 split)...")
    gkf = GroupKFold(n_splits=5)
    cv_scores = cross_val_score(model, X, y, cv=gkf, groups=groups, scoring="accuracy")
    
    print("\n================= RISULTATI VALIDAZIONE =================")
    print(f" Accuratezza Fold singole: {np.round(cv_scores * 100, 2)} %")
    print(f" ACCURATEZZA MEDIA:     {cv_scores.mean() * 100:.2f}% (+/- {cv_scores.std() * 100:.2f}%)")
    print("=========================================================\n")
    
    # 4. Addestramento finale sull'intero dataset e analisi delle Feature
    print("[INFO] Addestramento del modello finale su tutti i dati disponibili...")
    model.fit(X, y)
    
    # Estraiamo l'importanza di ogni feature secondo l'algoritmo
    importances = pd.Series(model.feature_importances_, index=feature_cols).sort_values(ascending=False)
    
    print("\n CLASSIFICA FEATURE PIÙ DISCRIMINANTI (Top 8):")
    print("---------------------------------------------------------")
    for idx, (feature_name, importance_val) in enumerate(importances.head(8).items(), 1):
        bar_len = int(importance_val * 40)
        bar_str = "█" * bar_len
        print(f" {idx}. {feature_name:<18} | {importance_val*100:5.2f}% | {bar_str}")
    print("---------------------------------------------------------\n")
    
    
    # 5. ANALISI OPERATIVA RIGOROSA SU DATI MAI VISTI (OUT-OF-FOLD)
    print("================ PROVA DEL NOVE ================")
    
    # Generiamo le predizioni per TUTTO il dataset, ma assicurandoci tramite GroupKFold
    # che ogni pomodoro venga predetto da un modello che NON LO HA MAI VISTO nel training!
    print("[INFO] Calcolo predizioni out-of-fold...")
    y_pred_oof = cross_val_predict(model, X, y, cv=gkf, groups=groups)
    
    # Matrice di Confusione reale e non inquinata
    cm = confusion_matrix(y, y_pred_oof)
    print("\n[MATRICE DI CONFUSIONE")
    print(cm)
    
    
    # Calcolo Accuratezza Operativa Binaria (Scarto vs. Buono) su dati mai visti:
    # - SCARTO (0): Verde (0), Giallo-Verde (1), Verde Scuro Sfumato (7)
    # - BUONO  (1): Arancio-Giallo (2), Rosso-Arancio (3), Rosso (4), Cherry Giallo (5), Cherry Rosso (6)
    classes_scarto = [0, 1, 7]
    
    y_real_binary = y.apply(lambda val: 0 if val in classes_scarto else 1)
    y_pred_binary = pd.Series(y_pred_oof).apply(lambda val: 0 if val in classes_scarto else 1)
    
    binary_accuracy = (y_real_binary == y_pred_binary).mean() * 100
    
    print("-----------------------------------------------------------------")
    print(f" 🎯 ACCURATEZZA OPERATIVA BINARIA (Su Pomodori mai visti prima!):")
    print(f"    👉 {binary_accuracy:.2f}% di decisioni pneumatiche corrette!")
    print("-----------------------------------------------------------------\n")

    return model, feature_cols, df

if __name__ == "__main__":
    # Eseguiamo l'addestramento da terminale
    trained_model, features, dataset = train_and_evaluate_model("data/raw")
    print("[SUCCESS] Addestramento completato.")