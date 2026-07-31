#ifndef TOMATO_CLASSIFIER_H
#define TOMATO_CLASSIFIER_H

/**
 * @warning COMPILAZIONE: le funzioni score_standard()/score_cherry() qui sotto
 * sono generate da m2cgen e usano compound literal in stile C99
 * (es. "(double[]){...}" dentro memcpy). Questa sintassi e' C valido ma
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
extern "C" {
#endif

// --- DOCUMENTAZIONE SPECIFICA SENSORE ---
/**
 * @brief ORDINE DELLE FEATURE DA PASSARE ALL'ARRAY 'input' (uguale per entrambi i lotti):
 *   input[0] = transit_len;
 *   input[1] = valid_slices;
 *   input[2] = IR1_mean;
 *   input[3] = IR2_mean;
 *   input[4] = Green_mean;
 *   input[5] = Red_mean;
 *   input[6] = Blue_mean;
 *   input[7] = IR3_mean;
 *   input[8] = SAT_mean;
 *   input[9] = HUE_mean;
 *   input[10] = IR1_std;
 *   input[11] = Green_std;
 *   input[12] = Red_std;
 *   input[13] = SAT_std;
 *   input[14] = HUE_std;
 *   input[15] = Red_Green_ratio;
 *   input[16] = IR1_IR2_ratio;
 *   input[17] = IR3_IR2_ratio;
 *
 * NOTA: 'is_cherry' NON e' piu' tra le feature. Il tipo di lotto va
 * dichiarato esplicitamente scegliendo BATCH_STANDARD o BATCH_CHERRY.
 */

/**
 * @brief CLASSI DI OUTPUT GLOBALI:
 *   0 = Pomodoro Verde Standard        | 4 = Pomodoro Rosso Standard      (lotto STANDARD)
 *   1 = Pomodoro Giallo-Verde          | 5 = Pomodorino Giallo (Cherry)   (lotto CHERRY)
 *   2 = Pomodoro Arancio-Giallo        | 6 = Pomodorino Rosso (Cherry)    (lotto CHERRY)
 *   3 = Pomodoro Rosso-Arancio         | 7 = Pomodorino Verde Scuro Sfumato (lotto CHERRY)
 *
 * predict_tomato_class() restituisce sempre l'ID GLOBALE (0-7) qui sopra,
 * indipendentemente da come m2cgen ha ordinato internamente le classi.
 */


// ==========================================================================
// CONFIGURAZIONE LOTTO (BATCH MODE)
// --------------------------------------------------------------------------
// Il tipo di lotto va impostato UNA VOLTA per turno/campagna di raccolta,
// da chi integra questo header (operatore linea / sistema a monte), non
// calcolato frutto per frutto. Non esiste un modo per dedurlo in modo
// affidabile dal solo segnale ottico di un singolo frutto isolato.
// ==========================================================================
typedef enum {
    BATCH_STANDARD = 0,
    BATCH_CHERRY = 1
} TomatoBatchMode;

// Soglia UNIFICATA di coerenza (dati verificati in docs/sorting_classes_taxonomy.md):
//  - lotto CHERRY: 0/23 cherry nel dataset superano transit_len = 12 -> soglia affidabile al 100%
//  - lotto STANDARD: usiamo la STESSA soglia (12) invece di un valore
//    scelto ad hoc, cosi' un frutto con transit_len <= 12 in un lotto
//    standard e' fisicamente indistinguibile da un cherry vero e va segnalato.
//    NOTA: questo non azzera i falsi positivi (14.6% dei pomodori standard reali
//    ha transit_len <= 12) -- c'e' una sovrapposizione fisica
//    reale che una soglia singola su transit_len non puo' eliminare del tutto.
#define TOMATO_STANDARD_MIN_TRANSIT_LEN 12.0
#define TOMATO_CHERRY_MAX_TRANSIT_LEN 12.0

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
int tomato_check_batch_anomaly(TomatoBatchMode mode, double transit_len) {
    if (mode == BATCH_STANDARD) {
        // Confronto <= (non <): 10 dei 23 cherry nel dataset hanno
        // transit_len ESATTAMENTE uguale alla soglia -- un confronto stretto
        // < li lascerebbe passare come "coerenti standard" per errore.
        return transit_len <= TOMATO_STANDARD_MIN_TRANSIT_LEN;
    } else {
        return transit_len > TOMATO_CHERRY_MAX_TRANSIT_LEN;
    }
}

// --- CODICE GENERATO AUTOMATICAMENTE DA M2CGEN - MODELLO LOTTO STANDARD (5 classi) ---
// input:  Array di 18 double con i valori letti dal sensore ottico
// output: Array di 5 double, punteggio per ciascuna classe standard
#include <string.h>
void add_vectors_standard(double *v1, double *v2, int size, double *result) {
    for(int i = 0; i < size; ++i)
        result[i] = v1[i] + v2[i];
}
void mul_vector_number_standard(double *v1, double num, int size, double *result) {
    for(int i = 0; i < size; ++i)
        result[i] = v1[i] * num;
}
void score_standard(double * input, double * output) {
    double var0[5];
    double var1[5];
    double var2[5];
    double var3[5];
    double var4[5];
    double var5[5];
    double var6[5];
    double var7[5];
    double var8[5];
    double var9[5];
    double var10[5];
    double var11[5];
    double var12[5];
    double var13[5];
    double var14[5];
    double var15[5];
    double var16[5];
    double var17[5];
    double var18[5];
    double var19[5];
    double var20[5];
    double var21[5];
    double var22[5];
    double var23[5];
    double var24[5];
    double var25[5];
    double var26[5];
    double var27[5];
    double var28[5];
    double var29[5];
    double var30[5];
    double var31[5];
    double var32[5];
    double var33[5];
    double var34[5];
    double var35[5];
    if (input[4] <= 434.0357208251953) {
        if (input[14] <= 86.34607315063477) {
            if (input[13] <= 5263.146728515625) {
                if (input[8] <= 11357.4287109375) {
                    if (input[15] <= 2.7963523864746094) {
                        if (input[0] <= 6.5) {
                            memcpy(var35, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var35, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[4] <= 352.8541717529297) {
                            memcpy(var35, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var35, (double[]){0.0, 0.0, 0.0, 0.30174081237911027, 0.6982591876208898}, 5 * sizeof(double));
                        }
                    }
                } else {
                    if (input[4] <= 341.8333282470703) {
                        if (input[11] <= 53.093262672424316) {
                            memcpy(var35, (double[]){0.0, 0.0, 0.0, 0.8455284552845529, 0.15447154471544713}, 5 * sizeof(double));
                        } else {
                            memcpy(var35, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        }
                    } else {
                        memcpy(var35, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                    }
                }
            } else {
                if (input[3] <= 1700.9000244140625) {
                    if (input[1] <= 4.5) {
                        if (input[15] <= 14.084110736846924) {
                            memcpy(var35, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var35, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[10] <= 969.1896667480469) {
                            memcpy(var35, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var35, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                } else {
                    memcpy(var35, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                }
            }
        } else {
            if (input[17] <= 1.5411214232444763) {
                memcpy(var35, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
            } else {
                if (input[17] <= 1.7310978770256042) {
                    memcpy(var35, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                } else {
                    memcpy(var35, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                }
            }
        }
    } else {
        if (input[9] <= 1409.5555419921875) {
            if (input[4] <= 1369.4722290039062) {
                if (input[0] <= 16.5) {
                    if (input[6] <= 245.1999969482422) {
                        memcpy(var35, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                    } else {
                        if (input[5] <= 1002.1000061035156) {
                            memcpy(var35, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var35, (double[]){0.0, 0.918918918918919, 0.08108108108108109, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                } else {
                    if (input[11] <= 369.0744323730469) {
                        if (input[15] <= 2.5721503496170044) {
                            memcpy(var35, (double[]){0.07545900867576837, 0.2698231219315354, 0.6547178693926962, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var35, (double[]){0.0, 0.0, 0.27431109877143056, 0.6135906156729368, 0.11209828555563267}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[15] <= 1.8152355551719666) {
                            memcpy(var35, (double[]){0.358695652173913, 0.6413043478260869, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var35, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                }
            } else {
                memcpy(var35, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
            }
        } else {
            if (input[3] <= 981.25) {
                if (input[9] <= 1915.375) {
                    memcpy(var35, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                } else {
                    if (input[11] <= 131.26576614379883) {
                        memcpy(var35, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var35, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                }
            } else {
                memcpy(var35, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
            }
        }
    }
    double var36[5];
    if (input[9] <= 680.25) {
        if (input[4] <= 437.3333435058594) {
            if (input[14] <= 96.99775695800781) {
                if (input[3] <= 2117.5625) {
                    if (input[7] <= 3281.2142333984375) {
                        if (input[4] <= 229.83333587646484) {
                            memcpy(var36, (double[]){0.0, 0.0, 0.041988950276243095, 0.6011049723756907, 0.35690607734806623}, 5 * sizeof(double));
                        } else {
                            memcpy(var36, (double[]){0.0, 0.0, 0.0, 0.12590799031476999, 0.87409200968523}, 5 * sizeof(double));
                        }
                    } else {
                        memcpy(var36, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                    }
                } else {
                    if (input[17] <= 1.6876121163368225) {
                        if (input[14] <= 33.44061851501465) {
                            memcpy(var36, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var36, (double[]){0.0, 0.0, 0.0, 0.7323943661971831, 0.2676056338028169}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[9] <= 42.916666984558105) {
                            memcpy(var36, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var36, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                }
            } else {
                if (input[3] <= 686.5) {
                    memcpy(var36, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                } else {
                    memcpy(var36, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                }
            }
        } else {
            if (input[7] <= 3864.7291259765625) {
                if (input[4] <= 632.3482055664062) {
                    if (input[4] <= 499.0) {
                        memcpy(var36, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var36, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                    }
                } else {
                    if (input[3] <= 1979.6517944335938) {
                        memcpy(var36, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var36, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                }
            } else {
                if (input[3] <= 2691.1666259765625) {
                    memcpy(var36, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                } else {
                    if (input[15] <= 5.154698371887207) {
                        memcpy(var36, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                    } else {
                        if (input[17] <= 1.5112992525100708) {
                            memcpy(var36, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var36, (double[]){0.0, 0.0, 0.8843537414965986, 0.0, 0.11564625850340135}, 5 * sizeof(double));
                        }
                    }
                }
            }
        }
    } else {
        if (input[15] <= 1.2071012258529663) {
            if (input[0] <= 13.5) {
                if (input[2] <= 1400.3999633789062) {
                    memcpy(var36, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                } else {
                    if (input[17] <= 1.3169685006141663) {
                        memcpy(var36, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var36, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                }
            } else {
                memcpy(var36, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
            }
        } else {
            if (input[8] <= 9767.0556640625) {
                if (input[7] <= 3149.888916015625) {
                    if (input[4] <= 1290.2857055664062) {
                        if (input[15] <= 1.3749059438705444) {
                            memcpy(var36, (double[]){0.4824561403508772, 0.5175438596491228, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var36, (double[]){0.044534412955465584, 0.9554655870445344, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        memcpy(var36, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                } else {
                    memcpy(var36, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                }
            } else {
                if (input[8] <= 10078.79150390625) {
                    memcpy(var36, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                } else {
                    memcpy(var36, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                }
            }
        }
    }
    add_vectors_standard(var35, var36, 5, var34);
    double var37[5];
    if (input[13] <= 2488.65087890625) {
        if (input[8] <= 7944.666748046875) {
            if (input[10] <= 33.941123962402344) {
                if (input[1] <= 1.5) {
                    memcpy(var37, (double[]){0.0, 0.0, 0.9017341040462428, 0.0, 0.09826589595375722}, 5 * sizeof(double));
                } else {
                    memcpy(var37, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                }
            } else {
                if (input[0] <= 28.5) {
                    memcpy(var37, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                } else {
                    if (input[0] <= 31.5) {
                        memcpy(var37, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var37, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                }
            }
        } else {
            if (input[15] <= 2.6067150831222534) {
                if (input[4] <= 1369.4722290039062) {
                    if (input[8] <= 9708.7001953125) {
                        if (input[8] <= 8995.21435546875) {
                            memcpy(var37, (double[]){0.08643401895077421, 0.46360064709960713, 0.4499653339496187, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var37, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[8] <= 10078.79150390625) {
                            memcpy(var37, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var37, (double[]){0.0, 0.40718562874251496, 0.592814371257485, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                } else {
                    if (input[13] <= 254.99271392822266) {
                        memcpy(var37, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var37, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                }
            } else {
                if (input[12] <= 595.0209503173828) {
                    if (input[10] <= 232.02619171142578) {
                        memcpy(var37, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var37, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                    }
                } else {
                    memcpy(var37, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                }
            }
        }
    } else {
        if (input[14] <= 66.82816696166992) {
            if (input[15] <= 8.389950275421143) {
                if (input[14] <= 23.97948455810547) {
                    memcpy(var37, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                } else {
                    if (input[6] <= 289.5416717529297) {
                        if (input[6] <= 266.57142639160156) {
                            memcpy(var37, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var37, (double[]){0.0, 0.0, 0.753623188405797, 0.0, 0.24637681159420285}, 5 * sizeof(double));
                        }
                    } else {
                        memcpy(var37, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                    }
                }
            } else {
                if (input[13] <= 5263.146728515625) {
                    if (input[16] <= 1.479526698589325) {
                        if (input[16] <= 1.335377037525177) {
                            memcpy(var37, (double[]){0.0, 0.0, 0.0, 0.47706422018348627, 0.5229357798165137}, 5 * sizeof(double));
                        } else {
                            memcpy(var37, (double[]){0.0, 0.0, 0.0, 0.04581497797356829, 0.9541850220264317}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[14] <= 21.024786949157715) {
                            memcpy(var37, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var37, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                } else {
                    if (input[10] <= 1511.5076904296875) {
                        if (input[11] <= 89.52102279663086) {
                            memcpy(var37, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var37, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        memcpy(var37, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    }
                }
            }
        } else {
            if (input[16] <= 1.381936252117157) {
                if (input[6] <= 289.8303527832031) {
                    memcpy(var37, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                } else {
                    memcpy(var37, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                }
            } else {
                if (input[5] <= 3450.875) {
                    if (input[14] <= 127.31472778320312) {
                        if (input[7] <= 3478.1666259765625) {
                            memcpy(var37, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var37, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[10] <= 896.887451171875) {
                            memcpy(var37, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var37, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                } else {
                    memcpy(var37, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                }
            }
        }
    }
    add_vectors_standard(var34, var37, 5, var33);
    double var38[5];
    if (input[8] <= 7748.222412109375) {
        if (input[4] <= 223.0) {
            if (input[11] <= 135.70497512817383) {
                if (input[13] <= 2948.25) {
                    if (input[0] <= 7.5) {
                        memcpy(var38, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var38, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    }
                } else {
                    memcpy(var38, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                }
            } else {
                memcpy(var38, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
            }
        } else {
            if (input[5] <= 354.5) {
                memcpy(var38, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
            } else {
                memcpy(var38, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
            }
        }
    } else {
        if (input[13] <= 836.9085083007812) {
            if (input[10] <= 1282.974609375) {
                if (input[15] <= 1.7974448204040527) {
                    if (input[9] <= 1329.875) {
                        if (input[9] <= 864.8958435058594) {
                            memcpy(var38, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var38, (double[]){0.0, 0.9488372093023256, 0.05116279069767441, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[12] <= 294.46202850341797) {
                            memcpy(var38, (double[]){0.2716049382716049, 0.7283950617283951, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var38, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                } else {
                    if (input[4] <= 426.3809509277344) {
                        if (input[14] <= 19.400099754333496) {
                            memcpy(var38, (double[]){0.0, 0.0, 0.0, 0.6099706744868035, 0.39002932551319647}, 5 * sizeof(double));
                        } else {
                            memcpy(var38, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[7] <= 3759.7916259765625) {
                            memcpy(var38, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var38, (double[]){0.0, 0.0, 0.6909090909090909, 0.3090909090909091, 0.0}, 5 * sizeof(double));
                        }
                    }
                }
            } else {
                memcpy(var38, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
            }
        } else {
            if (input[8] <= 9558.79150390625) {
                if (input[13] <= 4419.850830078125) {
                    if (input[6] <= 197.92856979370117) {
                        memcpy(var38, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var38, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                } else {
                    if (input[15] <= 12.34492540359497) {
                        if (input[12] <= 1038.0680236816406) {
                            memcpy(var38, (double[]){0.0, 0.0, 0.5277777777777778, 0.47222222222222227, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var38, (double[]){0.0, 0.0, 0.0, 0.9745649263721553, 0.02543507362784471}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[17] <= 1.6600494384765625) {
                            memcpy(var38, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var38, (double[]){0.0, 0.0, 0.0, 0.8455284552845529, 0.15447154471544713}, 5 * sizeof(double));
                        }
                    }
                }
            } else {
                if (input[17] <= 1.626436173915863) {
                    if (input[4] <= 358.75) {
                        if (input[14] <= 45.31095504760742) {
                            memcpy(var38, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var38, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[14] <= 119.24320220947266) {
                            memcpy(var38, (double[]){0.0, 0.0, 0.2490390068687378, 0.4456487491335308, 0.30531224399773144}, 5 * sizeof(double));
                        } else {
                            memcpy(var38, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                } else {
                    if (input[6] <= 115.58333206176758) {
                        memcpy(var38, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var38, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    }
                }
            }
        }
    }
    add_vectors_standard(var33, var38, 5, var32);
    double var39[5];
    if (input[8] <= 8310.66650390625) {
        if (input[1] <= 3.5) {
            if (input[14] <= 138.3168601989746) {
                if (input[12] <= 14.722432136535645) {
                    if (input[1] <= 1.5) {
                        memcpy(var39, (double[]){0.0, 0.0, 0.6046511627906977, 0.0, 0.39534883720930236}, 5 * sizeof(double));
                    } else {
                        memcpy(var39, (double[]){0.0, 0.0, 0.6046511627906977, 0.0, 0.39534883720930236}, 5 * sizeof(double));
                    }
                } else {
                    memcpy(var39, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                }
            } else {
                memcpy(var39, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
            }
        } else {
            if (input[4] <= 207.20000076293945) {
                memcpy(var39, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
            } else {
                if (input[12] <= 205.7092514038086) {
                    if (input[2] <= 1376.0999755859375) {
                        memcpy(var39, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var39, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                } else {
                    if (input[0] <= 13.5) {
                        memcpy(var39, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var39, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                }
            }
        }
    } else {
        if (input[9] <= 864.8958435058594) {
            if (input[4] <= 442.1071472167969) {
                if (input[14] <= 96.99775695800781) {
                    if (input[3] <= 2570.142822265625) {
                        if (input[6] <= 141.3249969482422) {
                            memcpy(var39, (double[]){0.0, 0.0, 0.0, 0.695187165775401, 0.3048128342245989}, 5 * sizeof(double));
                        } else {
                            memcpy(var39, (double[]){0.0, 0.0, 0.0, 0.3351749539594844, 0.6648250460405157}, 5 * sizeof(double));
                        }
                    } else {
                        memcpy(var39, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    }
                } else {
                    if (input[9] <= 287.6875) {
                        memcpy(var39, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var39, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                }
            } else {
                if (input[12] <= 1978.353515625) {
                    if (input[10] <= 1267.02294921875) {
                        memcpy(var39, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                    } else {
                        if (input[10] <= 1513.9181518554688) {
                            memcpy(var39, (double[]){0.0, 0.5352112676056339, 0.0, 0.46478873239436624, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var39, (double[]){0.0, 0.0, 0.8416120960442954, 0.14119150295479957, 0.017196401000905075}, 5 * sizeof(double));
                        }
                    }
                } else {
                    if (input[10] <= 2068.83056640625) {
                        if (input[9] <= 48.196428298950195) {
                            memcpy(var39, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var39, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        memcpy(var39, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    }
                }
            }
        } else {
            if (input[9] <= 1349.772705078125) {
                memcpy(var39, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
            } else {
                if (input[9] <= 1463.7083129882812) {
                    if (input[16] <= 1.3525028824806213) {
                        memcpy(var39, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var39, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                } else {
                    memcpy(var39, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                }
            }
        }
    }
    add_vectors_standard(var32, var39, 5, var31);
    double var40[5];
    if (input[15] <= 1.2516557574272156) {
        if (input[10] <= 60.758399963378906) {
            if (input[0] <= 6.5) {
                memcpy(var40, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
            } else {
                memcpy(var40, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
            }
        } else {
            if (input[0] <= 28.5) {
                if (input[12] <= 84.72529983520508) {
                    if (input[4] <= 463.75) {
                        memcpy(var40, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var40, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                } else {
                    memcpy(var40, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                }
            } else {
                if (input[4] <= 2166.5982055664062) {
                    memcpy(var40, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                } else {
                    memcpy(var40, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                }
            }
        }
    } else {
        if (input[15] <= 3.970415711402893) {
            if (input[4] <= 1177.4285278320312) {
                if (input[3] <= 1424.5) {
                    if (input[15] <= 1.6571736931800842) {
                        if (input[12] <= 321.5666961669922) {
                            memcpy(var40, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var40, (double[]){0.6335403726708074, 0.0, 0.3664596273291925, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[10] <= 326.8065643310547) {
                            memcpy(var40, (double[]){0.0, 0.5352112676056339, 0.0, 0.46478873239436624, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var40, (double[]){0.0, 0.15777262180974477, 0.8422273781902552, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                } else {
                    if (input[4] <= 512.4285583496094) {
                        memcpy(var40, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var40, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                }
            } else {
                memcpy(var40, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
            }
        } else {
            if (input[4] <= 365.07142639160156) {
                if (input[6] <= 141.3249969482422) {
                    if (input[8] <= 11478.625) {
                        if (input[11] <= 93.20328903198242) {
                            memcpy(var40, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var40, (double[]){0.0, 0.0, 0.0, 0.7665847665847666, 0.2334152334152334}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[13] <= 116.85600280761719) {
                            memcpy(var40, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var40, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                } else {
                    if (input[4] <= 352.5416717529297) {
                        memcpy(var40, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    } else {
                        if (input[12] <= 1605.6055908203125) {
                            memcpy(var40, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var40, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        }
                    }
                }
            } else {
                if (input[6] <= 218.9000015258789) {
                    if (input[8] <= 10576.41650390625) {
                        memcpy(var40, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var40, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                    }
                } else {
                    if (input[12] <= 2080.529296875) {
                        if (input[8] <= 9759.4287109375) {
                            memcpy(var40, (double[]){0.0, 0.0, 0.0, 0.9647495361781077, 0.03525046382189239}, 5 * sizeof(double));
                        } else {
                            memcpy(var40, (double[]){0.0, 0.0, 0.07852487680813862, 0.5620728024161501, 0.3594023207757113}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[6] <= 272.5) {
                            memcpy(var40, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var40, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        }
                    }
                }
            }
        }
    }
    add_vectors_standard(var31, var40, 5, var30);
    double var41[5];
    if (input[4] <= 434.0357208251953) {
        if (input[7] <= 3483.8857421875) {
            if (input[15] <= 5.0546228885650635) {
                if (input[14] <= 95.5450668334961) {
                    if (input[11] <= 20.719934225082397) {
                        if (input[4] <= 87.875) {
                            memcpy(var41, (double[]){0.0, 0.0, 0.4663677130044844, 0.0, 0.5336322869955157}, 5 * sizeof(double));
                        } else {
                            memcpy(var41, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[11] <= 46.32580375671387) {
                            memcpy(var41, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var41, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                } else {
                    memcpy(var41, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                }
            } else {
                if (input[1] <= 5.5) {
                    if (input[8] <= 9414.39990234375) {
                        if (input[4] <= 150.25) {
                            memcpy(var41, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var41, (double[]){0.0, 0.0, 0.0, 0.7323943661971831, 0.2676056338028169}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[11] <= 12.120609283447266) {
                            memcpy(var41, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var41, (double[]){0.0, 0.0, 0.0, 0.9285714285714286, 0.07142857142857142}, 5 * sizeof(double));
                        }
                    }
                } else {
                    if (input[11] <= 120.83053207397461) {
                        if (input[0] <= 19.5) {
                            memcpy(var41, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var41, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[15] <= 8.924805164337158) {
                            memcpy(var41, (double[]){0.0, 0.0, 0.0, 0.8201892744479495, 0.17981072555205047}, 5 * sizeof(double));
                        } else {
                            memcpy(var41, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        }
                    }
                }
            }
        } else {
            if (input[1] <= 5.5) {
                memcpy(var41, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
            } else {
                memcpy(var41, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
            }
        }
    } else {
        if (input[8] <= 8303.9443359375) {
            if (input[3] <= 2461.119140625) {
                memcpy(var41, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
            } else {
                if (input[15] <= 1.2001487612724304) {
                    memcpy(var41, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                } else {
                    memcpy(var41, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                }
            }
        } else {
            if (input[9] <= 864.8958435058594) {
                if (input[2] <= 3505.6875) {
                    if (input[17] <= 1.5378175377845764) {
                        if (input[16] <= 1.3221476674079895) {
                            memcpy(var41, (double[]){0.0, 0.5074626865671642, 0.4925373134328358, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var41, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[13] <= 4719.172119140625) {
                            memcpy(var41, (double[]){0.0, 0.0, 0.3584905660377359, 0.6415094339622642, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var41, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                } else {
                    if (input[12] <= 2100.6217041015625) {
                        if (input[9] <= 403.3214416503906) {
                            memcpy(var41, (double[]){0.0, 0.0, 0.07863111818543574, 0.8442499005173101, 0.07711898129725428}, 5 * sizeof(double));
                        } else {
                            memcpy(var41, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        memcpy(var41, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    }
                }
            } else {
                if (input[9] <= 901.75) {
                    if (input[4] <= 947.75) {
                        memcpy(var41, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var41, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                } else {
                    if (input[4] <= 883.7053527832031) {
                        if (input[6] <= 308.1428527832031) {
                            memcpy(var41, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var41, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        memcpy(var41, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                }
            }
        }
    }
    add_vectors_standard(var30, var41, 5, var29);
    double var42[5];
    if (input[14] <= 97.85238647460938) {
        if (input[13] <= 512.5746459960938) {
            if (input[15] <= 1.2508903741836548) {
                if (input[17] <= 1.1271938681602478) {
                    if (input[0] <= 9.5) {
                        if (input[1] <= 1.5) {
                            memcpy(var42, (double[]){0.0, 0.0, 0.753623188405797, 0.0, 0.24637681159420285}, 5 * sizeof(double));
                        } else {
                            memcpy(var42, (double[]){0.0, 0.0, 0.0, 0.5777777777777778, 0.4222222222222222}, 5 * sizeof(double));
                        }
                    } else {
                        memcpy(var42, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                } else {
                    if (input[14] <= 28.89546012878418) {
                        if (input[2] <= 1889.3499755859375) {
                            memcpy(var42, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var42, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[15] <= 0.9495211541652679) {
                            memcpy(var42, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var42, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                }
            } else {
                if (input[11] <= 520.3931274414062) {
                    if (input[10] <= 1092.3881225585938) {
                        if (input[15] <= 2.519345998764038) {
                            memcpy(var42, (double[]){0.038572607260726074, 0.8275577557755776, 0.1338696369636964, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var42, (double[]){0.0, 0.0, 0.09341906202723148, 0.41792738275340396, 0.4886535552193646}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[13] <= 248.7626495361328) {
                            memcpy(var42, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var42, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                } else {
                    memcpy(var42, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                }
            }
        } else {
            if (input[8] <= 9663.5) {
                if (input[4] <= 150.25) {
                    memcpy(var42, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                } else {
                    if (input[10] <= 1824.0699462890625) {
                        if (input[15] <= 14.80302906036377) {
                            memcpy(var42, (double[]){0.0, 0.0, 0.0, 0.9563218390804598, 0.04367816091954023}, 5 * sizeof(double));
                        } else {
                            memcpy(var42, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[8] <= 9420.28369140625) {
                            memcpy(var42, (double[]){0.0, 0.0, 0.6909090909090909, 0.3090909090909091, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var42, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        }
                    }
                }
            } else {
                if (input[13] <= 4126.805908203125) {
                    memcpy(var42, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                } else {
                    if (input[17] <= 1.563385248184204) {
                        if (input[8] <= 9764.3330078125) {
                            memcpy(var42, (double[]){0.0, 0.0, 0.0, 0.5777777777777778, 0.4222222222222222}, 5 * sizeof(double));
                        } else {
                            memcpy(var42, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[9] <= 49.14285659790039) {
                            memcpy(var42, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var42, (double[]){0.0, 0.0, 0.0, 0.9054726368159204, 0.0945273631840796}, 5 * sizeof(double));
                        }
                    }
                }
            }
        }
    } else {
        if (input[12] <= 670.3762817382812) {
            if (input[9] <= 1227.7999877929688) {
                if (input[6] <= 315.4166717529297) {
                    if (input[8] <= 8728.35693359375) {
                        memcpy(var42, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var42, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                } else {
                    memcpy(var42, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                }
            } else {
                if (input[17] <= 1.3318347930908203) {
                    memcpy(var42, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                } else {
                    memcpy(var42, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                }
            }
        } else {
            if (input[16] <= 1.3804572224617004) {
                if (input[8] <= 9501.04150390625) {
                    if (input[17] <= 1.440260410308838) {
                        if (input[16] <= 1.3202691078186035) {
                            memcpy(var42, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var42, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        memcpy(var42, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                    }
                } else {
                    memcpy(var42, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                }
            } else {
                memcpy(var42, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
            }
        }
    }
    add_vectors_standard(var29, var42, 5, var28);
    double var43[5];
    if (input[5] <= 3004.0416259765625) {
        if (input[6] <= 184.20833587646484) {
            if (input[4] <= 374.23809814453125) {
                if (input[8] <= 9557.2998046875) {
                    if (input[5] <= 1108.9833374023438) {
                        if (input[2] <= 1134.9666748046875) {
                            memcpy(var43, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var43, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[3] <= 1792.625) {
                            memcpy(var43, (double[]){0.0, 0.0, 0.0, 0.9012131715771231, 0.09878682842287695}, 5 * sizeof(double));
                        } else {
                            memcpy(var43, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        }
                    }
                } else {
                    if (input[8] <= 11501.0) {
                        if (input[7] <= 3292.7142333984375) {
                            memcpy(var43, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var43, (double[]){0.0, 0.0, 0.0, 0.7323943661971831, 0.2676056338028169}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[8] <= 11616.875) {
                            memcpy(var43, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var43, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        }
                    }
                }
            } else {
                if (input[10] <= 353.7432556152344) {
                    memcpy(var43, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                } else {
                    memcpy(var43, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                }
            }
        } else {
            if (input[13] <= 2555.0263671875) {
                if (input[8] <= 8255.46630859375) {
                    if (input[12] <= 1257.8802490234375) {
                        if (input[12] <= 195.0392303466797) {
                            memcpy(var43, (double[]){0.8703296703296703, 0.12967032967032968, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var43, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[9] <= 1618.2980346679688) {
                            memcpy(var43, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var43, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                } else {
                    if (input[17] <= 1.5313355922698975) {
                        if (input[8] <= 9809.111328125) {
                            memcpy(var43, (double[]){0.016695931668700335, 0.9253593643046338, 0.05794470402666586, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var43, (double[]){0.0, 0.35345861655337873, 0.5717712914834067, 0.0, 0.07477009196321471}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[10] <= 1329.3692626953125) {
                            memcpy(var43, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var43, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        }
                    }
                }
            } else {
                if (input[8] <= 9660.0) {
                    memcpy(var43, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                } else {
                    if (input[12] <= 1311.3346557617188) {
                        memcpy(var43, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var43, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    }
                }
            }
        }
    } else {
        if (input[17] <= 1.4778482913970947) {
            if (input[14] <= 25.79835796356201) {
                memcpy(var43, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
            } else {
                memcpy(var43, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
            }
        } else {
            if (input[14] <= 42.672786712646484) {
                if (input[3] <= 2360.0594482421875) {
                    if (input[3] <= 2220.5833740234375) {
                        memcpy(var43, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var43, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                    }
                } else {
                    if (input[9] <= 51.64285659790039) {
                        memcpy(var43, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    } else {
                        if (input[9] <= 55.821428298950195) {
                            memcpy(var43, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var43, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        }
                    }
                }
            } else {
                if (input[0] <= 31.5) {
                    memcpy(var43, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                } else {
                    if (input[2] <= 3858.6339111328125) {
                        memcpy(var43, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var43, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                }
            }
        }
    }
    add_vectors_standard(var28, var43, 5, var27);
    double var44[5];
    if (input[15] <= 3.970415711402893) {
        if (input[11] <= 520.3931274414062) {
            if (input[2] <= 1822.8571166992188) {
                if (input[9] <= 901.75) {
                    if (input[3] <= 1313.0119018554688) {
                        if (input[17] <= 1.165988028049469) {
                            memcpy(var44, (double[]){0.0, 0.0, 0.36296840558412935, 0.16238060249816313, 0.4746509919177076}, 5 * sizeof(double));
                        } else {
                            memcpy(var44, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[4] <= 696.40478515625) {
                            memcpy(var44, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var44, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                } else {
                    if (input[8] <= 8199.39990234375) {
                        if (input[14] <= 31.15809917449951) {
                            memcpy(var44, (double[]){0.528, 0.47200000000000003, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var44, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[1] <= 4.5) {
                            memcpy(var44, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var44, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                }
            } else {
                if (input[9] <= 1317.7142944335938) {
                    if (input[10] <= 353.7432556152344) {
                        memcpy(var44, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var44, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                } else {
                    memcpy(var44, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                }
            }
        } else {
            if (input[8] <= 8303.9443359375) {
                if (input[0] <= 28.5) {
                    memcpy(var44, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                } else {
                    if (input[16] <= 1.3012832999229431) {
                        memcpy(var44, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var44, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                }
            } else {
                if (input[15] <= 2.0247544050216675) {
                    memcpy(var44, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                } else {
                    memcpy(var44, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                }
            }
        }
    } else {
        if (input[9] <= 57.41666603088379) {
            if (input[2] <= 2043.0166015625) {
                if (input[8] <= 9476.39990234375) {
                    memcpy(var44, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                } else {
                    if (input[10] <= 232.02619171142578) {
                        memcpy(var44, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var44, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                    }
                }
            } else {
                memcpy(var44, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
            }
        } else {
            if (input[3] <= 2768.0) {
                if (input[16] <= 1.4874187111854553) {
                    if (input[7] <= 1482.875) {
                        memcpy(var44, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    } else {
                        if (input[16] <= 1.3945121765136719) {
                            memcpy(var44, (double[]){0.0, 0.0, 0.08634099449445076, 0.7725246875819278, 0.14113431792362144}, 5 * sizeof(double));
                        } else {
                            memcpy(var44, (double[]){0.0, 0.0, 0.0, 0.9789590254706533, 0.02104097452934662}, 5 * sizeof(double));
                        }
                    }
                } else {
                    memcpy(var44, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                }
            } else {
                if (input[17] <= 1.4508896470069885) {
                    memcpy(var44, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                } else {
                    memcpy(var44, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                }
            }
        }
    }
    add_vectors_standard(var27, var44, 5, var26);
    double var45[5];
    if (input[9] <= 680.25) {
        if (input[9] <= 127.48571014404297) {
            if (input[10] <= 715.0714111328125) {
                if (input[11] <= 15.919926166534424) {
                    if (input[1] <= 1.5) {
                        if (input[0] <= 6.5) {
                            memcpy(var45, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var45, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[14] <= 9.81495475769043) {
                            memcpy(var45, (double[]){0.0, 0.0, 0.0, 0.6459627329192547, 0.35403726708074534}, 5 * sizeof(double));
                        } else {
                            memcpy(var45, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        }
                    }
                } else {
                    memcpy(var45, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                }
            } else {
                if (input[8] <= 10165.34814453125) {
                    if (input[14] <= 32.373456954956055) {
                        if (input[12] <= 1646.8676147460938) {
                            memcpy(var45, (double[]){0.0, 0.0, 0.0, 0.5282167042889391, 0.471783295711061}, 5 * sizeof(double));
                        } else {
                            memcpy(var45, (double[]){0.0, 0.0, 0.0, 0.055026455026455035, 0.944973544973545}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[9] <= 47.154762268066406) {
                            memcpy(var45, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var45, (double[]){0.0, 0.0, 0.08520179372197309, 0.9147982062780269, 0.0}, 5 * sizeof(double));
                        }
                    }
                } else {
                    memcpy(var45, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                }
            }
        } else {
            if (input[7] <= 3866.9375) {
                if (input[17] <= 1.711413860321045) {
                    if (input[13] <= 253.52301025390625) {
                        if (input[11] <= 287.52913665771484) {
                            memcpy(var45, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var45, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[10] <= 1789.346923828125) {
                            memcpy(var45, (double[]){0.0, 0.0, 0.9839167455061495, 0.0, 0.016083254493850518}, 5 * sizeof(double));
                        } else {
                            memcpy(var45, (double[]){0.0, 0.0, 0.5277777777777778, 0.47222222222222227, 0.0}, 5 * sizeof(double));
                        }
                    }
                } else {
                    if (input[2] <= 1454.25) {
                        memcpy(var45, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var45, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    }
                }
            } else {
                if (input[2] <= 3869.5714111328125) {
                    if (input[15] <= 5.457087755203247) {
                        memcpy(var45, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                    } else {
                        if (input[7] <= 3931.5625) {
                            memcpy(var45, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var45, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        }
                    }
                } else {
                    memcpy(var45, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                }
            }
        }
    } else {
        if (input[15] <= 1.1573817133903503) {
            memcpy(var45, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
        } else {
            if (input[4] <= 624.6499938964844) {
                if (input[16] <= 1.2797122597694397) {
                    memcpy(var45, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                } else {
                    if (input[7] <= 1379.2166748046875) {
                        memcpy(var45, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var45, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                }
            } else {
                if (input[13] <= 619.7030334472656) {
                    if (input[5] <= 1082.4857177734375) {
                        if (input[3] <= 1225.17138671875) {
                            memcpy(var45, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var45, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[15] <= 1.2622830867767334) {
                            memcpy(var45, (double[]){0.528, 0.47200000000000003, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var45, (double[]){0.016666666666666663, 0.9833333333333334, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                } else {
                    memcpy(var45, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                }
            }
        }
    }
    add_vectors_standard(var26, var45, 5, var25);
    double var46[5];
    if (input[6] <= 393.8999938964844) {
        if (input[5] <= 1484.7428588867188) {
            if (input[2] <= 1796.952392578125) {
                if (input[5] <= 1089.1000366210938) {
                    if (input[9] <= 968.7000122070312) {
                        if (input[11] <= 135.70497512817383) {
                            memcpy(var46, (double[]){0.0, 0.0, 0.545615197702673, 0.09763640379942568, 0.35674839849790146}, 5 * sizeof(double));
                        } else {
                            memcpy(var46, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[15] <= 1.1218409836292267) {
                            memcpy(var46, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var46, (double[]){0.358695652173913, 0.6413043478260869, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                } else {
                    if (input[6] <= 192.1374969482422) {
                        if (input[2] <= 1539.625) {
                            memcpy(var46, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var46, (double[]){0.0, 0.0, 0.5277777777777778, 0.47222222222222227, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        memcpy(var46, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                }
            } else {
                if (input[8] <= 7949.6787109375) {
                    memcpy(var46, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                } else {
                    memcpy(var46, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                }
            }
        } else {
            if (input[6] <= 141.625) {
                if (input[15] <= 11.913187980651855) {
                    if (input[3] <= 1089.6500244140625) {
                        memcpy(var46, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    } else {
                        if (input[9] <= 37.68333435058594) {
                            memcpy(var46, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var46, (double[]){0.0, 0.0, 0.0, 0.9535452322738386, 0.04645476772616137}, 5 * sizeof(double));
                        }
                    }
                } else {
                    if (input[9] <= 27.766667366027832) {
                        if (input[3] <= 1631.3250122070312) {
                            memcpy(var46, (double[]){0.0, 0.0, 0.0, 0.7323943661971831, 0.2676056338028169}, 5 * sizeof(double));
                        } else {
                            memcpy(var46, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        }
                    } else {
                        memcpy(var46, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    }
                }
            } else {
                if (input[15] <= 8.307056427001953) {
                    if (input[9] <= 281.8928527832031) {
                        if (input[15] <= 6.85018253326416) {
                            memcpy(var46, (double[]){0.0, 0.0, 0.10661486996870616, 0.8585302686953706, 0.034854861335923165}, 5 * sizeof(double));
                        } else {
                            memcpy(var46, (double[]){0.0, 0.0, 0.59353598462093, 0.2124234050222276, 0.1940406103568425}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[9] <= 327.0) {
                            memcpy(var46, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var46, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                } else {
                    if (input[10] <= 1755.4785766601562) {
                        if (input[4] <= 365.92857360839844) {
                            memcpy(var46, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var46, (double[]){0.0, 0.0, 0.0, 0.5777777777777778, 0.4222222222222222}, 5 * sizeof(double));
                        }
                    } else {
                        memcpy(var46, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    }
                }
            }
        }
    } else {
        if (input[16] <= 1.3638045191764832) {
            if (input[4] <= 1757.1832885742188) {
                if (input[16] <= 1.261387288570404) {
                    memcpy(var46, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                } else {
                    if (input[15] <= 1.2964449524879456) {
                        if (input[11] <= 171.92096710205078) {
                            memcpy(var46, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var46, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[5] <= 3397.2388916015625) {
                            memcpy(var46, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var46, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        }
                    }
                }
            } else {
                if (input[2] <= 3210.8094482421875) {
                    memcpy(var46, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                } else {
                    if (input[5] <= 2717.0201416015625) {
                        memcpy(var46, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    } else {
                        if (input[15] <= 1.1156439185142517) {
                            memcpy(var46, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var46, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                }
            }
        } else {
            if (input[3] <= 1967.5803833007812) {
                if (input[16] <= 1.3911151885986328) {
                    memcpy(var46, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                } else {
                    memcpy(var46, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                }
            } else {
                if (input[8] <= 10462.10400390625) {
                    if (input[16] <= 1.4008298516273499) {
                        memcpy(var46, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var46, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                } else {
                    if (input[7] <= 4009.017822265625) {
                        memcpy(var46, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var46, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                }
            }
        }
    }
    add_vectors_standard(var25, var46, 5, var24);
    double var47[5];
    if (input[8] <= 7764.666748046875) {
        if (input[17] <= 1.0852614641189575) {
            if (input[1] <= 1.5) {
                memcpy(var47, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
            } else {
                if (input[0] <= 9.5) {
                    memcpy(var47, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                } else {
                    memcpy(var47, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                }
            }
        } else {
            if (input[6] <= 126.0999984741211) {
                memcpy(var47, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
            } else {
                memcpy(var47, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
            }
        }
    } else {
        if (input[6] <= 400.0) {
            if (input[6] <= 302.2232208251953) {
                if (input[15] <= 3.8651106357574463) {
                    if (input[6] <= 195.28571319580078) {
                        if (input[8] <= 11186.21630859375) {
                            memcpy(var47, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var47, (double[]){0.0, 0.0, 0.5277777777777778, 0.47222222222222227, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[11] <= 190.44442749023438) {
                            memcpy(var47, (double[]){0.0, 0.8782287822878229, 0.12177121771217712, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var47, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                } else {
                    if (input[10] <= 1808.8770141601562) {
                        if (input[3] <= 915.0) {
                            memcpy(var47, (double[]){0.0, 0.0, 0.25365853658536586, 0.0, 0.7463414634146341}, 5 * sizeof(double));
                        } else {
                            memcpy(var47, (double[]){0.0, 0.0, 0.021913676085702882, 0.7058510402342192, 0.27223528368007804}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[1] <= 5.5) {
                            memcpy(var47, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var47, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        }
                    }
                }
            } else {
                if (input[15] <= 4.3482760190963745) {
                    if (input[4] <= 708.3928527832031) {
                        if (input[10] <= 524.9915008544922) {
                            memcpy(var47, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var47, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        memcpy(var47, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                } else {
                    if (input[14] <= 40.54214286804199) {
                        memcpy(var47, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    } else {
                        if (input[16] <= 1.3833808302879333) {
                            memcpy(var47, (double[]){0.0, 0.0, 0.3584905660377359, 0.6415094339622642, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var47, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                }
            }
        } else {
            if (input[8] <= 9901.75) {
                if (input[11] <= 202.2553253173828) {
                    memcpy(var47, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                } else {
                    if (input[10] <= 1808.06591796875) {
                        if (input[13] <= 211.95989227294922) {
                            memcpy(var47, (double[]){0.10060975609756098, 0.899390243902439, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var47, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[16] <= 1.3771893382072449) {
                            memcpy(var47, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var47, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        }
                    }
                }
            } else {
                if (input[4] <= 1187.5089111328125) {
                    memcpy(var47, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                } else {
                    memcpy(var47, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                }
            }
        }
    }
    add_vectors_standard(var24, var47, 5, var23);
    double var48[5];
    if (input[8] <= 8199.39990234375) {
        if (input[4] <= 282.0) {
            if (input[3] <= 1166.6666870117188) {
                if (input[10] <= 436.75) {
                    if (input[1] <= 1.5) {
                        memcpy(var48, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    } else {
                        if (input[0] <= 9.5) {
                            memcpy(var48, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var48, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        }
                    }
                } else {
                    memcpy(var48, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                }
            } else {
                memcpy(var48, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
            }
        } else {
            if (input[2] <= 3347.875) {
                memcpy(var48, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
            } else {
                if (input[15] <= 1.1672114133834839) {
                    if (input[9] <= 2050.25) {
                        memcpy(var48, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var48, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                } else {
                    memcpy(var48, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                }
            }
        }
    } else {
        if (input[17] <= 1.455234706401825) {
            if (input[13] <= 627.7359619140625) {
                if (input[8] <= 10356.2021484375) {
                    if (input[13] <= 253.23638153076172) {
                        if (input[8] <= 9698.0) {
                            memcpy(var48, (double[]){0.06557185436269065, 0.8206416924785226, 0.11378645315878673, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var48, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        memcpy(var48, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                } else {
                    if (input[14] <= 34.98437023162842) {
                        memcpy(var48, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var48, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                }
            } else {
                memcpy(var48, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
            }
        } else {
            if (input[9] <= 200.39286041259766) {
                if (input[9] <= 49.071428298950195) {
                    if (input[2] <= 2262.175048828125) {
                        if (input[16] <= 1.3937729597091675) {
                            memcpy(var48, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var48, (double[]){0.0, 0.0, 0.0, 0.7738095238095238, 0.2261904761904762}, 5 * sizeof(double));
                        }
                    } else {
                        memcpy(var48, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    }
                } else {
                    if (input[5] <= 2746.7713623046875) {
                        if (input[2] <= 2774.928466796875) {
                            memcpy(var48, (double[]){0.0, 0.0, 0.0, 0.1319796954314721, 0.8680203045685279}, 5 * sizeof(double));
                        } else {
                            memcpy(var48, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[15] <= 9.776674747467041) {
                            memcpy(var48, (double[]){0.0, 0.0, 0.04174765486351729, 0.8217696273134455, 0.13648271782303728}, 5 * sizeof(double));
                        } else {
                            memcpy(var48, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        }
                    }
                }
            } else {
                if (input[15] <= 1.6428303122520447) {
                    if (input[8] <= 8719.4287109375) {
                        memcpy(var48, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var48, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                } else {
                    if (input[15] <= 3.9679774045944214) {
                        memcpy(var48, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                    } else {
                        if (input[15] <= 4.507719278335571) {
                            memcpy(var48, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var48, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                }
            }
        }
    }
    add_vectors_standard(var23, var48, 5, var22);
    double var49[5];
    if (input[5] <= 2838.857177734375) {
        if (input[4] <= 517.3000183105469) {
            if (input[6] <= 182.95833587646484) {
                if (input[14] <= 93.14620971679688) {
                    if (input[16] <= 1.4461408853530884) {
                        if (input[0] <= 25.5) {
                            memcpy(var49, (double[]){0.0, 0.0, 0.0, 0.771731448763251, 0.2282685512367491}, 5 * sizeof(double));
                        } else {
                            memcpy(var49, (double[]){0.0, 0.0, 0.17446583083171466, 0.31220201306727885, 0.5133321561010066}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[6] <= 97.25) {
                            memcpy(var49, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var49, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        }
                    }
                } else {
                    if (input[1] <= 2.5) {
                        memcpy(var49, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var49, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                }
            } else {
                if (input[5] <= 507.125) {
                    memcpy(var49, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                } else {
                    if (input[9] <= 797.9464302062988) {
                        memcpy(var49, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var49, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                }
            }
        } else {
            if (input[5] <= 988.9500122070312) {
                if (input[12] <= 137.81217193603516) {
                    if (input[10] <= 193.87628173828125) {
                        memcpy(var49, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var49, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                } else {
                    memcpy(var49, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                }
            } else {
                if (input[17] <= 1.4181222319602966) {
                    if (input[17] <= 1.3449422717094421) {
                        if (input[4] <= 799.4500122070312) {
                            memcpy(var49, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var49, (double[]){0.8075040783034259, 0.19249592169657423, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[15] <= 1.152640700340271) {
                            memcpy(var49, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var49, (double[]){0.018940864662288767, 0.9481911643060925, 0.032867971031618745, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                } else {
                    if (input[9] <= 1390.7857055664062) {
                        if (input[0] <= 16.5) {
                            memcpy(var49, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var49, (double[]){0.0, 0.15465645199904238, 0.8005745750538664, 0.04476897294709121, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[12] <= 316.9563903808594) {
                            memcpy(var49, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var49, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                }
            }
        }
    } else {
        if (input[11] <= 247.1020050048828) {
            if (input[15] <= 9.733527183532715) {
                if (input[17] <= 1.5934642553329468) {
                    if (input[17] <= 1.481737732887268) {
                        memcpy(var49, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var49, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    }
                } else {
                    if (input[4] <= 352.5416717529297) {
                        memcpy(var49, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var49, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                    }
                }
            } else {
                if (input[8] <= 9549.7001953125) {
                    memcpy(var49, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                } else {
                    memcpy(var49, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                }
            }
        } else {
            if (input[3] <= 3115.126953125) {
                if (input[2] <= 3356.45556640625) {
                    memcpy(var49, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                } else {
                    if (input[6] <= 308.8095245361328) {
                        if (input[17] <= 1.4638879895210266) {
                            memcpy(var49, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var49, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[4] <= 1436.9603271484375) {
                            memcpy(var49, (double[]){0.0, 0.0, 0.753623188405797, 0.0, 0.24637681159420285}, 5 * sizeof(double));
                        } else {
                            memcpy(var49, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                }
            } else {
                if (input[11] <= 317.93287658691406) {
                    memcpy(var49, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                } else {
                    memcpy(var49, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                }
            }
        }
    }
    add_vectors_standard(var22, var49, 5, var21);
    double var50[5];
    if (input[9] <= 903.9874877929688) {
        if (input[15] <= 3.970415711402893) {
            if (input[4] <= 1369.4722290039062) {
                if (input[13] <= 139.04312896728516) {
                    if (input[4] <= 513.9166717529297) {
                        if (input[6] <= 80.83333587646484) {
                            memcpy(var50, (double[]){0.0, 0.0, 0.3923749007148531, 0.3510722795869738, 0.2565528196981732}, 5 * sizeof(double));
                        } else {
                            memcpy(var50, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        memcpy(var50, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                } else {
                    if (input[14] <= 14.970583438873291) {
                        memcpy(var50, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var50, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                }
            } else {
                memcpy(var50, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
            }
        } else {
            if (input[4] <= 485.0428466796875) {
                if (input[5] <= 3150.8094482421875) {
                    if (input[0] <= 19.5) {
                        if (input[8] <= 11330.25) {
                            memcpy(var50, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var50, (double[]){0.0, 0.0, 0.0, 0.7323943661971831, 0.2676056338028169}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[8] <= 7859.08349609375) {
                            memcpy(var50, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var50, (double[]){0.0, 0.0, 0.0, 0.7506561679790027, 0.24934383202099736}, 5 * sizeof(double));
                        }
                    }
                } else {
                    if (input[15] <= 8.450368404388428) {
                        memcpy(var50, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                    } else {
                        if (input[14] <= 32.373456954956055) {
                            memcpy(var50, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var50, (double[]){0.0, 0.0, 0.0, 0.7323943661971831, 0.2676056338028169}, 5 * sizeof(double));
                        }
                    }
                }
            } else {
                if (input[7] <= 5049.053466796875) {
                    memcpy(var50, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                } else {
                    memcpy(var50, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                }
            }
        }
    } else {
        if (input[8] <= 7875.166748046875) {
            if (input[15] <= 1.2015577554702759) {
                memcpy(var50, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
            } else {
                if (input[9] <= 1368.5714111328125) {
                    memcpy(var50, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                } else {
                    memcpy(var50, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                }
            }
        } else {
            if (input[15] <= 1.265863060951233) {
                if (input[16] <= 1.36385977268219) {
                    memcpy(var50, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                } else {
                    memcpy(var50, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                }
            } else {
                if (input[15] <= 1.3363964557647705) {
                    if (input[6] <= 651.0113525390625) {
                        memcpy(var50, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var50, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                } else {
                    memcpy(var50, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                }
            }
        }
    }
    add_vectors_standard(var21, var50, 5, var20);
    double var51[5];
    if (input[9] <= 1397.2698364257812) {
        if (input[15] <= 7.156695365905762) {
            if (input[11] <= 597.2088012695312) {
                if (input[16] <= 1.3872619271278381) {
                    if (input[9] <= 675.9375) {
                        if (input[12] <= 957.0927734375) {
                            memcpy(var51, (double[]){0.0, 0.0, 0.2752855948732238, 0.5747190489458531, 0.1499953561809232}, 5 * sizeof(double));
                        } else {
                            memcpy(var51, (double[]){0.0, 0.0, 0.7948511665325824, 0.11853043711450789, 0.08661839635290962}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[17] <= 1.339517593383789) {
                            memcpy(var51, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var51, (double[]){0.15714285714285714, 0.8428571428571429, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                } else {
                    if (input[12] <= 161.6270294189453) {
                        memcpy(var51, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    } else {
                        if (input[4] <= 196.875) {
                            memcpy(var51, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var51, (double[]){0.0, 0.0, 0.9043064390645738, 0.08091162875840922, 0.014781932177017067}, 5 * sizeof(double));
                        }
                    }
                }
            } else {
                memcpy(var51, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
            }
        } else {
            if (input[10] <= 1808.0170288085938) {
                if (input[8] <= 9664.25) {
                    if (input[9] <= 30.800000190734863) {
                        if (input[11] <= 85.7793960571289) {
                            memcpy(var51, (double[]){0.0, 0.0, 0.0, 0.8914285714285715, 0.10857142857142857}, 5 * sizeof(double));
                        } else {
                            memcpy(var51, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[17] <= 1.5199192762374878) {
                            memcpy(var51, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var51, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                } else {
                    if (input[8] <= 11563.6962890625) {
                        if (input[11] <= 217.24102020263672) {
                            memcpy(var51, (double[]){0.0, 0.0, 0.0, 0.10633946830265849, 0.8936605316973415}, 5 * sizeof(double));
                        } else {
                            memcpy(var51, (double[]){0.0, 0.0, 0.0, 0.8914285714285715, 0.10857142857142857}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[9] <= 77.75) {
                            memcpy(var51, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var51, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                }
            } else {
                if (input[14] <= 58.006635665893555) {
                    if (input[15] <= 7.883877515792847) {
                        memcpy(var51, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var51, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    }
                } else {
                    memcpy(var51, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                }
            }
        }
    } else {
        if (input[1] <= 5.5) {
            if (input[8] <= 6654.89990234375) {
                if (input[12] <= 33.36259841918945) {
                    memcpy(var51, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                } else {
                    memcpy(var51, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                }
            } else {
                if (input[6] <= 396.1000061035156) {
                    if (input[5] <= 870.5499877929688) {
                        memcpy(var51, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var51, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                } else {
                    memcpy(var51, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                }
            }
        } else {
            if (input[15] <= 1.2071012258529663) {
                memcpy(var51, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
            } else {
                if (input[2] <= 3060.1983642578125) {
                    memcpy(var51, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                } else {
                    memcpy(var51, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                }
            }
        }
    }
    add_vectors_standard(var20, var51, 5, var19);
    double var52[5];
    if (input[4] <= 432.9166717529297) {
        if (input[9] <= 178.5) {
            if (input[7] <= 3890.28564453125) {
                if (input[3] <= 1171.6666870117188) {
                    if (input[17] <= 1.2697582840919495) {
                        if (input[0] <= 8.0) {
                            memcpy(var52, (double[]){0.0, 0.0, 0.6709677419354839, 0.0, 0.32903225806451614}, 5 * sizeof(double));
                        } else {
                            memcpy(var52, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        }
                    } else {
                        memcpy(var52, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    }
                } else {
                    if (input[13] <= 4390.899169921875) {
                        if (input[12] <= 818.4621276855469) {
                            memcpy(var52, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var52, (double[]){0.0, 0.0, 0.0, 0.17962003454231434, 0.8203799654576857}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[14] <= 20.97712230682373) {
                            memcpy(var52, (double[]){0.0, 0.0, 0.0, 0.4508670520231214, 0.5491329479768786}, 5 * sizeof(double));
                        } else {
                            memcpy(var52, (double[]){0.0, 0.0, 0.0, 0.8975188781014024, 0.10248112189859762}, 5 * sizeof(double));
                        }
                    }
                }
            } else {
                if (input[3] <= 2373.3333740234375) {
                    memcpy(var52, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                } else {
                    memcpy(var52, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                }
            }
        } else {
            if (input[13] <= 2093.7969970703125) {
                if (input[5] <= 1008.8333129882812) {
                    memcpy(var52, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                } else {
                    memcpy(var52, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                }
            } else {
                if (input[1] <= 4.5) {
                    memcpy(var52, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                } else {
                    memcpy(var52, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                }
            }
        }
    } else {
        if (input[15] <= 2.1284735202789307) {
            if (input[15] <= 1.2071012258529663) {
                memcpy(var52, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
            } else {
                if (input[2] <= 3061.6988525390625) {
                    if (input[6] <= 570.5625) {
                        if (input[1] <= 8.5) {
                            memcpy(var52, (double[]){0.024923363987738232, 0.802079168332667, 0.17299746767959479, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var52, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[16] <= 1.3158173561096191) {
                            memcpy(var52, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var52, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                } else {
                    memcpy(var52, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                }
            }
        } else {
            if (input[5] <= 3373.0) {
                if (input[1] <= 6.5) {
                    if (input[4] <= 486.31666564941406) {
                        memcpy(var52, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                    } else {
                        if (input[15] <= 2.8658111095428467) {
                            memcpy(var52, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var52, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                } else {
                    memcpy(var52, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                }
            } else {
                if (input[7] <= 4759.928466796875) {
                    if (input[0] <= 34.5) {
                        if (input[16] <= 1.3138142228126526) {
                            memcpy(var52, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var52, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        memcpy(var52, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    }
                } else {
                    memcpy(var52, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                }
            }
        }
    }
    add_vectors_standard(var19, var52, 5, var18);
    double var53[5];
    if (input[15] <= 3.970415711402893) {
        if (input[0] <= 28.5) {
            if (input[9] <= 1336.2999877929688) {
                if (input[9] <= 922.75) {
                    if (input[3] <= 1313.0119018554688) {
                        if (input[17] <= 1.1796733736991882) {
                            memcpy(var53, (double[]){0.0, 0.0, 0.44437781109445273, 0.26506746626686656, 0.2905547226386806}, 5 * sizeof(double));
                        } else {
                            memcpy(var53, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[6] <= 201.85713958740234) {
                            memcpy(var53, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var53, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                } else {
                    if (input[4] <= 609.9500122070312) {
                        memcpy(var53, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var53, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                }
            } else {
                if (input[6] <= 279.5) {
                    if (input[8] <= 7912.0498046875) {
                        memcpy(var53, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    } else {
                        if (input[1] <= 4.5) {
                            memcpy(var53, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var53, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                } else {
                    memcpy(var53, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                }
            }
        } else {
            if (input[6] <= 550.0069580078125) {
                if (input[5] <= 1865.6607055664062) {
                    if (input[17] <= 1.488025963306427) {
                        memcpy(var53, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var53, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                } else {
                    if (input[13] <= 296.3768768310547) {
                        memcpy(var53, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var53, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                }
            } else {
                if (input[0] <= 34.5) {
                    if (input[10] <= 1232.26318359375) {
                        memcpy(var53, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var53, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                } else {
                    memcpy(var53, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                }
            }
        }
    } else {
        if (input[3] <= 2570.142822265625) {
            if (input[3] <= 915.0) {
                memcpy(var53, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
            } else {
                if (input[6] <= 226.2738037109375) {
                    if (input[12] <= 1426.8681640625) {
                        if (input[15] <= 6.066704750061035) {
                            memcpy(var53, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var53, (double[]){0.0, 0.0, 0.0, 0.6051537822111389, 0.39484621778886114}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[15] <= 8.260012149810791) {
                            memcpy(var53, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var53, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        }
                    }
                } else {
                    if (input[4] <= 376.6190490722656) {
                        memcpy(var53, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    } else {
                        if (input[16] <= 1.3946613669395447) {
                            memcpy(var53, (double[]){0.0, 0.0, 0.0, 0.7323943661971831, 0.2676056338028169}, 5 * sizeof(double));
                        } else {
                            memcpy(var53, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                }
            }
        } else {
            if (input[13] <= 4399.863037109375) {
                if (input[1] <= 7.5) {
                    if (input[15] <= 8.450368404388428) {
                        if (input[5] <= 3519.6429443359375) {
                            memcpy(var53, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var53, (double[]){0.0, 0.0, 0.5277777777777778, 0.47222222222222227, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        memcpy(var53, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    }
                } else {
                    memcpy(var53, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                }
            } else {
                if (input[0] <= 28.5) {
                    memcpy(var53, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                } else {
                    memcpy(var53, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                }
            }
        }
    }
    add_vectors_standard(var18, var53, 5, var17);
    double var54[5];
    if (input[17] <= 1.455234706401825) {
        if (input[8] <= 7944.666748046875) {
            if (input[3] <= 213.25) {
                if (input[0] <= 9.5) {
                    if (input[0] <= 7.5) {
                        memcpy(var54, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var54, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                    }
                } else {
                    memcpy(var54, (double[]){0.0, 0.0, 0.4333333333333334, 0.0, 0.5666666666666667}, 5 * sizeof(double));
                }
            } else {
                memcpy(var54, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
            }
        } else {
            if (input[7] <= 3808.5166015625) {
                if (input[14] <= 15.923107147216797) {
                    memcpy(var54, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                } else {
                    if (input[12] <= 115.46342468261719) {
                        memcpy(var54, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    } else {
                        if (input[8] <= 9732.0) {
                            memcpy(var54, (double[]){0.08527131782945736, 0.9147286821705427, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var54, (double[]){0.0, 0.2556390977443609, 0.7443609022556391, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                }
            } else {
                memcpy(var54, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
            }
        }
    } else {
        if (input[11] <= 369.0744323730469) {
            if (input[15] <= 4.973653316497803) {
                if (input[4] <= 841.2261962890625) {
                    if (input[10] <= 251.33975219726562) {
                        memcpy(var54, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    } else {
                        if (input[11] <= 70.51800155639648) {
                            memcpy(var54, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var54, (double[]){0.04277862393218992, 0.0, 0.8908019336467783, 0.0664194424210317, 0.0}, 5 * sizeof(double));
                        }
                    }
                } else {
                    if (input[9] <= 1695.1666870117188) {
                        memcpy(var54, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var54, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                }
            } else {
                if (input[9] <= 40.68333435058594) {
                    if (input[4] <= 169.61666107177734) {
                        if (input[8] <= 9235.8251953125) {
                            memcpy(var54, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var54, (double[]){0.0, 0.0, 0.0, 0.7665847665847666, 0.2334152334152334}, 5 * sizeof(double));
                        }
                    } else {
                        memcpy(var54, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    }
                } else {
                    if (input[17] <= 1.7653201818466187) {
                        if (input[7] <= 4006.107177734375) {
                            memcpy(var54, (double[]){0.0, 0.0, 0.0, 0.8097886540600668, 0.19021134593993325}, 5 * sizeof(double));
                        } else {
                            memcpy(var54, (double[]){0.0, 0.0, 0.12104876255819652, 0.3249203626562117, 0.5540308747855918}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[5] <= 824.375) {
                            memcpy(var54, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var54, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        }
                    }
                }
            }
        } else {
            if (input[11] <= 575.8831176757812) {
                if (input[3] <= 2226.2708740234375) {
                    memcpy(var54, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                } else {
                    memcpy(var54, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                }
            } else {
                memcpy(var54, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
            }
        }
    }
    add_vectors_standard(var17, var54, 5, var16);
    double var55[5];
    if (input[9] <= 680.1875) {
        if (input[11] <= 345.0082092285156) {
            if (input[14] <= 47.693586349487305) {
                if (input[15] <= 9.434684753417969) {
                    if (input[17] <= 1.8021485209465027) {
                        if (input[5] <= 3649.5) {
                            memcpy(var55, (double[]){0.0, 0.0, 0.08307058477319545, 0.6317736578803549, 0.28515575734644977}, 5 * sizeof(double));
                        } else {
                            memcpy(var55, (double[]){0.0, 0.0, 0.0, 0.29629629629629634, 0.7037037037037037}, 5 * sizeof(double));
                        }
                    } else {
                        memcpy(var55, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    }
                } else {
                    if (input[7] <= 2717.125) {
                        if (input[16] <= 1.399790644645691) {
                            memcpy(var55, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var55, (double[]){0.0, 0.0, 0.0, 0.8272727272727273, 0.17272727272727273}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[3] <= 1628.7708129882812) {
                            memcpy(var55, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var55, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        }
                    }
                }
            } else {
                if (input[3] <= 2400.5333251953125) {
                    if (input[4] <= 239.1999969482422) {
                        memcpy(var55, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                    } else {
                        if (input[11] <= 293.20697021484375) {
                            memcpy(var55, (double[]){0.0, 0.0, 0.914572864321608, 0.0, 0.08542713567839195}, 5 * sizeof(double));
                        } else {
                            memcpy(var55, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                } else {
                    if (input[7] <= 3847.7083740234375) {
                        memcpy(var55, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var55, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                    }
                }
            }
        } else {
            if (input[7] <= 4062.8125) {
                memcpy(var55, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
            } else {
                if (input[11] <= 358.72772216796875) {
                    memcpy(var55, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                } else {
                    memcpy(var55, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                }
            }
        }
    } else {
        if (input[0] <= 28.5) {
            if (input[8] <= 8423.8193359375) {
                if (input[12] <= 1257.8802490234375) {
                    if (input[2] <= 3292.7833251953125) {
                        if (input[4] <= 904.1499938964844) {
                            memcpy(var55, (double[]){0.8790983606557377, 0.12090163934426229, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var55, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[12] <= 1116.0527954101562) {
                            memcpy(var55, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var55, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                } else {
                    memcpy(var55, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                }
            } else {
                if (input[9] <= 864.8958435058594) {
                    if (input[13] <= 185.43714141845703) {
                        memcpy(var55, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    } else {
                        if (input[13] <= 361.98765563964844) {
                            memcpy(var55, (double[]){0.0, 0.34, 0.66, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var55, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                } else {
                    memcpy(var55, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                }
            }
        } else {
            if (input[8] <= 9852.1806640625) {
                if (input[9] <= 1829.522705078125) {
                    memcpy(var55, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                } else {
                    memcpy(var55, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                }
            } else {
                memcpy(var55, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
            }
        }
    }
    add_vectors_standard(var16, var55, 5, var15);
    double var56[5];
    if (input[9] <= 281.8928527832031) {
        if (input[4] <= 557.0625) {
            if (input[2] <= 3504.702392578125) {
                if (input[17] <= 1.7381788492202759) {
                    if (input[15] <= 7.098873853683472) {
                        if (input[6] <= 254.5666732788086) {
                            memcpy(var56, (double[]){0.0, 0.0, 0.7345724907063198, 0.09389272437599576, 0.17153478491768456}, 5 * sizeof(double));
                        } else {
                            memcpy(var56, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[9] <= 40.68333435058594) {
                            memcpy(var56, (double[]){0.0, 0.0, 0.0, 0.18571428571428572, 0.8142857142857143}, 5 * sizeof(double));
                        } else {
                            memcpy(var56, (double[]){0.0, 0.0, 0.0, 0.8646080760095012, 0.13539192399049882}, 5 * sizeof(double));
                        }
                    }
                } else {
                    if (input[15] <= 14.084110736846924) {
                        memcpy(var56, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var56, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                    }
                }
            } else {
                if (input[7] <= 3922.5714111328125) {
                    memcpy(var56, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                } else {
                    memcpy(var56, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                }
            }
        } else {
            memcpy(var56, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
        }
    } else {
        if (input[9] <= 907.0486145019531) {
            if (input[17] <= 1.3282921314239502) {
                memcpy(var56, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
            } else {
                if (input[0] <= 34.5) {
                    if (input[6] <= 258.7857131958008) {
                        if (input[6] <= 242.85713958740234) {
                            memcpy(var56, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var56, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        memcpy(var56, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                } else {
                    memcpy(var56, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                }
            }
        } else {
            if (input[9] <= 1508.3713989257812) {
                if (input[6] <= 1018.388916015625) {
                    if (input[15] <= 1.265863060951233) {
                        if (input[1] <= 8.0) {
                            memcpy(var56, (double[]){0.18282548476454294, 0.817174515235457, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var56, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[5] <= 1101.5357055664062) {
                            memcpy(var56, (double[]){0.3655913978494624, 0.0, 0.6344086021505376, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var56, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                } else {
                    memcpy(var56, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                }
            } else {
                if (input[13] <= 51.54415702819824) {
                    memcpy(var56, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                } else {
                    memcpy(var56, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                }
            }
        }
    }
    add_vectors_standard(var15, var56, 5, var14);
    double var57[5];
    if (input[4] <= 606.4624938964844) {
        if (input[5] <= 1356.0) {
            if (input[0] <= 9.5) {
                if (input[15] <= 0.9959695637226105) {
                    memcpy(var57, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                } else {
                    if (input[5] <= 1024.5) {
                        if (input[15] <= 4.232895731925964) {
                            memcpy(var57, (double[]){0.0, 0.0, 0.5048543689320389, 0.0, 0.49514563106796117}, 5 * sizeof(double));
                        } else {
                            memcpy(var57, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        }
                    } else {
                        memcpy(var57, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                }
            } else {
                if (input[15] <= 5.917352676391602) {
                    if (input[3] <= 852.9166564941406) {
                        if (input[9] <= 881.5833129882812) {
                            memcpy(var57, (double[]){0.0, 0.0, 0.753623188405797, 0.0, 0.24637681159420285}, 5 * sizeof(double));
                        } else {
                            memcpy(var57, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        memcpy(var57, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                } else {
                    if (input[4] <= 127.36666488647461) {
                        memcpy(var57, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var57, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                    }
                }
            }
        } else {
            if (input[9] <= 56.75) {
                if (input[6] <= 111.36666870117188) {
                    if (input[6] <= 89.2750015258789) {
                        memcpy(var57, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var57, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                    }
                } else {
                    if (input[3] <= 3146.28564453125) {
                        memcpy(var57, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    } else {
                        if (input[13] <= 4509.65771484375) {
                            memcpy(var57, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var57, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        }
                    }
                }
            } else {
                if (input[3] <= 1337.4166870117188) {
                    memcpy(var57, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                } else {
                    if (input[5] <= 3992.267822265625) {
                        if (input[0] <= 19.5) {
                            memcpy(var57, (double[]){0.0, 0.0, 0.0, 0.47706422018348627, 0.5229357798165137}, 5 * sizeof(double));
                        } else {
                            memcpy(var57, (double[]){0.0, 0.0, 0.1566016801394833, 0.8126802979870027, 0.030718021873514027}, 5 * sizeof(double));
                        }
                    } else {
                        memcpy(var57, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    }
                }
            }
        }
    } else {
        if (input[13] <= 2488.65087890625) {
            if (input[9] <= 1397.2698364257812) {
                if (input[9] <= 770.9285583496094) {
                    if (input[3] <= 1395.8333129882812) {
                        memcpy(var57, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    } else {
                        if (input[4] <= 697.2380981445312) {
                            memcpy(var57, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var57, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                } else {
                    if (input[7] <= 1610.4166870117188) {
                        if (input[9] <= 1074.5499877929688) {
                            memcpy(var57, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var57, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[14] <= 175.9742660522461) {
                            memcpy(var57, (double[]){0.017178552837064025, 0.9828214471629361, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var57, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                }
            } else {
                if (input[14] <= 40.43780708312988) {
                    if (input[6] <= 299.8999938964844) {
                        memcpy(var57, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    } else {
                        if (input[13] <= 391.054443359375) {
                            memcpy(var57, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var57, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                } else {
                    memcpy(var57, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                }
            }
        } else {
            if (input[10] <= 1788.462890625) {
                memcpy(var57, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
            } else {
                if (input[10] <= 1822.2112426757812) {
                    memcpy(var57, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                } else {
                    memcpy(var57, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                }
            }
        }
    }
    add_vectors_standard(var14, var57, 5, var13);
    double var58[5];
    if (input[9] <= 864.8958435058594) {
        if (input[9] <= 164.0) {
            if (input[9] <= 56.35000038146973) {
                if (input[5] <= 2372.4375) {
                    if (input[5] <= 2202.8499755859375) {
                        if (input[6] <= 108.95000076293945) {
                            memcpy(var58, (double[]){0.0, 0.0, 0.20312500000000003, 0.0, 0.796875}, 5 * sizeof(double));
                        } else {
                            memcpy(var58, (double[]){0.0, 0.0, 0.0, 0.5777777777777778, 0.4222222222222222}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[16] <= 1.4365278482437134) {
                            memcpy(var58, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var58, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                } else {
                    memcpy(var58, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                }
            } else {
                if (input[16] <= 1.461912453174591) {
                    if (input[7] <= 4176.916748046875) {
                        if (input[0] <= 16.5) {
                            memcpy(var58, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var58, (double[]){0.0, 0.0, 0.043693614010260035, 0.8991685830532461, 0.057137802936493884}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[8] <= 9974.634765625) {
                            memcpy(var58, (double[]){0.0, 0.0, 0.45011389521640094, 0.4027334851936219, 0.14715261958997722}, 5 * sizeof(double));
                        } else {
                            memcpy(var58, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        }
                    }
                } else {
                    if (input[5] <= 1321.5833129882812) {
                        memcpy(var58, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var58, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    }
                }
            }
        } else {
            if (input[11] <= 68.99862861633301) {
                memcpy(var58, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
            } else {
                if (input[8] <= 9750.57177734375) {
                    if (input[16] <= 1.4266844391822815) {
                        if (input[15] <= 3.970415711402893) {
                            memcpy(var58, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var58, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[0] <= 34.5) {
                            memcpy(var58, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var58, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                } else {
                    memcpy(var58, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                }
            }
        }
    } else {
        if (input[6] <= 885.8194580078125) {
            if (input[8] <= 8255.46630859375) {
                if (input[10] <= 279.9795379638672) {
                    if (input[10] <= 213.15773010253906) {
                        memcpy(var58, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var58, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                } else {
                    memcpy(var58, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                }
            } else {
                if (input[16] <= 1.370431661605835) {
                    if (input[11] <= 686.6274719238281) {
                        memcpy(var58, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    } else {
                        if (input[17] <= 1.3689982295036316) {
                            memcpy(var58, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var58, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                } else {
                    if (input[15] <= 1.3693284392356873) {
                        if (input[12] <= 391.50425720214844) {
                            memcpy(var58, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var58, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        memcpy(var58, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                }
            }
        } else {
            if (input[11] <= 777.7081604003906) {
                if (input[5] <= 2207.1508178710938) {
                    memcpy(var58, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                } else {
                    memcpy(var58, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                }
            } else {
                memcpy(var58, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
            }
        }
    }
    add_vectors_standard(var13, var58, 5, var12);
    double var59[5];
    if (input[5] <= 2896.419677734375) {
        if (input[4] <= 432.9166717529297) {
            if (input[2] <= 677.125) {
                if (input[16] <= 1.1479647159576416) {
                    if (input[0] <= 9.5) {
                        if (input[1] <= 1.5) {
                            memcpy(var59, (double[]){0.0, 0.0, 0.5048543689320389, 0.0, 0.49514563106796117}, 5 * sizeof(double));
                        } else {
                            memcpy(var59, (double[]){0.0, 0.0, 0.0, 0.7323943661971831, 0.2676056338028169}, 5 * sizeof(double));
                        }
                    } else {
                        memcpy(var59, (double[]){0.0, 0.0, 0.7536231884057971, 0.0, 0.24637681159420288}, 5 * sizeof(double));
                    }
                } else {
                    if (input[16] <= 1.3674123287200928) {
                        if (input[16] <= 1.3088831305503845) {
                            memcpy(var59, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var59, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        memcpy(var59, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                }
            } else {
                if (input[9] <= 177.375) {
                    if (input[17] <= 1.8115244507789612) {
                        if (input[14] <= 45.31095504760742) {
                            memcpy(var59, (double[]){0.0, 0.0, 0.0, 0.43384982121573307, 0.566150178784267}, 5 * sizeof(double));
                        } else {
                            memcpy(var59, (double[]){0.0, 0.0, 0.0, 0.9425981873111783, 0.057401812688821746}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[11] <= 11.097254753112793) {
                            memcpy(var59, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var59, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        }
                    }
                } else {
                    if (input[10] <= 623.9821624755859) {
                        memcpy(var59, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var59, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                }
            }
        } else {
            if (input[15] <= 1.773399293422699) {
                if (input[8] <= 8186.0498046875) {
                    if (input[2] <= 1413.5) {
                        if (input[15] <= 1.1787781715393066) {
                            memcpy(var59, (double[]){0.2716049382716049, 0.7283950617283951, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var59, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[7] <= 3311.2589111328125) {
                            memcpy(var59, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var59, (double[]){0.6265822784810127, 0.37341772151898733, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                } else {
                    if (input[8] <= 8459.30810546875) {
                        if (input[5] <= 1357.6889038085938) {
                            memcpy(var59, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var59, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[8] <= 8798.0) {
                            memcpy(var59, (double[]){0.10060975609756098, 0.899390243902439, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var59, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                }
            } else {
                if (input[15] <= 3.9662989377975464) {
                    if (input[17] <= 1.4047318696975708) {
                        if (input[16] <= 1.2992175817489624) {
                            memcpy(var59, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var59, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[13] <= 80.83462524414062) {
                            memcpy(var59, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var59, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                } else {
                    memcpy(var59, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                }
            }
        }
    } else {
        if (input[7] <= 3638.988037109375) {
            memcpy(var59, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
        } else {
            if (input[14] <= 31.407182693481445) {
                if (input[16] <= 1.4595487713813782) {
                    if (input[15] <= 7.661236524581909) {
                        if (input[13] <= 2323.906295776367) {
                            memcpy(var59, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var59, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        memcpy(var59, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    }
                } else {
                    if (input[4] <= 356.25) {
                        memcpy(var59, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var59, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                    }
                }
            } else {
                if (input[16] <= 1.3914958238601685) {
                    if (input[9] <= 127.48571014404297) {
                        if (input[14] <= 49.40779495239258) {
                            memcpy(var59, (double[]){0.0, 0.0, 0.3746681835419037, 0.5028441410693971, 0.12248767538869929}, 5 * sizeof(double));
                        } else {
                            memcpy(var59, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[8] <= 9903.70556640625) {
                            memcpy(var59, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var59, (double[]){0.0, 0.0, 0.0, 0.7323943661971831, 0.2676056338028169}, 5 * sizeof(double));
                        }
                    }
                } else {
                    memcpy(var59, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                }
            }
        }
    }
    add_vectors_standard(var12, var59, 5, var11);
    double var60[5];
    if (input[15] <= 2.066505789756775) {
        if (input[6] <= 886.5113525390625) {
            if (input[9] <= 1510.0) {
                if (input[4] <= 549.4500122070312) {
                    if (input[13] <= 644.6384124755859) {
                        if (input[15] <= 1.4364744424819946) {
                            memcpy(var60, (double[]){0.0, 0.0, 0.44156424581005593, 0.1975418994413408, 0.36089385474860336}, 5 * sizeof(double));
                        } else {
                            memcpy(var60, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        memcpy(var60, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                } else {
                    if (input[9] <= 1255.2142944335938) {
                        memcpy(var60, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    } else {
                        if (input[16] <= 1.4048227071762085) {
                            memcpy(var60, (double[]){0.2891566265060241, 0.7108433734939759, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var60, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                }
            } else {
                if (input[14] <= 31.608986854553223) {
                    memcpy(var60, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                } else {
                    memcpy(var60, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                }
            }
        } else {
            if (input[8] <= 7715.166748046875) {
                memcpy(var60, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
            } else {
                memcpy(var60, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
            }
        }
    } else {
        if (input[15] <= 3.970415711402893) {
            if (input[11] <= 177.08414459228516) {
                memcpy(var60, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
            } else {
                memcpy(var60, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
            }
        } else {
            if (input[17] <= 1.5454419255256653) {
                if (input[14] <= 61.93458938598633) {
                    if (input[5] <= 2900.9405517578125) {
                        if (input[4] <= 457.4791564941406) {
                            memcpy(var60, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var60, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        memcpy(var60, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    }
                } else {
                    if (input[12] <= 1587.8114624023438) {
                        memcpy(var60, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var60, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                }
            } else {
                if (input[9] <= 58.875) {
                    if (input[14] <= 32.373456954956055) {
                        if (input[10] <= 1497.5198364257812) {
                            memcpy(var60, (double[]){0.0, 0.0, 0.0, 0.49133858267716535, 0.5086614173228347}, 5 * sizeof(double));
                        } else {
                            memcpy(var60, (double[]){0.0, 0.0, 0.0, 0.0787878787878788, 0.9212121212121213}, 5 * sizeof(double));
                        }
                    } else {
                        memcpy(var60, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                    }
                } else {
                    if (input[7] <= 4072.767822265625) {
                        if (input[17] <= 1.7209073305130005) {
                            memcpy(var60, (double[]){0.0, 0.0, 0.10009624639076033, 0.8508180943214629, 0.049085659287776695}, 5 * sizeof(double));
                        } else {
                            memcpy(var60, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[8] <= 9842.78564453125) {
                            memcpy(var60, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var60, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                }
            }
        }
    }
    add_vectors_standard(var11, var60, 5, var10);
    double var61[5];
    if (input[11] <= 480.5703582763672) {
        if (input[15] <= 3.9679774045944214) {
            if (input[0] <= 22.5) {
                if (input[9] <= 1506.5750122070312) {
                    if (input[14] <= 15.204157829284668) {
                        if (input[17] <= 1.1774582266807556) {
                            memcpy(var61, (double[]){0.0, 0.0, 0.6709677419354839, 0.0, 0.32903225806451614}, 5 * sizeof(double));
                        } else {
                            memcpy(var61, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[14] <= 85.8154067993164) {
                            memcpy(var61, (double[]){0.12267657992565055, 0.8773234200743495, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var61, (double[]){0.5354330708661418, 0.0, 0.4645669291338583, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                } else {
                    if (input[6] <= 202.5) {
                        memcpy(var61, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var61, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                }
            } else {
                if (input[9] <= 940.0499877929688) {
                    if (input[14] <= 28.786542892456055) {
                        if (input[9] <= 478.6999969482422) {
                            memcpy(var61, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var61, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        memcpy(var61, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                } else {
                    memcpy(var61, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                }
            }
        } else {
            if (input[10] <= 1700.5048828125) {
                if (input[9] <= 40.68333435058594) {
                    if (input[8] <= 9647.9287109375) {
                        if (input[8] <= 8980.125) {
                            memcpy(var61, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var61, (double[]){0.0, 0.0, 0.0, 0.8201892744479495, 0.17981072555205047}, 5 * sizeof(double));
                        }
                    } else {
                        memcpy(var61, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    }
                } else {
                    if (input[0] <= 16.5) {
                        if (input[8] <= 11330.25) {
                            memcpy(var61, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var61, (double[]){0.0, 0.0, 0.0, 0.5777777777777778, 0.4222222222222222}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[9] <= 125.39285659790039) {
                            memcpy(var61, (double[]){0.0, 0.0, 0.0, 0.9193776520509194, 0.08062234794908062}, 5 * sizeof(double));
                        } else {
                            memcpy(var61, (double[]){0.0, 0.0, 0.5277777777777778, 0.47222222222222227, 0.0}, 5 * sizeof(double));
                        }
                    }
                }
            } else {
                if (input[8] <= 9488.25) {
                    if (input[1] <= 7.0) {
                        memcpy(var61, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var61, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    }
                } else {
                    if (input[13] <= 4418.633544921875) {
                        if (input[4] <= 486.2053527832031) {
                            memcpy(var61, (double[]){0.0, 0.0, 0.3573883161512028, 0.0, 0.6426116838487973}, 5 * sizeof(double));
                        } else {
                            memcpy(var61, (double[]){0.0, 0.0, 0.0, 0.9054726368159204, 0.0945273631840796}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[8] <= 9750.5830078125) {
                            memcpy(var61, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var61, (double[]){0.0, 0.0, 0.0, 0.23318385650224216, 0.7668161434977578}, 5 * sizeof(double));
                        }
                    }
                }
            }
        }
    } else {
        if (input[3] <= 2268.4666748046875) {
            if (input[8] <= 8812.10400390625) {
                memcpy(var61, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
            } else {
                memcpy(var61, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
            }
        } else {
            if (input[8] <= 7501.357177734375) {
                memcpy(var61, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
            } else {
                if (input[9] <= 701.2152709960938) {
                    memcpy(var61, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                } else {
                    memcpy(var61, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                }
            }
        }
    }
    add_vectors_standard(var10, var61, 5, var9);
    double var62[5];
    if (input[13] <= 991.567138671875) {
        if (input[17] <= 1.3467410802841187) {
            if (input[1] <= 2.5) {
                memcpy(var62, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
            } else {
                memcpy(var62, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
            }
        } else {
            if (input[9] <= 907.0486145019531) {
                if (input[15] <= 3.7492467164993286) {
                    if (input[0] <= 22.5) {
                        if (input[9] <= 811.6428527832031) {
                            memcpy(var62, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var62, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[12] <= 293.8653106689453) {
                            memcpy(var62, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var62, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                } else {
                    if (input[16] <= 1.3609765768051147) {
                        memcpy(var62, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    } else {
                        if (input[9] <= 58.79166603088379) {
                            memcpy(var62, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var62, (double[]){0.0, 0.0, 0.0, 0.87248322147651, 0.12751677852348992}, 5 * sizeof(double));
                        }
                    }
                }
            } else {
                if (input[15] <= 1.1557065844535828) {
                    memcpy(var62, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                } else {
                    if (input[14] <= 135.57137298583984) {
                        if (input[15] <= 1.3363964557647705) {
                            memcpy(var62, (double[]){0.3586956521739131, 0.6413043478260869, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var62, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        memcpy(var62, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                }
            }
        }
    } else {
        if (input[9] <= 127.48571014404297) {
            if (input[8] <= 9663.5) {
                if (input[9] <= 54.58333396911621) {
                    if (input[5] <= 2260.375) {
                        if (input[7] <= 2027.9000244140625) {
                            memcpy(var62, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var62, (double[]){0.0, 0.0, 0.0, 0.931899641577061, 0.06810035842293906}, 5 * sizeof(double));
                        }
                    } else {
                        memcpy(var62, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    }
                } else {
                    if (input[5] <= 1378.0833129882812) {
                        memcpy(var62, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var62, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                    }
                }
            } else {
                if (input[0] <= 31.5) {
                    if (input[17] <= 1.4969273209571838) {
                        if (input[9] <= 48.738094329833984) {
                            memcpy(var62, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var62, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[5] <= 3739.107177734375) {
                            memcpy(var62, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var62, (double[]){0.0, 0.0, 0.3376623376623377, 0.0, 0.6623376623376623}, 5 * sizeof(double));
                        }
                    }
                } else {
                    if (input[12] <= 1896.5604248046875) {
                        if (input[2] <= 3210.375) {
                            memcpy(var62, (double[]){0.0, 0.0, 0.0, 0.7323943661971831, 0.2676056338028169}, 5 * sizeof(double));
                        } else {
                            memcpy(var62, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[13] <= 4150.045654296875) {
                            memcpy(var62, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var62, (double[]){0.0, 0.0, 0.0, 0.931899641577061, 0.06810035842293906}, 5 * sizeof(double));
                        }
                    }
                }
            }
        } else {
            if (input[7] <= 3790.511962890625) {
                if (input[13] <= 2488.65087890625) {
                    if (input[11] <= 267.0018382072449) {
                        memcpy(var62, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var62, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                } else {
                    if (input[3] <= 2139.0208740234375) {
                        if (input[0] <= 31.5) {
                            memcpy(var62, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var62, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        memcpy(var62, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                }
            } else {
                if (input[15] <= 6.85018253326416) {
                    if (input[4] <= 673.3809509277344) {
                        memcpy(var62, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var62, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                } else {
                    memcpy(var62, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                }
            }
        }
    }
    add_vectors_standard(var9, var62, 5, var8);
    double var63[5];
    if (input[15] <= 1.7974448204040527) {
        if (input[8] <= 8255.46630859375) {
            if (input[6] <= 94.0) {
                if (input[0] <= 9.5) {
                    memcpy(var63, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                } else {
                    memcpy(var63, (double[]){0.0, 0.0, 0.9017341040462428, 0.0, 0.09826589595375722}, 5 * sizeof(double));
                }
            } else {
                if (input[0] <= 28.5) {
                    memcpy(var63, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                } else {
                    if (input[5] <= 2418.4285888671875) {
                        if (input[16] <= 1.327509582042694) {
                            memcpy(var63, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var63, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        memcpy(var63, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                }
            }
        } else {
            if (input[14] <= 15.439496040344238) {
                memcpy(var63, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
            } else {
                if (input[5] <= 1082.4857177734375) {
                    if (input[2] <= 1630.6428833007812) {
                        if (input[1] <= 4.5) {
                            memcpy(var63, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var63, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        memcpy(var63, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                } else {
                    if (input[16] <= 1.3677310347557068) {
                        memcpy(var63, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    } else {
                        if (input[2] <= 1827.6875) {
                            memcpy(var63, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var63, (double[]){0.21854304635761587, 0.7814569536423841, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                }
            }
        }
    } else {
        if (input[9] <= 281.8928527832031) {
            if (input[11] <= 230.94265747070312) {
                if (input[9] <= 40.17142868041992) {
                    if (input[17] <= 1.7002054452896118) {
                        memcpy(var63, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    } else {
                        if (input[2] <= 2575.9000244140625) {
                            memcpy(var63, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var63, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        }
                    }
                } else {
                    if (input[2] <= 1261.25) {
                        if (input[6] <= 86.66666793823242) {
                            memcpy(var63, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var63, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[17] <= 1.5433382391929626) {
                            memcpy(var63, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var63, (double[]){0.0, 0.0, 0.0, 0.756201881950385, 0.24379811804961504}, 5 * sizeof(double));
                        }
                    }
                }
            } else {
                if (input[6] <= 225.73333740234375) {
                    memcpy(var63, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                } else {
                    if (input[14] <= 27.220873832702637) {
                        if (input[10] <= 1386.3746948242188) {
                            memcpy(var63, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var63, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[16] <= 1.3693723678588867) {
                            memcpy(var63, (double[]){0.0, 0.0, 0.3845854418061503, 0.5735046062021539, 0.04190995199169586}, 5 * sizeof(double));
                        } else {
                            memcpy(var63, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                }
            }
        } else {
            if (input[11] <= 70.51800155639648) {
                memcpy(var63, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
            } else {
                if (input[1] <= 5.5) {
                    if (input[2] <= 1747.0) {
                        memcpy(var63, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var63, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                    }
                } else {
                    memcpy(var63, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                }
            }
        }
    }
    add_vectors_standard(var8, var63, 5, var7);
    double var64[5];
    if (input[14] <= 28.955219268798828) {
        if (input[12] <= 1324.1742553710938) {
            if (input[4] <= 426.3809509277344) {
                if (input[16] <= 1.4085264205932617) {
                    if (input[13] <= 110.66302108764648) {
                        if (input[9] <= 47.625) {
                            memcpy(var64, (double[]){0.0, 0.0, 0.5160616348916166, 0.2308696787673022, 0.2530686863410812}, 5 * sizeof(double));
                        } else {
                            memcpy(var64, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        memcpy(var64, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    }
                } else {
                    if (input[3] <= 1043.0) {
                        memcpy(var64, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    } else {
                        if (input[17] <= 1.7948954701423645) {
                            memcpy(var64, (double[]){0.0, 0.0, 0.0, 0.9093799682034976, 0.09062003179650238}, 5 * sizeof(double));
                        } else {
                            memcpy(var64, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        }
                    }
                }
            } else {
                if (input[9] <= 928.4166564941406) {
                    if (input[15] <= 2.6067150831222534) {
                        memcpy(var64, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                    } else {
                        if (input[11] <= 80.58058547973633) {
                            memcpy(var64, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var64, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                } else {
                    if (input[11] <= 365.77516174316406) {
                        memcpy(var64, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var64, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                }
            }
        } else {
            if (input[15] <= 9.103895664215088) {
                if (input[16] <= 1.4603042006492615) {
                    if (input[16] <= 1.3650984168052673) {
                        if (input[5] <= 4631.446533203125) {
                            memcpy(var64, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var64, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        }
                    } else {
                        memcpy(var64, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    }
                } else {
                    memcpy(var64, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                }
            } else {
                memcpy(var64, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
            }
        }
    } else {
        if (input[15] <= 1.2543537616729736) {
            if (input[15] <= 1.2071012258529663) {
                memcpy(var64, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
            } else {
                if (input[4] <= 1805.7857055664062) {
                    memcpy(var64, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                } else {
                    memcpy(var64, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                }
            }
        } else {
            if (input[12] <= 1422.2229614257812) {
                if (input[11] <= 591.26025390625) {
                    if (input[15] <= 2.1332982778549194) {
                        if (input[2] <= 1491.0) {
                            memcpy(var64, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var64, (double[]){0.04474397830594991, 0.7999680969851652, 0.15528792470888497, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[9] <= 161.5) {
                            memcpy(var64, (double[]){0.0, 0.0, 0.0, 0.7849056603773585, 0.2150943396226415}, 5 * sizeof(double));
                        } else {
                            memcpy(var64, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                } else {
                    memcpy(var64, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                }
            } else {
                if (input[11] <= 134.87217712402344) {
                    memcpy(var64, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                } else {
                    if (input[16] <= 1.3693723678588867) {
                        if (input[14] <= 62.808481216430664) {
                            memcpy(var64, (double[]){0.0, 0.0, 0.2593175853018373, 0.2320209973753281, 0.5086614173228347}, 5 * sizeof(double));
                        } else {
                            memcpy(var64, (double[]){0.0, 0.0, 0.21839080459770116, 0.7816091954022989, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[6] <= 222.9404754638672) {
                            memcpy(var64, (double[]){0.0, 0.0, 0.3584905660377359, 0.6415094339622642, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var64, (double[]){0.0, 0.0, 0.0, 0.9745649263721553, 0.02543507362784471}, 5 * sizeof(double));
                        }
                    }
                }
            }
        }
    }
    add_vectors_standard(var7, var64, 5, var6);
    double var65[5];
    if (input[8] <= 8255.46630859375) {
        if (input[4] <= 267.5) {
            if (input[11] <= 152.04823684692383) {
                if (input[8] <= 7415.966796875) {
                    if (input[0] <= 9.5) {
                        if (input[1] <= 1.5) {
                            memcpy(var65, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var65, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[13] <= 3217.748046875) {
                            memcpy(var65, (double[]){0.0, 0.0, 0.6046511627906977, 0.0, 0.39534883720930236}, 5 * sizeof(double));
                        } else {
                            memcpy(var65, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        }
                    }
                } else {
                    memcpy(var65, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                }
            } else {
                memcpy(var65, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
            }
        } else {
            if (input[15] <= 1.2015577554702759) {
                memcpy(var65, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
            } else {
                if (input[4] <= 1805.7857055664062) {
                    if (input[9] <= 1176.2000122070312) {
                        memcpy(var65, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var65, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                } else {
                    memcpy(var65, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                }
            }
        }
    } else {
        if (input[15] <= 3.9317632913589478) {
            if (input[15] <= 1.773399293422699) {
                if (input[4] <= 787.4761962890625) {
                    if (input[2] <= 1755.0429077148438) {
                        memcpy(var65, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var65, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                } else {
                    memcpy(var65, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                }
            } else {
                if (input[2] <= 1851.8095703125) {
                    if (input[16] <= 1.2992175817489624) {
                        memcpy(var65, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var65, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                } else {
                    if (input[8] <= 11186.21630859375) {
                        memcpy(var65, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var65, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                    }
                }
            }
        } else {
            if (input[15] <= 8.389950275421143) {
                if (input[0] <= 22.5) {
                    if (input[9] <= 236.1666717529297) {
                        memcpy(var65, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var65, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                    }
                } else {
                    if (input[12] <= 1719.6854858398438) {
                        if (input[10] <= 712.5558166503906) {
                            memcpy(var65, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var65, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[14] <= 65.23699951171875) {
                            memcpy(var65, (double[]){0.0, 0.0, 0.0, 0.87248322147651, 0.12751677852348992}, 5 * sizeof(double));
                        } else {
                            memcpy(var65, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                }
            } else {
                if (input[10] <= 1749.677734375) {
                    if (input[13] <= 4391.5888671875) {
                        if (input[10] <= 208.8608627319336) {
                            memcpy(var65, (double[]){0.0, 0.0, 0.0, 0.7323943661971831, 0.2676056338028169}, 5 * sizeof(double));
                        } else {
                            memcpy(var65, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[13] <= 5342.78857421875) {
                            memcpy(var65, (double[]){0.0, 0.0, 0.0, 0.7323943661971831, 0.2676056338028169}, 5 * sizeof(double));
                        } else {
                            memcpy(var65, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        }
                    }
                } else {
                    if (input[9] <= 24.93333339691162) {
                        memcpy(var65, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var65, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    }
                }
            }
        }
    }
    add_vectors_standard(var6, var65, 5, var5);
    double var66[5];
    if (input[15] <= 3.76836359500885) {
        if (input[9] <= 898.2999877929688) {
            if (input[1] <= 1.5) {
                memcpy(var66, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
            } else {
                if (input[0] <= 9.5) {
                    memcpy(var66, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                } else {
                    if (input[7] <= 3631.3194580078125) {
                        if (input[0] <= 19.5) {
                            memcpy(var66, (double[]){0.0, 0.11409395973154363, 0.8859060402684563, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var66, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[13] <= 2116.345001220703) {
                            memcpy(var66, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var66, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                }
            }
        } else {
            if (input[15] <= 1.2516557574272156) {
                if (input[4] <= 540.0) {
                    if (input[3] <= 716.125) {
                        memcpy(var66, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var66, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                } else {
                    if (input[16] <= 1.230243444442749) {
                        memcpy(var66, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var66, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                }
            } else {
                if (input[5] <= 786.1999816894531) {
                    memcpy(var66, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                } else {
                    if (input[15] <= 1.2622830867767334) {
                        if (input[5] <= 1228.6889038085938) {
                            memcpy(var66, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var66, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[13] <= 619.7030334472656) {
                            memcpy(var66, (double[]){0.017722878625134265, 0.9822771213748658, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var66, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                }
            }
        }
    } else {
        if (input[4] <= 557.0625) {
            if (input[8] <= 9557.2998046875) {
                if (input[8] <= 7859.08349609375) {
                    memcpy(var66, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                } else {
                    if (input[15] <= 5.751742601394653) {
                        memcpy(var66, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                    } else {
                        if (input[0] <= 31.5) {
                            memcpy(var66, (double[]){0.0, 0.0, 0.0, 0.8338192419825073, 0.1661807580174927}, 5 * sizeof(double));
                        } else {
                            memcpy(var66, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        }
                    }
                }
            } else {
                if (input[8] <= 9956.28564453125) {
                    if (input[9] <= 106.66666793823242) {
                        memcpy(var66, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var66, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                } else {
                    if (input[17] <= 1.6175508499145508) {
                        if (input[16] <= 1.3908387422561646) {
                            memcpy(var66, (double[]){0.0, 0.0, 0.10018251875887245, 0.17927398093692964, 0.7205435003041979}, 5 * sizeof(double));
                        } else {
                            memcpy(var66, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[6] <= 282.9375) {
                            memcpy(var66, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var66, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                }
            }
        } else {
            if (input[7] <= 4308.28564453125) {
                memcpy(var66, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
            } else {
                if (input[9] <= 153.42857360839844) {
                    if (input[8] <= 9866.3212890625) {
                        memcpy(var66, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var66, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    }
                } else {
                    memcpy(var66, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                }
            }
        }
    }
    add_vectors_standard(var5, var66, 5, var4);
    double var67[5];
    if (input[15] <= 1.2516557574272156) {
        if (input[10] <= 33.941123962402344) {
            if (input[1] <= 1.5) {
                memcpy(var67, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
            } else {
                if (input[0] <= 9.5) {
                    memcpy(var67, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                } else {
                    memcpy(var67, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                }
            }
        } else {
            if (input[2] <= 3226.455322265625) {
                memcpy(var67, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
            } else {
                if (input[13] <= 411.56353759765625) {
                    memcpy(var67, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                } else {
                    memcpy(var67, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                }
            }
        }
    } else {
        if (input[7] <= 3860.9791259765625) {
            if (input[16] <= 1.362529456615448) {
                if (input[4] <= 493.9642791748047) {
                    if (input[9] <= 60.41666603088379) {
                        memcpy(var67, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    } else {
                        if (input[12] <= 1347.6790771484375) {
                            memcpy(var67, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var67, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                } else {
                    if (input[8] <= 9698.0) {
                        if (input[13] <= 619.7030334472656) {
                            memcpy(var67, (double[]){0.0, 0.95139911634757, 0.04860088365243004, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var67, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[16] <= 1.2926440238952637) {
                            memcpy(var67, (double[]){0.0, 0.5074626865671642, 0.4925373134328358, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var67, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                }
            } else {
                if (input[15] <= 5.0546228885650635) {
                    if (input[2] <= 3505.6875) {
                        if (input[9] <= 895.2999877929688) {
                            memcpy(var67, (double[]){0.0, 0.0, 0.951563064328424, 0.03547493880171755, 0.012961996869858335}, 5 * sizeof(double));
                        } else {
                            memcpy(var67, (double[]){0.0, 0.6732673267326733, 0.32673267326732675, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        memcpy(var67, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                    }
                } else {
                    if (input[2] <= 1583.1500244140625) {
                        memcpy(var67, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    } else {
                        if (input[16] <= 1.427508294582367) {
                            memcpy(var67, (double[]){0.0, 0.0, 0.0, 0.6529680365296804, 0.34703196347031967}, 5 * sizeof(double));
                        } else {
                            memcpy(var67, (double[]){0.0, 0.0, 0.0, 0.11063829787234043, 0.8893617021276595}, 5 * sizeof(double));
                        }
                    }
                }
            }
        } else {
            if (input[14] <= 47.08053970336914) {
                if (input[7] <= 4020.6666259765625) {
                    if (input[2] <= 3391.40478515625) {
                        memcpy(var67, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    } else {
                        if (input[11] <= 171.97400665283203) {
                            memcpy(var67, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var67, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                } else {
                    if (input[5] <= 4342.071533203125) {
                        memcpy(var67, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    } else {
                        if (input[5] <= 4403.48828125) {
                            memcpy(var67, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var67, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        }
                    }
                }
            } else {
                if (input[16] <= 1.374217450618744) {
                    memcpy(var67, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                } else {
                    memcpy(var67, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                }
            }
        }
    }
    add_vectors_standard(var4, var67, 5, var3);
    double var68[5];
    if (input[6] <= 400.0) {
        if (input[0] <= 19.5) {
            if (input[4] <= 422.5333251953125) {
                if (input[17] <= 1.6201257109642029) {
                    if (input[1] <= 1.5) {
                        memcpy(var68, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    } else {
                        if (input[14] <= 49.2359676361084) {
                            memcpy(var68, (double[]){0.0, 0.0, 0.0, 0.8914285714285715, 0.10857142857142857}, 5 * sizeof(double));
                        } else {
                            memcpy(var68, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                } else {
                    memcpy(var68, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                }
            } else {
                if (input[11] <= 207.81864166259766) {
                    if (input[8] <= 10349.98583984375) {
                        if (input[2] <= 1446.4000244140625) {
                            memcpy(var68, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var68, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        memcpy(var68, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                } else {
                    if (input[11] <= 270.4981231689453) {
                        memcpy(var68, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var68, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                }
            }
        } else {
            if (input[4] <= 438.452392578125) {
                if (input[17] <= 1.5420124530792236) {
                    memcpy(var68, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                } else {
                    if (input[9] <= 43.70833396911621) {
                        if (input[2] <= 2262.175048828125) {
                            memcpy(var68, (double[]){0.0, 0.0, 0.0, 0.47706422018348627, 0.5229357798165137}, 5 * sizeof(double));
                        } else {
                            memcpy(var68, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[2] <= 2236.3333740234375) {
                            memcpy(var68, (double[]){0.0, 0.0, 0.5984251968503937, 0.4015748031496063, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var68, (double[]){0.0, 0.0, 0.0, 0.8975188781014023, 0.10248112189859761}, 5 * sizeof(double));
                        }
                    }
                }
            } else {
                if (input[5] <= 3782.6429443359375) {
                    if (input[2] <= 3397.5) {
                        memcpy(var68, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                    } else {
                        if (input[2] <= 3448.1785888671875) {
                            memcpy(var68, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var68, (double[]){0.0, 0.0, 0.8172043010752689, 0.18279569892473121, 0.0}, 5 * sizeof(double));
                        }
                    }
                } else {
                    if (input[9] <= 75.67856979370117) {
                        memcpy(var68, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var68, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                    }
                }
            }
        }
    } else {
        if (input[8] <= 8423.8193359375) {
            if (input[15] <= 1.2071012258529663) {
                memcpy(var68, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
            } else {
                if (input[11] <= 761.3520202636719) {
                    memcpy(var68, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                } else {
                    memcpy(var68, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                }
            }
        } else {
            if (input[5] <= 2989.452392578125) {
                if (input[15] <= 1.9615249037742615) {
                    if (input[13] <= 231.87276458740234) {
                        if (input[13] <= 222.29776763916016) {
                            memcpy(var68, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var68, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        memcpy(var68, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                } else {
                    if (input[6] <= 499.25) {
                        memcpy(var68, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var68, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                    }
                }
            } else {
                if (input[2] <= 4072.420654296875) {
                    memcpy(var68, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                } else {
                    if (input[14] <= 31.2089262008667) {
                        memcpy(var68, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var68, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    }
                }
            }
        }
    }
    add_vectors_standard(var3, var68, 5, var2);
    double var69[5];
    if (input[8] <= 8255.46630859375) {
        if (input[13] <= 3766.570068359375) {
            if (input[11] <= 18.03122329711914) {
                if (input[0] <= 9.5) {
                    if (input[0] <= 7.5) {
                        memcpy(var69, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    } else {
                        memcpy(var69, (double[]){0.0, 0.0, 0.0, 0.5777777777777778, 0.4222222222222222}, 5 * sizeof(double));
                    }
                } else {
                    memcpy(var69, (double[]){0.0, 0.0, 0.753623188405797, 0.0, 0.24637681159420285}, 5 * sizeof(double));
                }
            } else {
                if (input[7] <= 3311.2589111328125) {
                    memcpy(var69, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                } else {
                    if (input[3] <= 2529.5555419921875) {
                        memcpy(var69, (double[]){0.0, 1.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                    } else {
                        if (input[2] <= 3339.3194580078125) {
                            memcpy(var69, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var69, (double[]){1.0, 0.0, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                }
            }
        } else {
            if (input[15] <= 9.515350341796875) {
                if (input[0] <= 12.5) {
                    memcpy(var69, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                } else {
                    memcpy(var69, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                }
            } else {
                memcpy(var69, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
            }
        }
    } else {
        if (input[15] <= 4.00251305103302) {
            if (input[15] <= 2.1284735202789307) {
                if (input[16] <= 1.2625237703323364) {
                    memcpy(var69, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                } else {
                    if (input[14] <= 18.22756052017212) {
                        memcpy(var69, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                    } else {
                        if (input[8] <= 9809.111328125) {
                            memcpy(var69, (double[]){0.04638088545326774, 0.9536191145467323, 0.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var69, (double[]){0.0, 0.6732673267326733, 0.32673267326732675, 0.0, 0.0}, 5 * sizeof(double));
                        }
                    }
                }
            } else {
                if (input[8] <= 9326.85693359375) {
                    memcpy(var69, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                } else {
                    memcpy(var69, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                }
            }
        } else {
            if (input[15] <= 8.454740047454834) {
                if (input[8] <= 9969.10693359375) {
                    if (input[4] <= 486.31666564941406) {
                        if (input[16] <= 1.3918465971946716) {
                            memcpy(var69, (double[]){0.0, 0.0, 1.0, 0.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var69, (double[]){0.0, 0.0, 0.6909090909090909, 0.3090909090909091, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[12] <= 2029.6351318359375) {
                            memcpy(var69, (double[]){0.0, 0.0, 0.15702479338842976, 0.8429752066115702, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var69, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        }
                    }
                } else {
                    if (input[7] <= 2068.5833129882812) {
                        memcpy(var69, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                    } else {
                        if (input[2] <= 1898.0833129882812) {
                            memcpy(var69, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var69, (double[]){0.0, 0.0, 0.0, 0.8914285714285713, 0.10857142857142854}, 5 * sizeof(double));
                        }
                    }
                }
            } else {
                if (input[14] <= 27.702656745910645) {
                    if (input[12] <= 673.9879760742188) {
                        if (input[4] <= 156.375) {
                            memcpy(var69, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var69, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[9] <= 27.766667366027832) {
                            memcpy(var69, (double[]){0.0, 0.0, 0.0, 0.21487603305785125, 0.7851239669421487}, 5 * sizeof(double));
                        } else {
                            memcpy(var69, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        }
                    }
                } else {
                    if (input[16] <= 1.3866488933563232) {
                        if (input[17] <= 1.4914673566818237) {
                            memcpy(var69, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var69, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        }
                    } else {
                        if (input[8] <= 10554.0595703125) {
                            memcpy(var69, (double[]){0.0, 0.0, 0.0, 1.0, 0.0}, 5 * sizeof(double));
                        } else {
                            memcpy(var69, (double[]){0.0, 0.0, 0.0, 0.0, 1.0}, 5 * sizeof(double));
                        }
                    }
                }
            }
        }
    }
    add_vectors_standard(var2, var69, 5, var1);
    mul_vector_number_standard(var1, 0.02857142857142857, 5, var0);
    memcpy(output, var0, 5 * sizeof(double));
}


// --- CODICE GENERATO AUTOMATICAMENTE DA M2CGEN - MODELLO LOTTO CHERRY (3 classi) ---
// input:  Array di 18 double con i valori letti dal sensore ottico
// output: Array di 3 double, punteggio per ciascuna classe cherry
#include <string.h>
void add_vectors_cherry(double *v1, double *v2, int size, double *result) {
    for(int i = 0; i < size; ++i)
        result[i] = v1[i] + v2[i];
}
void mul_vector_number_cherry(double *v1, double num, int size, double *result) {
    for(int i = 0; i < size; ++i)
        result[i] = v1[i] * num;
}
void score_cherry(double * input, double * output) {
    double var0[3];
    double var1[3];
    double var2[3];
    double var3[3];
    double var4[3];
    double var5[3];
    double var6[3];
    double var7[3];
    double var8[3];
    double var9[3];
    double var10[3];
    double var11[3];
    double var12[3];
    double var13[3];
    double var14[3];
    double var15[3];
    double var16[3];
    double var17[3];
    double var18[3];
    double var19[3];
    double var20[3];
    double var21[3];
    double var22[3];
    double var23[3];
    double var24[3];
    double var25[3];
    double var26[3];
    double var27[3];
    double var28[3];
    double var29[3];
    double var30[3];
    double var31[3];
    double var32[3];
    double var33[3];
    double var34[3];
    double var35[3];
    if (input[4] <= 254.1666717529297) {
        if (input[14] <= 72.0456600189209) {
            memcpy(var35, (double[]){0.0, 1.0, 0.0}, 3 * sizeof(double));
        } else {
            memcpy(var35, (double[]){0.0, 0.0, 1.0}, 3 * sizeof(double));
        }
    } else {
        memcpy(var35, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
    }
    double var36[3];
    if (input[9] <= 333.75) {
        memcpy(var36, (double[]){0.0, 1.0, 0.0}, 3 * sizeof(double));
    } else {
        if (input[4] <= 260.8000030517578) {
            if (input[8] <= 5808.625) {
                memcpy(var36, (double[]){0.0, 0.0, 1.0}, 3 * sizeof(double));
            } else {
                memcpy(var36, (double[]){0.0, 0.0, 1.0}, 3 * sizeof(double));
            }
        } else {
            if (input[8] <= 10392.02490234375) {
                memcpy(var36, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
            } else {
                memcpy(var36, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
            }
        }
    }
    add_vectors_cherry(var35, var36, 3, var34);
    double var37[3];
    if (input[17] <= 1.3444982767105103) {
        memcpy(var37, (double[]){0.0, 0.0, 1.0}, 3 * sizeof(double));
    } else {
        if (input[15] <= 3.1869221925735474) {
            memcpy(var37, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
        } else {
            memcpy(var37, (double[]){0.0, 1.0, 0.0}, 3 * sizeof(double));
        }
    }
    add_vectors_cherry(var34, var37, 3, var33);
    double var38[3];
    if (input[14] <= 63.57773399353027) {
        if (input[4] <= 254.1666717529297) {
            memcpy(var38, (double[]){0.0, 1.0, 0.0}, 3 * sizeof(double));
        } else {
            memcpy(var38, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
        }
    } else {
        memcpy(var38, (double[]){0.0, 0.0, 1.0}, 3 * sizeof(double));
    }
    add_vectors_cherry(var33, var38, 3, var32);
    double var39[3];
    if (input[17] <= 1.3422484993934631) {
        memcpy(var39, (double[]){0.0, 0.0, 1.0}, 3 * sizeof(double));
    } else {
        if (input[16] <= 1.360450029373169) {
            if (input[14] <= 12.361594200134277) {
                memcpy(var39, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
            } else {
                if (input[15] <= 2.904057502746582) {
                    memcpy(var39, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
                } else {
                    memcpy(var39, (double[]){0.0, 1.0, 0.0}, 3 * sizeof(double));
                }
            }
        } else {
            if (input[10] <= 423.81640625) {
                memcpy(var39, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
            } else {
                memcpy(var39, (double[]){0.0, 1.0, 0.0}, 3 * sizeof(double));
            }
        }
    }
    add_vectors_cherry(var32, var39, 3, var31);
    double var40[3];
    if (input[15] <= 2.047934830188751) {
        memcpy(var40, (double[]){0.0, 0.0, 1.0}, 3 * sizeof(double));
    } else {
        if (input[4] <= 229.6666717529297) {
            memcpy(var40, (double[]){0.0, 1.0, 0.0}, 3 * sizeof(double));
        } else {
            memcpy(var40, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
        }
    }
    add_vectors_cherry(var31, var40, 3, var30);
    double var41[3];
    if (input[4] <= 294.375) {
        if (input[5] <= 469.3500061035156) {
            memcpy(var41, (double[]){0.0, 0.0, 1.0}, 3 * sizeof(double));
        } else {
            memcpy(var41, (double[]){0.0, 1.0, 0.0}, 3 * sizeof(double));
        }
    } else {
        if (input[15] <= 2.332601547241211) {
            memcpy(var41, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
        } else {
            memcpy(var41, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
        }
    }
    add_vectors_cherry(var30, var41, 3, var29);
    double var42[3];
    if (input[14] <= 72.0456600189209) {
        if (input[0] <= 10.5) {
            if (input[11] <= 23.087151288986206) {
                memcpy(var42, (double[]){0.0, 1.0, 0.0}, 3 * sizeof(double));
            } else {
                memcpy(var42, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
            }
        } else {
            memcpy(var42, (double[]){0.0, 1.0, 0.0}, 3 * sizeof(double));
        }
    } else {
        memcpy(var42, (double[]){0.0, 0.0, 1.0}, 3 * sizeof(double));
    }
    add_vectors_cherry(var29, var42, 3, var28);
    double var43[3];
    if (input[5] <= 511.5500030517578) {
        memcpy(var43, (double[]){0.0, 0.0, 1.0}, 3 * sizeof(double));
    } else {
        if (input[5] <= 918.8999938964844) {
            if (input[4] <= 229.6666717529297) {
                memcpy(var43, (double[]){0.0, 1.0, 0.0}, 3 * sizeof(double));
            } else {
                memcpy(var43, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
            }
        } else {
            memcpy(var43, (double[]){0.0, 1.0, 0.0}, 3 * sizeof(double));
        }
    }
    add_vectors_cherry(var28, var43, 3, var27);
    double var44[3];
    if (input[15] <= 3.1869221925735474) {
        if (input[14] <= 99.25719451904297) {
            memcpy(var44, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
        } else {
            memcpy(var44, (double[]){0.0, 0.0, 1.0}, 3 * sizeof(double));
        }
    } else {
        memcpy(var44, (double[]){0.0, 1.0, 0.0}, 3 * sizeof(double));
    }
    add_vectors_cherry(var27, var44, 3, var26);
    double var45[3];
    if (input[8] <= 8662.0) {
        memcpy(var45, (double[]){0.0, 0.0, 1.0}, 3 * sizeof(double));
    } else {
        if (input[9] <= 406.25) {
            memcpy(var45, (double[]){0.0, 1.0, 0.0}, 3 * sizeof(double));
        } else {
            memcpy(var45, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
        }
    }
    add_vectors_cherry(var26, var45, 3, var25);
    double var46[3];
    if (input[11] <= 46.63205146789551) {
        if (input[5] <= 469.3500061035156) {
            memcpy(var46, (double[]){0.0, 0.0, 1.0}, 3 * sizeof(double));
        } else {
            if (input[11] <= 38.695695877075195) {
                memcpy(var46, (double[]){0.0, 1.0, 0.0}, 3 * sizeof(double));
            } else {
                memcpy(var46, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
            }
        }
    } else {
        memcpy(var46, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
    }
    add_vectors_cherry(var25, var46, 3, var24);
    double var47[3];
    if (input[4] <= 248.36666870117188) {
        if (input[15] <= 2.8211915493011475) {
            memcpy(var47, (double[]){0.0, 0.0, 1.0}, 3 * sizeof(double));
        } else {
            if (input[1] <= 2.5) {
                memcpy(var47, (double[]){0.0, 1.0, 0.0}, 3 * sizeof(double));
            } else {
                memcpy(var47, (double[]){0.0, 1.0, 0.0}, 3 * sizeof(double));
            }
        }
    } else {
        memcpy(var47, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
    }
    add_vectors_cherry(var24, var47, 3, var23);
    double var48[3];
    if (input[8] <= 8662.0) {
        memcpy(var48, (double[]){0.0, 0.0, 1.0}, 3 * sizeof(double));
    } else {
        if (input[4] <= 229.6666717529297) {
            memcpy(var48, (double[]){0.0, 1.0, 0.0}, 3 * sizeof(double));
        } else {
            memcpy(var48, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
        }
    }
    add_vectors_cherry(var23, var48, 3, var22);
    double var49[3];
    if (input[5] <= 523.3000030517578) {
        memcpy(var49, (double[]){0.0, 0.0, 1.0}, 3 * sizeof(double));
    } else {
        if (input[4] <= 254.1666717529297) {
            memcpy(var49, (double[]){0.0, 1.0, 0.0}, 3 * sizeof(double));
        } else {
            memcpy(var49, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
        }
    }
    add_vectors_cherry(var22, var49, 3, var21);
    double var50[3];
    if (input[9] <= 307.5) {
        memcpy(var50, (double[]){0.0, 1.0, 0.0}, 3 * sizeof(double));
    } else {
        if (input[15] <= 2.1303595304489136) {
            memcpy(var50, (double[]){0.0, 0.0, 1.0}, 3 * sizeof(double));
        } else {
            if (input[17] <= 1.4577987790107727) {
                memcpy(var50, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
            } else {
                memcpy(var50, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
            }
        }
    }
    add_vectors_cherry(var21, var50, 3, var20);
    double var51[3];
    if (input[8] <= 8967.66650390625) {
        memcpy(var51, (double[]){0.0, 0.0, 1.0}, 3 * sizeof(double));
    } else {
        if (input[4] <= 253.00000762939453) {
            memcpy(var51, (double[]){0.0, 1.0, 0.0}, 3 * sizeof(double));
        } else {
            if (input[11] <= 60.76272392272949) {
                memcpy(var51, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
            } else {
                memcpy(var51, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
            }
        }
    }
    add_vectors_cherry(var20, var51, 3, var19);
    double var52[3];
    if (input[5] <= 523.3000030517578) {
        memcpy(var52, (double[]){0.0, 0.0, 1.0}, 3 * sizeof(double));
    } else {
        if (input[9] <= 380.0) {
            memcpy(var52, (double[]){0.0, 1.0, 0.0}, 3 * sizeof(double));
        } else {
            memcpy(var52, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
        }
    }
    add_vectors_cherry(var19, var52, 3, var18);
    double var53[3];
    if (input[15] <= 2.047934830188751) {
        memcpy(var53, (double[]){0.0, 0.0, 1.0}, 3 * sizeof(double));
    } else {
        if (input[10] <= 136.84809494018555) {
            memcpy(var53, (double[]){0.0, 1.0, 0.0}, 3 * sizeof(double));
        } else {
            if (input[9] <= 276.5999984741211) {
                memcpy(var53, (double[]){0.0, 1.0, 0.0}, 3 * sizeof(double));
            } else {
                memcpy(var53, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
            }
        }
    }
    add_vectors_cherry(var18, var53, 3, var17);
    double var54[3];
    if (input[8] <= 8662.0) {
        memcpy(var54, (double[]){0.0, 0.0, 1.0}, 3 * sizeof(double));
    } else {
        if (input[4] <= 254.1666717529297) {
            memcpy(var54, (double[]){0.0, 1.0, 0.0}, 3 * sizeof(double));
        } else {
            if (input[3] <= 444.5) {
                memcpy(var54, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
            } else {
                memcpy(var54, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
            }
        }
    }
    add_vectors_cherry(var17, var54, 3, var16);
    double var55[3];
    if (input[15] <= 2.1303595304489136) {
        memcpy(var55, (double[]){0.0, 0.0, 1.0}, 3 * sizeof(double));
    } else {
        if (input[11] <= 38.695695877075195) {
            memcpy(var55, (double[]){0.0, 1.0, 0.0}, 3 * sizeof(double));
        } else {
            if (input[14] <= 14.622396469116211) {
                memcpy(var55, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
            } else {
                memcpy(var55, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
            }
        }
    }
    add_vectors_cherry(var16, var55, 3, var15);
    double var56[3];
    if (input[4] <= 258.0) {
        if (input[17] <= 1.3410751223564148) {
            memcpy(var56, (double[]){0.0, 0.0, 1.0}, 3 * sizeof(double));
        } else {
            if (input[2] <= 604.6666564941406) {
                memcpy(var56, (double[]){0.0, 1.0, 0.0}, 3 * sizeof(double));
            } else {
                memcpy(var56, (double[]){0.0, 1.0, 0.0}, 3 * sizeof(double));
            }
        }
    } else {
        memcpy(var56, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
    }
    add_vectors_cherry(var15, var56, 3, var14);
    double var57[3];
    if (input[5] <= 511.5500030517578) {
        memcpy(var57, (double[]){0.0, 0.0, 1.0}, 3 * sizeof(double));
    } else {
        if (input[8] <= 10257.77490234375) {
            if (input[11] <= 35.91100025177002) {
                memcpy(var57, (double[]){0.0, 1.0, 0.0}, 3 * sizeof(double));
            } else {
                memcpy(var57, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
            }
        } else {
            if (input[15] <= 4.277349352836609) {
                if (input[5] <= 857.8500061035156) {
                    memcpy(var57, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
                } else {
                    memcpy(var57, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
                }
            } else {
                memcpy(var57, (double[]){0.0, 1.0, 0.0}, 3 * sizeof(double));
            }
        }
    }
    add_vectors_cherry(var14, var57, 3, var13);
    double var58[3];
    if (input[4] <= 184.5) {
        memcpy(var58, (double[]){0.0, 1.0, 0.0}, 3 * sizeof(double));
    } else {
        if (input[9] <= 601.625) {
            memcpy(var58, (double[]){0.0, 0.0, 1.0}, 3 * sizeof(double));
        } else {
            if (input[9] <= 706.0) {
                memcpy(var58, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
            } else {
                if (input[5] <= 278.875) {
                    memcpy(var58, (double[]){0.0, 0.0, 1.0}, 3 * sizeof(double));
                } else {
                    memcpy(var58, (double[]){0.0, 0.0, 1.0}, 3 * sizeof(double));
                }
            }
        }
    }
    add_vectors_cherry(var13, var58, 3, var12);
    double var59[3];
    if (input[5] <= 469.3500061035156) {
        memcpy(var59, (double[]){0.0, 0.0, 1.0}, 3 * sizeof(double));
    } else {
        if (input[4] <= 254.1666717529297) {
            memcpy(var59, (double[]){0.0, 1.0, 0.0}, 3 * sizeof(double));
        } else {
            memcpy(var59, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
        }
    }
    add_vectors_cherry(var12, var59, 3, var11);
    double var60[3];
    if (input[17] <= 1.3410751223564148) {
        memcpy(var60, (double[]){0.0, 0.0, 1.0}, 3 * sizeof(double));
    } else {
        if (input[6] <= 140.0749969482422) {
            if (input[9] <= 418.75) {
                if (input[13] <= 1016.5158081054688) {
                    memcpy(var60, (double[]){0.0, 1.0, 0.0}, 3 * sizeof(double));
                } else {
                    memcpy(var60, (double[]){0.0, 1.0, 0.0}, 3 * sizeof(double));
                }
            } else {
                memcpy(var60, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
            }
        } else {
            if (input[3] <= 840.1000061035156) {
                memcpy(var60, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
            } else {
                memcpy(var60, (double[]){0.0, 1.0, 0.0}, 3 * sizeof(double));
            }
        }
    }
    add_vectors_cherry(var11, var60, 3, var10);
    double var61[3];
    if (input[11] <= 33.09632396697998) {
        if (input[17] <= 1.3422484993934631) {
            memcpy(var61, (double[]){0.0, 0.0, 1.0}, 3 * sizeof(double));
        } else {
            memcpy(var61, (double[]){0.0, 1.0, 0.0}, 3 * sizeof(double));
        }
    } else {
        if (input[8] <= 8662.0) {
            memcpy(var61, (double[]){0.0, 0.0, 1.0}, 3 * sizeof(double));
        } else {
            memcpy(var61, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
        }
    }
    add_vectors_cherry(var10, var61, 3, var9);
    double var62[3];
    if (input[14] <= 72.0456600189209) {
        if (input[6] <= 143.0999984741211) {
            if (input[4] <= 296.875) {
                memcpy(var62, (double[]){0.0, 1.0, 0.0}, 3 * sizeof(double));
            } else {
                memcpy(var62, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
            }
        } else {
            memcpy(var62, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
        }
    } else {
        if (input[16] <= 1.3508790731430054) {
            memcpy(var62, (double[]){0.0, 0.0, 1.0}, 3 * sizeof(double));
        } else {
            memcpy(var62, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
        }
    }
    add_vectors_cherry(var9, var62, 3, var8);
    double var63[3];
    if (input[15] <= 2.2125768661499023) {
        memcpy(var63, (double[]){0.0, 0.0, 1.0}, 3 * sizeof(double));
    } else {
        if (input[4] <= 253.00000762939453) {
            memcpy(var63, (double[]){0.0, 1.0, 0.0}, 3 * sizeof(double));
        } else {
            memcpy(var63, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
        }
    }
    add_vectors_cherry(var8, var63, 3, var7);
    double var64[3];
    if (input[14] <= 72.0456600189209) {
        if (input[5] <= 817.125) {
            if (input[11] <= 23.087151288986206) {
                memcpy(var64, (double[]){0.0, 1.0, 0.0}, 3 * sizeof(double));
            } else {
                memcpy(var64, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
            }
        } else {
            if (input[12] <= 392.93443298339844) {
                memcpy(var64, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
            } else {
                memcpy(var64, (double[]){0.0, 1.0, 0.0}, 3 * sizeof(double));
            }
        }
    } else {
        memcpy(var64, (double[]){0.0, 0.0, 1.0}, 3 * sizeof(double));
    }
    add_vectors_cherry(var7, var64, 3, var6);
    double var65[3];
    if (input[4] <= 254.1666717529297) {
        if (input[4] <= 182.875) {
            memcpy(var65, (double[]){0.0, 1.0, 0.0}, 3 * sizeof(double));
        } else {
            if (input[7] <= 1335.0) {
                if (input[13] <= 1160.8932495117188) {
                    memcpy(var65, (double[]){0.0, 0.0, 1.0}, 3 * sizeof(double));
                } else {
                    memcpy(var65, (double[]){0.0, 0.0, 1.0}, 3 * sizeof(double));
                }
            } else {
                memcpy(var65, (double[]){0.0, 1.0, 0.0}, 3 * sizeof(double));
            }
        }
    } else {
        memcpy(var65, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
    }
    add_vectors_cherry(var6, var65, 3, var5);
    double var66[3];
    if (input[15] <= 2.9059505462646484) {
        if (input[4] <= 248.36666870117188) {
            memcpy(var66, (double[]){0.0, 0.0, 1.0}, 3 * sizeof(double));
        } else {
            if (input[1] <= 3.5) {
                memcpy(var66, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
            } else {
                memcpy(var66, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
            }
        }
    } else {
        memcpy(var66, (double[]){0.0, 1.0, 0.0}, 3 * sizeof(double));
    }
    add_vectors_cherry(var5, var66, 3, var4);
    double var67[3];
    if (input[15] <= 2.210683822631836) {
        memcpy(var67, (double[]){0.0, 0.0, 1.0}, 3 * sizeof(double));
    } else {
        if (input[11] <= 46.339744567871094) {
            memcpy(var67, (double[]){0.0, 1.0, 0.0}, 3 * sizeof(double));
        } else {
            if (input[12] <= 284.98309326171875) {
                memcpy(var67, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
            } else {
                memcpy(var67, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
            }
        }
    }
    add_vectors_cherry(var4, var67, 3, var3);
    double var68[3];
    if (input[0] <= 10.5) {
        if (input[10] <= 135.4955177307129) {
            memcpy(var68, (double[]){0.0, 1.0, 0.0}, 3 * sizeof(double));
        } else {
            if (input[14] <= 45.05682945251465) {
                memcpy(var68, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
            } else {
                memcpy(var68, (double[]){0.0, 1.0, 0.0}, 3 * sizeof(double));
            }
        }
    } else {
        if (input[17] <= 1.3797371983528137) {
            memcpy(var68, (double[]){0.0, 0.0, 1.0}, 3 * sizeof(double));
        } else {
            if (input[16] <= 1.3827956914901733) {
                memcpy(var68, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
            } else {
                memcpy(var68, (double[]){0.0, 1.0, 0.0}, 3 * sizeof(double));
            }
        }
    }
    add_vectors_cherry(var3, var68, 3, var2);
    double var69[3];
    if (input[17] <= 1.357939064502716) {
        memcpy(var69, (double[]){0.0, 0.0, 1.0}, 3 * sizeof(double));
    } else {
        if (input[3] <= 845.5750122070312) {
            if (input[11] <= 30.731199979782104) {
                memcpy(var69, (double[]){0.0, 1.0, 0.0}, 3 * sizeof(double));
            } else {
                memcpy(var69, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
            }
        } else {
            memcpy(var69, (double[]){0.0, 1.0, 0.0}, 3 * sizeof(double));
        }
    }
    add_vectors_cherry(var2, var69, 3, var1);
    mul_vector_number_cherry(var1, 0.02857142857142857, 3, var0);
    memcpy(output, var0, 3 * sizeof(double));
}


#ifdef __cplusplus
}

// --- FUNZIONE HELPER C++ PER INFERENZA REAL-TIME ---

// Mappa: indice interno del modello -> ID di classe globale (0-7), come da
// tabella "CLASSI DI OUTPUT GLOBALI" sopra. Necessaria perche' m2cgen ordina
// le classi internamente e l'ordine puo' non coincidere con l'ID globale
// (es. nel modello cherry, l'indice 0 del suo output corrisponde alla
// classe globale 5, non alla classe 0).
static const int TOMATO_LABELS_STANDARD[5] = { 0, 1, 2, 3, 4 };
static const int TOMATO_LABELS_CHERRY[3]   = { 5, 6, 7 };

/**
 * @brief Esegue l'inferenza per il lotto indicato e restituisce l'ID di
 *        classe GLOBALE (0-7) vincente.
 * @param mode  Lotto dichiarato per questo turno/linea (BATCH_STANDARD o BATCH_CHERRY).
 * @param input Array con le 18 feature calcolate in streaming (senza is_cherry!).
 *              NOTA: non 'const' perche' le funzioni score_* generate da
 *              m2cgen richiedono un puntatore non-const (anche se non
 *              modificano il contenuto).
 * @return int ID GLOBALE della classe predetta (da 0 a 7).
 */
inline int predict_tomato_class(TomatoBatchMode mode, double* input) {
    if (mode == BATCH_STANDARD) {
        double class_scores[5] = {0.0};
        score_standard(input, class_scores);
        int best = 0;
        double max_score = class_scores[0];
        for (int i = 1; i < 5; ++i) {
            if (class_scores[i] > max_score) { max_score = class_scores[i]; best = i; }
        }
        return TOMATO_LABELS_STANDARD[best];
    } else {
        double class_scores[3] = {0.0};
        score_cherry(input, class_scores);
        int best = 0;
        double max_score = class_scores[0];
        for (int i = 1; i < 3; ++i) {
            if (class_scores[i] > max_score) { max_score = class_scores[i]; best = i; }
        }
        return TOMATO_LABELS_CHERRY[best];
    }
}
#endif

#endif // TOMATO_CLASSIFIER_H
