#ifndef TOMATO_CLASSIFIER_H
#define TOMATO_CLASSIFIER_H

#ifdef __cplusplus
extern "C" {
#endif

// --- DOCUMENTAZIONE SPECIFICA SENSORE ---
/**
 * @brief ORDINE DELLE FEATURE DA PASSARE ALL'ARRAY 'input':
 *   input[0] = is_cherry;
 *   input[1] = transit_len;
 *   input[2] = valid_slices;
 *   input[3] = IR1_mean;
 *   input[4] = IR2_mean;
 *   input[5] = Green_mean;
 *   input[6] = Red_mean;
 *   input[7] = Blue_mean;
 *   input[8] = IR3_mean;
 *   input[9] = SAT_mean;
 *   input[10] = HUE_mean;
 *   input[11] = IR1_std;
 *   input[12] = Green_std;
 *   input[13] = Red_std;
 *   input[14] = SAT_std;
 *   input[15] = HUE_std;
 *   input[16] = Red_Green_ratio;
 *   input[17] = IR1_IR2_ratio;
 *   input[18] = IR3_IR2_ratio;
 */

/**
 * @brief CLASSI DI OUTPUT (Indice del valore massimo nell'array 'output'):
 *   0 = Pomodoro Verde Standard        | 4 = Pomodoro Rosso Standard
 *   1 = Pomodoro Giallo-Verde          | 5 = Pomodorino Giallo (Cherry)
 *   2 = Pomodoro Arancio-Giallo        | 6 = Pomodorino Rosso (Cherry)
 *   3 = Pomodoro Rosso-Arancio         | 7 = Pomodorino Verde Scuro Sfumato
 */


// --- CODICE GENERATO AUTOMATICAMENTE DA M2CGEN (ALBERO DI DECISIONE) ---
// Questa funzione calcola il punteggio di voto per tutte le 8 classi.
// Parametri:
//   input:  Array di 19 double con i valori letti dal sensore ottico
//   output: Array di 8 double dove verrà salvato il punteggio di ogni classe
#include <string.h>
void add_vectors(double *v1, double *v2, int size, double *result) {
    for(int i = 0; i < size; ++i)
        result[i] = v1[i] + v2[i];
}
void mul_vector_number(double *v1, double num, int size, double *result) {
    for(int i = 0; i < size; ++i)
        result[i] = v1[i] * num;
}
void score(double * input, double * output) {
    double var0[8];
    double var1[8];
    double var2[8];
    double var3[8];
    double var4[8];
    double var5[8];
    double var6[8];
    double var7[8];
    double var8[8];
    double var9[8];
    double var10[8];
    double var11[8];
    double var12[8];
    double var13[8];
    double var14[8];
    double var15[8];
    double var16[8];
    double var17[8];
    double var18[8];
    double var19[8];
    double var20[8];
    double var21[8];
    double var22[8];
    double var23[8];
    double var24[8];
    double var25[8];
    double var26[8];
    double var27[8];
    double var28[8];
    double var29[8];
    double var30[8];
    double var31[8];
    double var32[8];
    double var33[8];
    double var34[8];
    double var35[8];
    if (input[10] <= 165.75) {
        if (input[4] <= 987.25) {
            if (input[5] <= 169.23333740234375) {
                if (input[11] <= 969.1896667480469) {
                    if (input[11] <= 98.19061279296875) {
                        if (input[2] <= 1.5) {
                            memcpy(var35, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var35, (double[]){0.0, 0.0, 0.4, 0.2, 0.4, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        memcpy(var35, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                } else {
                    memcpy(var35, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            } else {
                memcpy(var35, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0}, 8 * sizeof(double));
            }
        } else {
            if (input[4] <= 2758.0) {
                if (input[12] <= 193.01934051513672) {
                    if (input[3] <= 3042.2000732421875) {
                        if (input[2] <= 5.5) {
                            memcpy(var35, (double[]){0.0, 0.0, 0.0, 0.8571428571428571, 0.14285714285714285, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var35, (double[]){0.0, 0.0, 0.0, 0.36363636363636365, 0.6363636363636364, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        memcpy(var35, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                } else {
                    if (input[5] <= 407.125) {
                        memcpy(var35, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        if (input[13] <= 1718.3843994140625) {
                            memcpy(var35, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var35, (double[]){0.0, 0.0, 0.5, 0.16666666666666666, 0.3333333333333333, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                }
            } else {
                if (input[7] <= 426.952392578125) {
                    memcpy(var35, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                } else {
                    if (input[8] <= 4815.392822265625) {
                        memcpy(var35, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        memcpy(var35, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                }
            }
        }
    } else {
        if (input[18] <= 1.2495181560516357) {
            memcpy(var35, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0}, 8 * sizeof(double));
        } else {
            if (input[0] <= 0.5) {
                if (input[10] <= 693.9375) {
                    if (input[9] <= 11186.21630859375) {
                        memcpy(var35, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        memcpy(var35, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                } else {
                    if (input[10] <= 1409.5555419921875) {
                        if (input[6] <= 1082.4857177734375) {
                            memcpy(var35, (double[]){0.3333333333333333, 0.0, 0.6666666666666666, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var35, (double[]){0.02857142857142857, 0.9142857142857143, 0.05714285714285714, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        if (input[1] <= 28.5) {
                            memcpy(var35, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var35, (double[]){0.3333333333333333, 0.6666666666666666, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                }
            } else {
                if (input[6] <= 585.25) {
                    memcpy(var35, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0}, 8 * sizeof(double));
                } else {
                    memcpy(var35, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            }
        }
    }
    double var36[8];
    if (input[5] <= 445.8666687011719) {
        if (input[16] <= 3.1827051639556885) {
            if (input[6] <= 530.3000030517578) {
                if (input[15] <= 42.749267578125) {
                    if (input[1] <= 7.5) {
                        memcpy(var36, (double[]){0.0, 0.0, 0.5, 0.0, 0.5, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        if (input[1] <= 9.5) {
                            memcpy(var36, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var36, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                } else {
                    if (input[18] <= 1.2495181560516357) {
                        memcpy(var36, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0}, 8 * sizeof(double));
                    } else {
                        memcpy(var36, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                }
            } else {
                memcpy(var36, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0}, 8 * sizeof(double));
            }
        } else {
            if (input[4] <= 974.8333435058594) {
                if (input[0] <= 0.5) {
                    if (input[11] <= 977.0693969726562) {
                        if (input[17] <= 1.418717086315155) {
                            memcpy(var36, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var36, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        memcpy(var36, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                } else {
                    memcpy(var36, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0}, 8 * sizeof(double));
                }
            } else {
                if (input[4] <= 2363.4166259765625) {
                    if (input[16] <= 3.8592673540115356) {
                        memcpy(var36, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        if (input[2] <= 5.5) {
                            memcpy(var36, (double[]){0.0, 0.0, 0.0, 0.7857142857142857, 0.21428571428571427, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var36, (double[]){0.0, 0.0, 0.0, 0.3888888888888889, 0.6111111111111112, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                } else {
                    if (input[10] <= 51.64285659790039) {
                        memcpy(var36, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        if (input[4] <= 2490.5) {
                            memcpy(var36, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var36, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                }
            }
        }
    } else {
        if (input[1] <= 28.5) {
            if (input[7] <= 271.67857360839844) {
                if (input[18] <= 1.4329145550727844) {
                    memcpy(var36, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                } else {
                    memcpy(var36, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            } else {
                if (input[9] <= 7875.166748046875) {
                    if (input[12] <= 166.40697479248047) {
                        if (input[10] <= 1804.2999877929688) {
                            memcpy(var36, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var36, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        memcpy(var36, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                } else {
                    if (input[17] <= 1.4048227071762085) {
                        if (input[15] <= 15.439496040344238) {
                            memcpy(var36, (double[]){0.0, 0.0, 0.5, 0.5, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var36, (double[]){0.09375, 0.90625, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        memcpy(var36, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                }
            }
        } else {
            if (input[16] <= 2.0199296474456787) {
                if (input[15] <= 175.9742660522461) {
                    if (input[3] <= 3625.300048828125) {
                        memcpy(var36, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        if (input[13] <= 979.4530029296875) {
                            memcpy(var36, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var36, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                } else {
                    memcpy(var36, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            } else {
                if (input[10] <= 286.82142639160156) {
                    if (input[16] <= 8.114880084991455) {
                        if (input[9] <= 9734.25) {
                            memcpy(var36, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var36, (double[]){0.0, 0.0, 0.42857142857142855, 0.42857142857142855, 0.14285714285714285, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        memcpy(var36, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                } else {
                    memcpy(var36, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            }
        }
    }
    add_vectors(var35, var36, 8, var34);
    double var37[8];
    if (input[12] <= 44.09356880187988) {
        if (input[5] <= 182.875) {
            if (input[7] <= 113.91666793823242) {
                if (input[1] <= 6.5) {
                    memcpy(var37, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                } else {
                    if (input[5] <= 155.20833587646484) {
                        if (input[8] <= 451.0) {
                            memcpy(var37, (double[]){0.0, 0.0, 0.16666666666666666, 0.0, 0.8333333333333334, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var37, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        memcpy(var37, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                }
            } else {
                if (input[9] <= 10616.60009765625) {
                    memcpy(var37, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0}, 8 * sizeof(double));
                } else {
                    if (input[6] <= 1348.0) {
                        memcpy(var37, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        memcpy(var37, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                }
            }
        } else {
            if (input[8] <= 1335.0) {
                if (input[9] <= 8662.0) {
                    memcpy(var37, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0}, 8 * sizeof(double));
                } else {
                    memcpy(var37, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            } else {
                if (input[7] <= 149.5250015258789) {
                    if (input[9] <= 11275.625) {
                        memcpy(var37, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        memcpy(var37, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                } else {
                    memcpy(var37, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0}, 8 * sizeof(double));
                }
            }
        }
    } else {
        if (input[0] <= 0.5) {
            if (input[1] <= 22.5) {
                if (input[10] <= 1270.0142822265625) {
                    if (input[17] <= 1.3858915567398071) {
                        if (input[14] <= 361.98765563964844) {
                            memcpy(var37, (double[]){0.0, 0.9166666666666666, 0.08333333333333333, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var37, (double[]){0.0, 0.0, 0.5, 0.5, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        if (input[13] <= 954.2078552246094) {
                            memcpy(var37, (double[]){0.0, 0.0, 0.8888888888888888, 0.1111111111111111, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var37, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                } else {
                    if (input[12] <= 166.40697479248047) {
                        if (input[13] <= 115.46342468261719) {
                            memcpy(var37, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var37, (double[]){0.25, 0.75, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        memcpy(var37, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                }
            } else {
                if (input[16] <= 1.2509716153144836) {
                    memcpy(var37, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                } else {
                    if (input[10] <= 71.875) {
                        if (input[15] <= 33.44061851501465) {
                            memcpy(var37, (double[]){0.0, 0.0, 0.0, 0.25925925925925924, 0.7407407407407407, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var37, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        if (input[5] <= 1369.4722290039062) {
                            memcpy(var37, (double[]){0.0, 0.025, 0.45, 0.5, 0.025, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var37, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                }
            }
        } else {
            memcpy(var37, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0}, 8 * sizeof(double));
        }
    }
    add_vectors(var34, var37, 8, var33);
    double var38[8];
    if (input[18] <= 1.252532184123993) {
        if (input[17] <= 1.139391303062439) {
            if (input[2] <= 1.5) {
                if (input[1] <= 6.5) {
                    memcpy(var38, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                } else {
                    memcpy(var38, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            } else {
                if (input[1] <= 9.5) {
                    memcpy(var38, (double[]){0.0, 0.0, 0.0, 0.75, 0.25, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                } else {
                    memcpy(var38, (double[]){0.0, 0.0, 0.5, 0.0, 0.5, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            }
        } else {
            memcpy(var38, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0}, 8 * sizeof(double));
        }
    } else {
        if (input[1] <= 13.5) {
            if (input[14] <= 367.38072204589844) {
                if (input[12] <= 30.86594271659851) {
                    memcpy(var38, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0}, 8 * sizeof(double));
                } else {
                    if (input[12] <= 169.72499084472656) {
                        if (input[18] <= 1.3415306210517883) {
                            memcpy(var38, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var38, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        if (input[16] <= 1.3000023365020752) {
                            memcpy(var38, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var38, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                }
            } else {
                if (input[12] <= 38.695695877075195) {
                    if (input[18] <= 1.8228754997253418) {
                        memcpy(var38, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0}, 8 * sizeof(double));
                    } else {
                        memcpy(var38, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                } else {
                    if (input[15] <= 111.08056640625) {
                        memcpy(var38, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        memcpy(var38, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                }
            }
        } else {
            if (input[16] <= 2.1284735202789307) {
                if (input[3] <= 1794.8839111328125) {
                    if (input[14] <= 80.14792442321777) {
                        memcpy(var38, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        memcpy(var38, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                } else {
                    if (input[5] <= 2110.451416015625) {
                        if (input[6] <= 1627.857177734375) {
                            memcpy(var38, (double[]){0.8, 0.0, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var38, (double[]){0.16129032258064518, 0.8064516129032259, 0.03225806451612904, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        memcpy(var38, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                }
            } else {
                if (input[10] <= 190.0625) {
                    if (input[8] <= 3900.5) {
                        if (input[12] <= 95.31817245483398) {
                            memcpy(var38, (double[]){0.0, 0.0, 0.0, 0.45454545454545453, 0.5454545454545454, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var38, (double[]){0.0, 0.0, 0.06060606060606061, 0.8181818181818182, 0.12121212121212122, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        if (input[5] <= 618.1745910644531) {
                            memcpy(var38, (double[]){0.0, 0.0, 0.09090909090909091, 0.045454545454545456, 0.8636363636363636, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var38, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                } else {
                    if (input[13] <= 1170.6909790039062) {
                        memcpy(var38, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        if (input[8] <= 3518.6500244140625) {
                            memcpy(var38, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var38, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                }
            }
        }
    }
    add_vectors(var33, var38, 8, var32);
    double var39[8];
    if (input[0] <= 0.5) {
        if (input[5] <= 700.625) {
            if (input[4] <= 1398.6666870117188) {
                if (input[15] <= 146.87681579589844) {
                    if (input[18] <= 1.6372559070587158) {
                        if (input[9] <= 7021.541748046875) {
                            memcpy(var39, (double[]){0.75, 0.0, 0.0, 0.0, 0.25, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var39, (double[]){0.0, 0.45454545454545453, 0.2727272727272727, 0.2727272727272727, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        memcpy(var39, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                } else {
                    memcpy(var39, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            } else {
                if (input[7] <= 156.41666412353516) {
                    if (input[16] <= 14.830659866333008) {
                        memcpy(var39, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        memcpy(var39, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                } else {
                    if (input[5] <= 382.5952453613281) {
                        if (input[10] <= 49.988094329833984) {
                            memcpy(var39, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var39, (double[]){0.0, 0.0, 0.16666666666666666, 0.5, 0.3333333333333333, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        if (input[15] <= 66.49799346923828) {
                            memcpy(var39, (double[]){0.0, 0.0, 0.0, 0.875, 0.125, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var39, (double[]){0.0, 0.0, 0.6363636363636364, 0.36363636363636365, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                }
            }
        } else {
            if (input[1] <= 28.5) {
                if (input[7] <= 674.3666687011719) {
                    if (input[16] <= 1.3209794163703918) {
                        if (input[11] <= 279.9795379638672) {
                            memcpy(var39, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var39, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        memcpy(var39, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                } else {
                    memcpy(var39, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            } else {
                if (input[14] <= 2116.345001220703) {
                    if (input[5] <= 1369.4722290039062) {
                        if (input[4] <= 2126.25) {
                            memcpy(var39, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var39, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        if (input[15] <= 38.81235885620117) {
                            memcpy(var39, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var39, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                } else {
                    memcpy(var39, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            }
        }
    } else {
        if (input[16] <= 3.1869221925735474) {
            if (input[12] <= 34.897311210632324) {
                memcpy(var39, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0}, 8 * sizeof(double));
            } else {
                if (input[4] <= 818.2749938964844) {
                    memcpy(var39, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0}, 8 * sizeof(double));
                } else {
                    memcpy(var39, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0}, 8 * sizeof(double));
                }
            }
        } else {
            memcpy(var39, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0}, 8 * sizeof(double));
        }
    }
    add_vectors(var32, var39, 8, var31);
    double var40[8];
    if (input[18] <= 1.2495181560516357) {
        if (input[18] <= 1.0689808130264282) {
            memcpy(var40, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
        } else {
            memcpy(var40, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0}, 8 * sizeof(double));
        }
    } else {
        if (input[16] <= 3.4731894731521606) {
            if (input[8] <= 1245.7666625976562) {
                if (input[9] <= 8903.5) {
                    if (input[11] <= 193.46337890625) {
                        memcpy(var40, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        memcpy(var40, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                } else {
                    memcpy(var40, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            } else {
                if (input[5] <= 1757.1832885742188) {
                    if (input[10] <= 898.2999877929688) {
                        if (input[5] <= 1221.3125) {
                            memcpy(var40, (double[]){0.0, 0.09090909090909091, 0.9090909090909091, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var40, (double[]){0.0, 0.75, 0.25, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        if (input[17] <= 1.420472800731659) {
                            memcpy(var40, (double[]){0.23529411764705882, 0.7647058823529411, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var40, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                } else {
                    if (input[10] <= 1530.75) {
                        if (input[7] <= 840.2778015136719) {
                            memcpy(var40, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var40, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        memcpy(var40, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                }
            }
        } else {
            if (input[5] <= 212.0) {
                if (input[3] <= 1366.7750244140625) {
                    if (input[6] <= 535.0) {
                        memcpy(var40, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        if (input[17] <= 1.352962613105774) {
                            memcpy(var40, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var40, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                } else {
                    if (input[17] <= 1.3937729597091675) {
                        memcpy(var40, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        memcpy(var40, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                }
            } else {
                if (input[10] <= 54.071428298950195) {
                    if (input[4] <= 2079.607177734375) {
                        if (input[11] <= 1351.6376342773438) {
                            memcpy(var40, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var40, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        memcpy(var40, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                } else {
                    if (input[18] <= 1.6719442009925842) {
                        if (input[8] <= 5049.053466796875) {
                            memcpy(var40, (double[]){0.0, 0.0, 0.2222222222222222, 0.7222222222222222, 0.05555555555555555, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var40, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        if (input[16] <= 6.551791191101074) {
                            memcpy(var40, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var40, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                }
            }
        }
    }
    add_vectors(var31, var40, 8, var30);
    double var41[8];
    if (input[6] <= 480.0500030517578) {
        if (input[15] <= 42.749267578125) {
            if (input[1] <= 8.0) {
                memcpy(var41, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
            } else {
                if (input[1] <= 9.5) {
                    memcpy(var41, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                } else {
                    memcpy(var41, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            }
        } else {
            if (input[7] <= 170.25) {
                memcpy(var41, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0}, 8 * sizeof(double));
            } else {
                memcpy(var41, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
            }
        }
    } else {
        if (input[12] <= 38.695695877075195) {
            if (input[0] <= 0.5) {
                if (input[11] <= 167.9466323852539) {
                    memcpy(var41, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                } else {
                    memcpy(var41, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            } else {
                memcpy(var41, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0}, 8 * sizeof(double));
            }
        } else {
            if (input[8] <= 1267.9749755859375) {
                if (input[0] <= 0.5) {
                    if (input[5] <= 90.83333396911621) {
                        memcpy(var41, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        if (input[18] <= 1.424514889717102) {
                            memcpy(var41, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var41, (double[]){0.5, 0.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                } else {
                    memcpy(var41, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            } else {
                if (input[5] <= 628.6856994628906) {
                    if (input[3] <= 3563.4285888671875) {
                        if (input[13] <= 1011.2929077148438) {
                            memcpy(var41, (double[]){0.0, 0.058823529411764705, 0.7058823529411765, 0.17647058823529413, 0.058823529411764705, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var41, (double[]){0.0, 0.0, 0.0975609756097561, 0.5609756097560976, 0.34146341463414637, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        if (input[5] <= 573.7767944335938) {
                            memcpy(var41, (double[]){0.0, 0.0, 0.07142857142857142, 0.0, 0.9285714285714286, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var41, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                } else {
                    if (input[16] <= 1.265863060951233) {
                        if (input[5] <= 888.9833374023438) {
                            memcpy(var41, (double[]){0.3333333333333333, 0.6666666666666666, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var41, (double[]){0.9705882352941176, 0.029411764705882353, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        if (input[10] <= 682.4375) {
                            memcpy(var41, (double[]){0.0, 0.0, 0.9444444444444444, 0.05555555555555555, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var41, (double[]){0.0, 0.8780487804878049, 0.12195121951219512, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                }
            }
        }
    }
    add_vectors(var30, var41, 8, var29);
    double var42[8];
    if (input[10] <= 912.9499816894531) {
        if (input[10] <= 321.3541717529297) {
            if (input[0] <= 0.5) {
                if (input[16] <= 7.350059270858765) {
                    if (input[5] <= 568.9732055664062) {
                        if (input[18] <= 1.622126042842865) {
                            memcpy(var42, (double[]){0.0, 0.0, 0.47368421052631576, 0.3684210526315789, 0.15789473684210525, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var42, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        if (input[11] <= 1829.6788940429688) {
                            memcpy(var42, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var42, (double[]){0.0, 0.0, 0.5, 0.5, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                } else {
                    if (input[2] <= 5.5) {
                        if (input[16] <= 9.276399612426758) {
                            memcpy(var42, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var42, (double[]){0.0, 0.0, 0.0, 0.25, 0.75, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        if (input[9] <= 9683.0712890625) {
                            memcpy(var42, (double[]){0.0, 0.0, 0.0, 0.5, 0.5, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var42, (double[]){0.0, 0.0, 0.030303030303030304, 0.06060606060606061, 0.9090909090909091, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                }
            } else {
                memcpy(var42, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0}, 8 * sizeof(double));
            }
        } else {
            if (input[18] <= 1.301936686038971) {
                memcpy(var42, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0}, 8 * sizeof(double));
            } else {
                if (input[12] <= 208.56381225585938) {
                    if (input[14] <= 354.58741760253906) {
                        if (input[15] <= 17.137521266937256) {
                            memcpy(var42, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var42, (double[]){0.0, 0.3333333333333333, 0.0, 0.6666666666666666, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        if (input[14] <= 2823.3612060546875) {
                            memcpy(var42, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var42, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                } else {
                    if (input[17] <= 1.4329598546028137) {
                        memcpy(var42, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        if (input[7] <= 527.6944580078125) {
                            memcpy(var42, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var42, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                }
            }
        }
    } else {
        if (input[17] <= 1.3853232264518738) {
            if (input[16] <= 1.2516557574272156) {
                if (input[9] <= 6744.27490234375) {
                    memcpy(var42, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                } else {
                    if (input[18] <= 1.3761141300201416) {
                        if (input[7] <= 1015.5158996582031) {
                            memcpy(var42, (double[]){0.2857142857142857, 0.7142857142857143, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var42, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        memcpy(var42, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                }
            } else {
                if (input[6] <= 1043.0666809082031) {
                    if (input[14] <= 466.94032287597656) {
                        memcpy(var42, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        memcpy(var42, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                } else {
                    if (input[18] <= 1.4242706298828125) {
                        memcpy(var42, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        if (input[16] <= 1.3212295770645142) {
                            memcpy(var42, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var42, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                }
            }
        } else {
            if (input[16] <= 1.2374500632286072) {
                memcpy(var42, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
            } else {
                if (input[9] <= 9246.12890625) {
                    memcpy(var42, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                } else {
                    memcpy(var42, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            }
        }
    }
    add_vectors(var29, var42, 8, var28);
    double var43[8];
    if (input[12] <= 38.695695877075195) {
        if (input[10] <= 339.9166717529297) {
            if (input[9] <= 11292.91650390625) {
                if (input[8] <= 305.75) {
                    if (input[1] <= 8.5) {
                        memcpy(var43, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        memcpy(var43, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                } else {
                    if (input[0] <= 0.5) {
                        memcpy(var43, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        memcpy(var43, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0}, 8 * sizeof(double));
                    }
                }
            } else {
                memcpy(var43, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
            }
        } else {
            memcpy(var43, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0}, 8 * sizeof(double));
        }
    } else {
        if (input[16] <= 2.076227903366089) {
            if (input[9] <= 8303.9443359375) {
                if (input[0] <= 0.5) {
                    if (input[8] <= 3272.9603271484375) {
                        if (input[15] <= 32.51734638214111) {
                            memcpy(var43, (double[]){0.75, 0.25, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var43, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        if (input[8] <= 3383.015869140625) {
                            memcpy(var43, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var43, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                } else {
                    memcpy(var43, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0}, 8 * sizeof(double));
                }
            } else {
                if (input[5] <= 787.4761962890625) {
                    if (input[6] <= 1131.4166870117188) {
                        if (input[9] <= 8648.9287109375) {
                            memcpy(var43, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var43, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        memcpy(var43, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                } else {
                    if (input[16] <= 1.3363964557647705) {
                        if (input[13] <= 765.5860748291016) {
                            memcpy(var43, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var43, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        memcpy(var43, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                }
            }
        } else {
            if (input[11] <= 672.5299987792969) {
                if (input[17] <= 1.3824143409729004) {
                    memcpy(var43, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0}, 8 * sizeof(double));
                } else {
                    if (input[18] <= 1.588739812374115) {
                        memcpy(var43, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        memcpy(var43, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                }
            } else {
                if (input[8] <= 4318.083251953125) {
                    if (input[2] <= 5.5) {
                        if (input[1] <= 18.0) {
                            memcpy(var43, (double[]){0.0, 0.0, 0.25, 0.0, 0.25, 0.0, 0.0, 0.5}, 8 * sizeof(double));
                        } else {
                            memcpy(var43, (double[]){0.0, 0.0, 0.0625, 0.875, 0.0625, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        if (input[5] <= 372.48809814453125) {
                            memcpy(var43, (double[]){0.0, 0.0, 0.0, 0.14285714285714285, 0.8571428571428571, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var43, (double[]){0.0, 0.0, 0.6052631578947368, 0.34210526315789475, 0.05263157894736842, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                } else {
                    if (input[10] <= 104.11904525756836) {
                        memcpy(var43, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        memcpy(var43, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                }
            }
        }
    }
    add_vectors(var28, var43, 8, var27);
    double var44[8];
    if (input[4] <= 985.0499877929688) {
        if (input[5] <= 254.1666717529297) {
            if (input[15] <= 83.1524429321289) {
                if (input[7] <= 109.75) {
                    if (input[2] <= 3.0) {
                        memcpy(var44, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        if (input[7] <= 89.45833206176758) {
                            memcpy(var44, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var44, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                } else {
                    memcpy(var44, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0}, 8 * sizeof(double));
                }
            } else {
                memcpy(var44, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0}, 8 * sizeof(double));
            }
        } else {
            if (input[6] <= 560.25) {
                memcpy(var44, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
            } else {
                if (input[18] <= 1.42678964138031) {
                    if (input[10] <= 1253.4000244140625) {
                        memcpy(var44, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        memcpy(var44, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                } else {
                    memcpy(var44, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            }
        }
    } else {
        if (input[10] <= 680.25) {
            if (input[5] <= 437.3333435058594) {
                if (input[7] <= 164.6999969482422) {
                    if (input[5] <= 156.63333892822266) {
                        memcpy(var44, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        if (input[15] <= 16.352631092071533) {
                            memcpy(var44, (double[]){0.0, 0.0, 0.0, 0.3333333333333333, 0.6666666666666666, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var44, (double[]){0.0, 0.0, 0.0625, 0.875, 0.0625, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                } else {
                    if (input[5] <= 365.7857208251953) {
                        memcpy(var44, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        if (input[17] <= 1.3856849074363708) {
                            memcpy(var44, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var44, (double[]){0.0, 0.0, 0.0, 0.8888888888888888, 0.1111111111111111, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                }
            } else {
                if (input[16] <= 4.066655278205872) {
                    memcpy(var44, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                } else {
                    if (input[13] <= 1718.3843994140625) {
                        memcpy(var44, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        if (input[15] <= 45.817983627319336) {
                            memcpy(var44, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var44, (double[]){0.0, 0.0, 0.6666666666666666, 0.3333333333333333, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                }
            }
        } else {
            if (input[16] <= 1.2622830867767334) {
                if (input[13] <= 195.0392303466797) {
                    if (input[6] <= 1047.3250122070312) {
                        memcpy(var44, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        memcpy(var44, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                } else {
                    if (input[4] <= 2479.4732666015625) {
                        memcpy(var44, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        if (input[3] <= 3289.517822265625) {
                            memcpy(var44, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var44, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                }
            } else {
                if (input[4] <= 1195.9166870117188) {
                    if (input[16] <= 1.7172241806983948) {
                        memcpy(var44, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        memcpy(var44, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                } else {
                    if (input[17] <= 1.3677310347557068) {
                        memcpy(var44, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        if (input[9] <= 8970.60009765625) {
                            memcpy(var44, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var44, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                }
            }
        }
    }
    add_vectors(var27, var44, 8, var26);
    double var45[8];
    if (input[0] <= 0.5) {
        if (input[5] <= 740.4464416503906) {
            if (input[16] <= 3.970415711402893) {
                if (input[14] <= 93.18865203857422) {
                    if (input[3] <= 287.5) {
                        if (input[1] <= 8.0) {
                            memcpy(var45, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var45, (double[]){0.0, 0.0, 0.3333333333333333, 0.3333333333333333, 0.3333333333333333, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        if (input[2] <= 3.5) {
                            memcpy(var45, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var45, (double[]){0.0, 0.5, 0.0, 0.5, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                } else {
                    if (input[10] <= 928.4166564941406) {
                        memcpy(var45, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        memcpy(var45, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                }
            } else {
                if (input[15] <= 25.753628730773926) {
                    if (input[6] <= 2902.5833740234375) {
                        if (input[9] <= 11169.0478515625) {
                            memcpy(var45, (double[]){0.0, 0.0, 0.0, 0.6, 0.4, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var45, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        memcpy(var45, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                } else {
                    if (input[5] <= 292.6547546386719) {
                        if (input[14] <= 5044.04541015625) {
                            memcpy(var45, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var45, (double[]){0.0, 0.0, 0.1111111111111111, 0.7777777777777778, 0.1111111111111111, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        if (input[7] <= 287.25) {
                            memcpy(var45, (double[]){0.0, 0.0, 0.3333333333333333, 0.5333333333333333, 0.13333333333333333, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var45, (double[]){0.0, 0.0, 0.047619047619047616, 0.9523809523809523, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                }
            }
        } else {
            if (input[16] <= 1.3209794163703918) {
                if (input[12] <= 753.6448974609375) {
                    memcpy(var45, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                } else {
                    if (input[10] <= 1607.625) {
                        memcpy(var45, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        memcpy(var45, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                }
            } else {
                if (input[12] <= 520.3931274414062) {
                    if (input[8] <= 2965.4375) {
                        memcpy(var45, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        memcpy(var45, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                } else {
                    memcpy(var45, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            }
        }
    } else {
        if (input[16] <= 2.1303595304489136) {
            memcpy(var45, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0}, 8 * sizeof(double));
        } else {
            if (input[5] <= 254.1666717529297) {
                memcpy(var45, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0}, 8 * sizeof(double));
            } else {
                memcpy(var45, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0}, 8 * sizeof(double));
            }
        }
    }
    add_vectors(var26, var45, 8, var25);
    double var46[8];
    if (input[0] <= 0.5) {
        if (input[16] <= 1.3038802742958069) {
            if (input[17] <= 1.1133105754852295) {
                if (input[2] <= 1.5) {
                    memcpy(var46, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                } else {
                    memcpy(var46, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            } else {
                if (input[9] <= 6836.257080078125) {
                    memcpy(var46, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                } else {
                    if (input[7] <= 840.2778015136719) {
                        if (input[9] <= 6960.64990234375) {
                            memcpy(var46, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var46, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        memcpy(var46, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                }
            }
        } else {
            if (input[10] <= 898.2999877929688) {
                if (input[10] <= 128.4499969482422) {
                    if (input[9] <= 9590.7666015625) {
                        if (input[8] <= 977.125) {
                            memcpy(var46, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var46, (double[]){0.0, 0.0, 0.0, 0.9375, 0.0625, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        if (input[7] <= 223.91666412353516) {
                            memcpy(var46, (double[]){0.0, 0.0, 0.0, 0.09523809523809523, 0.9047619047619048, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var46, (double[]){0.0, 0.0, 0.125, 0.5625, 0.3125, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                } else {
                    if (input[9] <= 11186.21630859375) {
                        if (input[5] <= 1221.3125) {
                            memcpy(var46, (double[]){0.0, 0.057692307692307696, 0.8076923076923077, 0.09615384615384616, 0.038461538461538464, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var46, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        memcpy(var46, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                }
            } else {
                memcpy(var46, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
            }
        }
    } else {
        if (input[15] <= 99.25719451904297) {
            if (input[4] <= 566.5) {
                if (input[13] <= 99.36068344116211) {
                    memcpy(var46, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0}, 8 * sizeof(double));
                } else {
                    memcpy(var46, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            } else {
                if (input[12] <= 46.339744567871094) {
                    if (input[6] <= 532.0) {
                        memcpy(var46, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0}, 8 * sizeof(double));
                    } else {
                        memcpy(var46, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0}, 8 * sizeof(double));
                    }
                } else {
                    memcpy(var46, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            }
        } else {
            memcpy(var46, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0}, 8 * sizeof(double));
        }
    }
    add_vectors(var25, var46, 8, var24);
    double var47[8];
    if (input[0] <= 0.5) {
        if (input[5] <= 577.9642944335938) {
            if (input[18] <= 1.4857494235038757) {
                if (input[6] <= 2322.5) {
                    if (input[17] <= 1.3732196688652039) {
                        if (input[7] <= 192.0) {
                            memcpy(var47, (double[]){0.375, 0.0, 0.25, 0.125, 0.25, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var47, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        memcpy(var47, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                } else {
                    memcpy(var47, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            } else {
                if (input[10] <= 46.54166603088379) {
                    if (input[9] <= 9566.5) {
                        if (input[13] <= 1280.8943481445312) {
                            memcpy(var47, (double[]){0.0, 0.0, 0.0, 0.8333333333333334, 0.16666666666666666, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var47, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        memcpy(var47, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                } else {
                    if (input[10] <= 128.4499969482422) {
                        if (input[17] <= 1.5394791960716248) {
                            memcpy(var47, (double[]){0.0, 0.0, 0.03571428571428571, 0.75, 0.21428571428571427, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var47, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        if (input[18] <= 1.7859901785850525) {
                            memcpy(var47, (double[]){0.0, 0.0, 0.8, 0.2, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var47, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                }
            }
        } else {
            if (input[14] <= 2181.9957580566406) {
                if (input[9] <= 8303.9443359375) {
                    if (input[3] <= 3226.455322265625) {
                        if (input[8] <= 1507.63330078125) {
                            memcpy(var47, (double[]){0.7, 0.3, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var47, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        if (input[14] <= 411.56353759765625) {
                            memcpy(var47, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var47, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                } else {
                    if (input[18] <= 1.4623695611953735) {
                        if (input[18] <= 1.447442889213562) {
                            memcpy(var47, (double[]){0.0, 0.8888888888888888, 0.1111111111111111, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var47, (double[]){0.4, 0.6, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        if (input[16] <= 1.9853649139404297) {
                            memcpy(var47, (double[]){0.0, 0.8, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var47, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                }
            } else {
                if (input[5] <= 763.5625) {
                    memcpy(var47, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                } else {
                    memcpy(var47, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            }
        }
    } else {
        if (input[10] <= 333.75) {
            memcpy(var47, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0}, 8 * sizeof(double));
        } else {
            if (input[14] <= 1000.8206481933594) {
                if (input[6] <= 469.3333282470703) {
                    memcpy(var47, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0}, 8 * sizeof(double));
                } else {
                    memcpy(var47, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            } else {
                if (input[7] <= 153.60000610351562) {
                    memcpy(var47, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0}, 8 * sizeof(double));
                } else {
                    memcpy(var47, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            }
        }
    }
    add_vectors(var24, var47, 8, var23);
    double var48[8];
    if (input[6] <= 487.0500030517578) {
        if (input[18] <= 1.317383885383606) {
            if (input[10] <= 234.6666717529297) {
                if (input[1] <= 9.5) {
                    memcpy(var48, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                } else {
                    memcpy(var48, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            } else {
                memcpy(var48, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0}, 8 * sizeof(double));
            }
        } else {
            memcpy(var48, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
        }
    } else {
        if (input[9] <= 7734.381103515625) {
            if (input[16] <= 1.1573817133903503) {
                memcpy(var48, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
            } else {
                if (input[14] <= 343.7568817138672) {
                    if (input[16] <= 1.1755722165107727) {
                        memcpy(var48, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        memcpy(var48, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                } else {
                    if (input[9] <= 5754.1905517578125) {
                        memcpy(var48, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        memcpy(var48, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                }
            }
        } else {
            if (input[3] <= 1307.9000244140625) {
                if (input[16] <= 3.1869221925735474) {
                    memcpy(var48, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0}, 8 * sizeof(double));
                } else {
                    if (input[0] <= 0.5) {
                        if (input[6] <= 1020.0833129882812) {
                            memcpy(var48, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var48, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        memcpy(var48, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0}, 8 * sizeof(double));
                    }
                }
            } else {
                if (input[16] <= 3.9679774045944214) {
                    if (input[18] <= 1.4182880520820618) {
                        if (input[15] <= 27.35563373565674) {
                            memcpy(var48, (double[]){0.0, 0.2222222222222222, 0.7777777777777778, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var48, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        if (input[16] <= 1.7771799564361572) {
                            memcpy(var48, (double[]){0.375, 0.5, 0.125, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var48, (double[]){0.0, 0.0, 0.9090909090909091, 0.09090909090909091, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                } else {
                    if (input[17] <= 1.3870482444763184) {
                        if (input[10] <= 88.5) {
                            memcpy(var48, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var48, (double[]){0.0, 0.0, 0.0, 0.875, 0.125, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        if (input[13] <= 1718.3843994140625) {
                            memcpy(var48, (double[]){0.0, 0.0, 0.05263157894736842, 0.7894736842105263, 0.15789473684210525, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var48, (double[]){0.0, 0.0, 0.2, 0.1, 0.7, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                }
            }
        }
    }
    add_vectors(var23, var48, 8, var22);
    double var49[8];
    if (input[18] <= 1.2495181560516357) {
        if (input[14] <= 193.33326721191406) {
            if (input[2] <= 1.5) {
                memcpy(var49, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
            } else {
                if (input[1] <= 9.5) {
                    memcpy(var49, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                } else {
                    memcpy(var49, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            }
        } else {
            memcpy(var49, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0}, 8 * sizeof(double));
        }
    } else {
        if (input[16] <= 1.2516557574272156) {
            if (input[10] <= 1548.1749877929688) {
                if (input[17] <= 1.313654363155365) {
                    if (input[10] <= 1428.2142944335938) {
                        memcpy(var49, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        memcpy(var49, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                } else {
                    memcpy(var49, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            } else {
                memcpy(var49, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
            }
        } else {
            if (input[10] <= 898.2999877929688) {
                if (input[0] <= 0.5) {
                    if (input[16] <= 5.478549957275391) {
                        if (input[14] <= 139.04312896728516) {
                            memcpy(var49, (double[]){0.0, 0.6666666666666666, 0.16666666666666666, 0.16666666666666666, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var49, (double[]){0.0, 0.0196078431372549, 0.8235294117647058, 0.1568627450980392, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        if (input[5] <= 362.5) {
                            memcpy(var49, (double[]){0.0, 0.0, 0.0, 0.34285714285714286, 0.6571428571428571, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var49, (double[]){0.0, 0.0, 0.04545454545454546, 0.6818181818181819, 0.27272727272727276, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                } else {
                    if (input[10] <= 333.75) {
                        memcpy(var49, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0}, 8 * sizeof(double));
                    } else {
                        memcpy(var49, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                }
            } else {
                if (input[17] <= 1.4048227071762085) {
                    memcpy(var49, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                } else {
                    memcpy(var49, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            }
        }
    }
    add_vectors(var22, var49, 8, var21);
    double var50[8];
    if (input[18] <= 1.2495181560516357) {
        if (input[7] <= 69.125) {
            if (input[2] <= 1.5) {
                memcpy(var50, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
            } else {
                if (input[1] <= 9.5) {
                    memcpy(var50, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                } else {
                    memcpy(var50, (double[]){0.0, 0.0, 0.5, 0.0, 0.5, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            }
        } else {
            if (input[14] <= 304.4315719604492) {
                memcpy(var50, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
            } else {
                memcpy(var50, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0}, 8 * sizeof(double));
            }
        }
    } else {
        if (input[9] <= 7726.881103515625) {
            if (input[16] <= 2.5863442420959473) {
                memcpy(var50, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
            } else {
                if (input[15] <= 132.49798393249512) {
                    memcpy(var50, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                } else {
                    memcpy(var50, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            }
        } else {
            if (input[10] <= 321.3541717529297) {
                if (input[0] <= 0.5) {
                    if (input[10] <= 56.75) {
                        if (input[7] <= 128.29999923706055) {
                            memcpy(var50, (double[]){0.0, 0.0, 0.0, 0.4666666666666667, 0.5333333333333333, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var50, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        if (input[9] <= 9633.48779296875) {
                            memcpy(var50, (double[]){0.0, 0.0, 0.6666666666666666, 0.3333333333333333, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var50, (double[]){0.0, 0.0, 0.0625, 0.6875, 0.25, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                } else {
                    memcpy(var50, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0}, 8 * sizeof(double));
                }
            } else {
                if (input[4] <= 816.375) {
                    memcpy(var50, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0}, 8 * sizeof(double));
                } else {
                    if (input[10] <= 907.4375) {
                        if (input[17] <= 1.2926440238952637) {
                            memcpy(var50, (double[]){0.0, 0.625, 0.375, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var50, (double[]){0.0, 0.09523809523809525, 0.8571428571428572, 0.04761904761904762, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        if (input[9] <= 8798.0) {
                            memcpy(var50, (double[]){0.26666666666666666, 0.7333333333333333, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var50, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                }
            }
        }
    }
    add_vectors(var21, var50, 8, var20);
    double var51[8];
    if (input[5] <= 442.6428680419922) {
        if (input[16] <= 3.1827051639556885) {
            if (input[6] <= 530.3000030517578) {
                if (input[6] <= 130.375) {
                    if (input[2] <= 1.5) {
                        memcpy(var51, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        if (input[1] <= 9.5) {
                            memcpy(var51, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var51, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                } else {
                    if (input[10] <= 1623.0666198730469) {
                        memcpy(var51, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0}, 8 * sizeof(double));
                    } else {
                        memcpy(var51, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                }
            } else {
                memcpy(var51, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0}, 8 * sizeof(double));
            }
        } else {
            if (input[13] <= 640.2853088378906) {
                if (input[3] <= 1366.7333374023438) {
                    if (input[17] <= 1.4176592230796814) {
                        if (input[18] <= 1.7210693955421448) {
                            memcpy(var51, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var51, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        memcpy(var51, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                } else {
                    if (input[3] <= 1491.3333129882812) {
                        memcpy(var51, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        memcpy(var51, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                }
            } else {
                if (input[10] <= 173.8499984741211) {
                    if (input[15] <= 24.137192726135254) {
                        if (input[11] <= 776.4212646484375) {
                            memcpy(var51, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var51, (double[]){0.0, 0.0, 0.0, 0.08333333333333333, 0.9166666666666666, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        if (input[14] <= 5052.965576171875) {
                            memcpy(var51, (double[]){0.0, 0.0, 0.0, 0.3333333333333333, 0.6666666666666666, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var51, (double[]){0.0, 0.0, 0.0, 0.8461538461538461, 0.15384615384615385, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                } else {
                    memcpy(var51, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            }
        }
    } else {
        if (input[17] <= 1.3638045191764832) {
            if (input[16] <= 1.2612122893333435) {
                if (input[4] <= 2373.0357666015625) {
                    memcpy(var51, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                } else {
                    if (input[4] <= 2509.435791015625) {
                        memcpy(var51, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        memcpy(var51, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                }
            } else {
                if (input[14] <= 619.7030334472656) {
                    if (input[16] <= 2.1284735202789307) {
                        if (input[11] <= 399.31275939941406) {
                            memcpy(var51, (double[]){0.0, 0.5, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var51, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        memcpy(var51, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                } else {
                    if (input[8] <= 4649.53955078125) {
                        if (input[17] <= 1.322085201740265) {
                            memcpy(var51, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var51, (double[]){0.5, 0.0, 0.0, 0.0, 0.5, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        memcpy(var51, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                }
            }
        } else {
            if (input[1] <= 22.5) {
                if (input[14] <= 437.5498962402344) {
                    if (input[7] <= 380.1666717529297) {
                        if (input[8] <= 1887.8095703125) {
                            memcpy(var51, (double[]){0.0, 0.875, 0.125, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var51, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        memcpy(var51, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                } else {
                    memcpy(var51, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            } else {
                if (input[11] <= 1746.5721435546875) {
                    if (input[12] <= 636.7636108398438) {
                        if (input[10] <= 290.25) {
                            memcpy(var51, (double[]){0.0, 0.0, 0.4, 0.6, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var51, (double[]){0.0, 0.0, 0.9523809523809523, 0.047619047619047616, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        memcpy(var51, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                } else {
                    if (input[11] <= 1832.4653930664062) {
                        if (input[15] <= 26.28215789794922) {
                            memcpy(var51, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var51, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        if (input[17] <= 1.3736178278923035) {
                            memcpy(var51, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var51, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                }
            }
        }
    }
    add_vectors(var20, var51, 8, var19);
    double var52[8];
    if (input[16] <= 2.1332982778549194) {
        if (input[18] <= 1.252532184123993) {
            if (input[11] <= 68.7337875366211) {
                if (input[1] <= 9.5) {
                    memcpy(var52, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                } else {
                    memcpy(var52, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            } else {
                if (input[0] <= 0.5) {
                    memcpy(var52, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                } else {
                    memcpy(var52, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0}, 8 * sizeof(double));
                }
            }
        } else {
            if (input[9] <= 8255.46630859375) {
                if (input[8] <= 3294.0714111328125) {
                    if (input[16] <= 1.1557065844535828) {
                        memcpy(var52, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        if (input[12] <= 267.89515686035156) {
                            memcpy(var52, (double[]){0.5, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var52, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                } else {
                    if (input[8] <= 3392.8839111328125) {
                        memcpy(var52, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        memcpy(var52, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                }
            } else {
                if (input[7] <= 242.85713958740234) {
                    memcpy(var52, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                } else {
                    if (input[6] <= 1082.4857177734375) {
                        if (input[5] <= 708.5428466796875) {
                            memcpy(var52, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var52, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        if (input[10] <= 1349.772705078125) {
                            memcpy(var52, (double[]){0.0, 0.9655172413793104, 0.034482758620689655, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var52, (double[]){0.6666666666666666, 0.3333333333333333, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                }
            }
        }
    } else {
        if (input[1] <= 13.5) {
            if (input[6] <= 1199.699951171875) {
                if (input[12] <= 27.7586088180542) {
                    memcpy(var52, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0}, 8 * sizeof(double));
                } else {
                    if (input[15] <= 172.26427459716797) {
                        memcpy(var52, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        memcpy(var52, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                }
            } else {
                if (input[9] <= 10889.5) {
                    memcpy(var52, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0}, 8 * sizeof(double));
                } else {
                    memcpy(var52, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            }
        } else {
            if (input[10] <= 180.45536041259766) {
                if (input[11] <= 1921.0965576171875) {
                    if (input[11] <= 1824.1137084960938) {
                        if (input[5] <= 232.5749969482422) {
                            memcpy(var52, (double[]){0.0, 0.0, 0.0, 0.875, 0.125, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var52, (double[]){0.0, 0.0, 0.03225806451612903, 0.4838709677419355, 0.4838709677419355, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        memcpy(var52, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                } else {
                    if (input[10] <= 44.14285659790039) {
                        memcpy(var52, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        memcpy(var52, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                }
            } else {
                if (input[4] <= 2594.3125) {
                    if (input[2] <= 5.5) {
                        if (input[4] <= 1155.0999755859375) {
                            memcpy(var52, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var52, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        memcpy(var52, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                } else {
                    if (input[6] <= 3519.6429443359375) {
                        memcpy(var52, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        memcpy(var52, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                }
            }
        }
    }
    add_vectors(var19, var52, 8, var18);
    double var53[8];
    if (input[6] <= 480.0500030517578) {
        if (input[2] <= 2.5) {
            if (input[1] <= 7.5) {
                memcpy(var53, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
            } else {
                if (input[1] <= 9.5) {
                    memcpy(var53, (double[]){0.0, 0.0, 0.0, 0.5, 0.5, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                } else {
                    memcpy(var53, (double[]){0.0, 0.0, 0.5, 0.0, 0.5, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            }
        } else {
            if (input[15] <= 198.6787338256836) {
                memcpy(var53, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0}, 8 * sizeof(double));
            } else {
                memcpy(var53, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
            }
        }
    } else {
        if (input[4] <= 887.2000122070312) {
            if (input[11] <= 130.39956283569336) {
                memcpy(var53, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0}, 8 * sizeof(double));
            } else {
                if (input[10] <= 315.25) {
                    if (input[7] <= 133.5749969482422) {
                        if (input[6] <= 582.25) {
                            memcpy(var53, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var53, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        memcpy(var53, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0}, 8 * sizeof(double));
                    }
                } else {
                    if (input[7] <= 180.5) {
                        memcpy(var53, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        memcpy(var53, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                }
            }
        } else {
            if (input[18] <= 1.456339180469513) {
                if (input[5] <= 2024.5982055664062) {
                    if (input[16] <= 1.175044596195221) {
                        memcpy(var53, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        if (input[18] <= 1.4182880520820618) {
                            memcpy(var53, (double[]){0.08571428571428572, 0.8571428571428571, 0.05714285714285714, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var53, (double[]){0.0625, 0.3125, 0.5625, 0.0625, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                } else {
                    memcpy(var53, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            } else {
                if (input[16] <= 5.478549957275391) {
                    if (input[10] <= 1068.0999755859375) {
                        if (input[11] <= 1323.2205810546875) {
                            memcpy(var53, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var53, (double[]){0.0, 0.0, 0.6666666666666666, 0.3333333333333333, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        memcpy(var53, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                } else {
                    if (input[4] <= 2570.142822265625) {
                        if (input[7] <= 284.7291717529297) {
                            memcpy(var53, (double[]){0.0, 0.0, 0.026315789473684213, 0.5263157894736843, 0.4210526315789474, 0.0, 0.026315789473684213, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var53, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        if (input[16] <= 8.294390201568604) {
                            memcpy(var53, (double[]){0.0, 0.0, 0.2857142857142857, 0.2857142857142857, 0.42857142857142855, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var53, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                }
            }
        }
    }
    add_vectors(var18, var53, 8, var17);
    double var54[8];
    if (input[6] <= 488.6999969482422) {
        if (input[16] <= 1.1817535161972046) {
            if (input[1] <= 7.5) {
                memcpy(var54, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
            } else {
                if (input[1] <= 9.5) {
                    memcpy(var54, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                } else {
                    memcpy(var54, (double[]){0.0, 0.0, 0.3333333333333333, 0.0, 0.6666666666666666, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            }
        } else {
            memcpy(var54, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0}, 8 * sizeof(double));
        }
    } else {
        if (input[0] <= 0.5) {
            if (input[16] <= 3.899646282196045) {
                if (input[18] <= 1.4444562792778015) {
                    if (input[9] <= 7837.381103515625) {
                        memcpy(var54, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        if (input[9] <= 9809.111328125) {
                            memcpy(var54, (double[]){0.07407407407407407, 0.9259259259259259, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var54, (double[]){0.0, 0.2, 0.8, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                } else {
                    if (input[16] <= 1.750188410282135) {
                        if (input[16] <= 1.3670865297317505) {
                            memcpy(var54, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var54, (double[]){0.0, 0.75, 0.25, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        memcpy(var54, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                }
            } else {
                if (input[10] <= 40.17142868041992) {
                    memcpy(var54, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                } else {
                    if (input[9] <= 9840.42822265625) {
                        if (input[10] <= 127.48571014404297) {
                            memcpy(var54, (double[]){0.0, 0.0, 0.0, 0.96, 0.04, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var54, (double[]){0.0, 0.0, 0.75, 0.25, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        if (input[11] <= 715.0714111328125) {
                            memcpy(var54, (double[]){0.0, 0.0, 0.0, 0.7, 0.3, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var54, (double[]){0.0, 0.0, 0.0, 0.17647058823529413, 0.8235294117647058, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                }
            }
        } else {
            if (input[16] <= 3.1869221925735474) {
                memcpy(var54, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0}, 8 * sizeof(double));
            } else {
                memcpy(var54, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0}, 8 * sizeof(double));
            }
        }
    }
    add_vectors(var17, var54, 8, var16);
    double var55[8];
    if (input[16] <= 3.4731894731521606) {
        if (input[7] <= 165.83333587646484) {
            if (input[9] <= 8662.0) {
                if (input[13] <= 12.69760513305664) {
                    memcpy(var55, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                } else {
                    memcpy(var55, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0}, 8 * sizeof(double));
                }
            } else {
                if (input[8] <= 1412.0999755859375) {
                    memcpy(var55, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0}, 8 * sizeof(double));
                } else {
                    if (input[6] <= 1434.0750122070312) {
                        memcpy(var55, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        memcpy(var55, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                }
            }
        } else {
            if (input[9] <= 7944.666748046875) {
                if (input[10] <= 1533.7166748046875) {
                    if (input[8] <= 1392.6749877929688) {
                        memcpy(var55, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        if (input[15] <= 39.64872455596924) {
                            memcpy(var55, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var55, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                } else {
                    memcpy(var55, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            } else {
                if (input[12] <= 591.26025390625) {
                    if (input[10] <= 934.1041564941406) {
                        if (input[17] <= 1.2926440238952637) {
                            memcpy(var55, (double[]){0.0, 0.6666666666666666, 0.3333333333333333, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var55, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        if (input[18] <= 1.4623695611953735) {
                            memcpy(var55, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var55, (double[]){0.5, 0.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                } else {
                    if (input[4] <= 2228.4111328125) {
                        memcpy(var55, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        memcpy(var55, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                }
            }
        }
    } else {
        if (input[6] <= 1392.2833251953125) {
            if (input[14] <= 3789.4033203125) {
                if (input[18] <= 1.619277536869049) {
                    memcpy(var55, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0}, 8 * sizeof(double));
                } else {
                    if (input[10] <= 135.45000076293945) {
                        memcpy(var55, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0}, 8 * sizeof(double));
                    } else {
                        memcpy(var55, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                }
            } else {
                if (input[5] <= 127.36666488647461) {
                    memcpy(var55, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                } else {
                    memcpy(var55, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            }
        } else {
            if (input[10] <= 46.54166603088379) {
                if (input[14] <= 5986.956298828125) {
                    if (input[11] <= 1282.4267578125) {
                        if (input[3] <= 2221.300048828125) {
                            memcpy(var55, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var55, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        memcpy(var55, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                } else {
                    if (input[5] <= 137.25) {
                        memcpy(var55, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        memcpy(var55, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                }
            } else {
                if (input[7] <= 164.48571014404297) {
                    if (input[17] <= 1.427508294582367) {
                        memcpy(var55, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        if (input[6] <= 2244.625) {
                            memcpy(var55, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var55, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                } else {
                    if (input[15] <= 63.44840431213379) {
                        if (input[6] <= 3234.738037109375) {
                            memcpy(var55, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var55, (double[]){0.0, 0.0, 0.0, 0.6, 0.4, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        if (input[14] <= 4305.10546875) {
                            memcpy(var55, (double[]){0.0, 0.0, 0.1, 0.8, 0.1, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var55, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                }
            }
        }
    }
    add_vectors(var16, var55, 8, var15);
    double var56[8];
    if (input[14] <= 2877.74609375) {
        if (input[0] <= 0.5) {
            if (input[18] <= 1.3547487258911133) {
                if (input[16] <= 1.2964449524879456) {
                    if (input[8] <= 309.5) {
                        if (input[2] <= 1.5) {
                            memcpy(var56, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var56, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        memcpy(var56, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                } else {
                    if (input[14] <= 204.09158325195312) {
                        memcpy(var56, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        memcpy(var56, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                }
            } else {
                if (input[16] <= 1.153401255607605) {
                    memcpy(var56, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                } else {
                    if (input[18] <= 1.5423136949539185) {
                        if (input[17] <= 1.2760518789291382) {
                            memcpy(var56, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var56, (double[]){0.05660377358490567, 0.7358490566037736, 0.169811320754717, 0.0, 0.03773584905660378, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        if (input[5] <= 420.8809509277344) {
                            memcpy(var56, (double[]){0.0, 0.0, 0.0, 0.6, 0.4, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var56, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                }
            }
        } else {
            if (input[16] <= 3.1869221925735474) {
                if (input[16] <= 2.1303595304489136) {
                    memcpy(var56, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0}, 8 * sizeof(double));
                } else {
                    memcpy(var56, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            } else {
                memcpy(var56, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0}, 8 * sizeof(double));
            }
        }
    } else {
        if (input[14] <= 3908.2552490234375) {
            if (input[7] <= 406.3541717529297) {
                memcpy(var56, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
            } else {
                memcpy(var56, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
            }
        } else {
            if (input[17] <= 1.3801239132881165) {
                if (input[12] <= 247.46038055419922) {
                    if (input[16] <= 8.963386058807373) {
                        memcpy(var56, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        if (input[14] <= 6583.572265625) {
                            memcpy(var56, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var56, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                } else {
                    if (input[15] <= 75.57667541503906) {
                        memcpy(var56, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        memcpy(var56, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                }
            } else {
                if (input[2] <= 3.5) {
                    memcpy(var56, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                } else {
                    if (input[13] <= 1720.7200317382812) {
                        if (input[16] <= 3.9317632913589478) {
                            memcpy(var56, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var56, (double[]){0.0, 0.0, 0.038461538461538464, 0.8461538461538461, 0.11538461538461539, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        if (input[12] <= 250.69947052001953) {
                            memcpy(var56, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var56, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                }
            }
        }
    }
    add_vectors(var15, var56, 8, var14);
    double var57[8];
    if (input[6] <= 480.0500030517578) {
        if (input[16] <= 1.187815010547638) {
            if (input[5] <= 158.5) {
                memcpy(var57, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
            } else {
                memcpy(var57, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
            }
        } else {
            memcpy(var57, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0}, 8 * sizeof(double));
        }
    } else {
        if (input[10] <= 321.3541717529297) {
            if (input[8] <= 1582.8333129882812) {
                if (input[11] <= 768.8507690429688) {
                    if (input[15] <= 67.40741920471191) {
                        memcpy(var57, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0}, 8 * sizeof(double));
                    } else {
                        memcpy(var57, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                } else {
                    if (input[13] <= 885.2603759765625) {
                        memcpy(var57, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        if (input[18] <= 1.5776175260543823) {
                            memcpy(var57, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var57, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                }
            } else {
                if (input[10] <= 46.91666603088379) {
                    if (input[17] <= 1.4604056477546692) {
                        if (input[6] <= 2299.2999267578125) {
                            memcpy(var57, (double[]){0.0, 0.0, 0.0, 0.3333333333333333, 0.6666666666666666, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var57, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        memcpy(var57, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                } else {
                    if (input[1] <= 16.5) {
                        if (input[17] <= 1.4439249634742737) {
                            memcpy(var57, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var57, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        if (input[15] <= 41.84426498413086) {
                            memcpy(var57, (double[]){0.0, 0.0, 0.0, 0.8, 0.2, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var57, (double[]){0.0, 0.0, 0.40625, 0.53125, 0.0625, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                }
            }
        } else {
            if (input[8] <= 1267.9749755859375) {
                if (input[14] <= 342.329833984375) {
                    memcpy(var57, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                } else {
                    memcpy(var57, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            } else {
                if (input[9] <= 8794.91650390625) {
                    if (input[4] <= 981.25) {
                        memcpy(var57, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        if (input[6] <= 2612.72216796875) {
                            memcpy(var57, (double[]){0.9615384615384616, 0.038461538461538464, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var57, (double[]){0.5, 0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                } else {
                    if (input[5] <= 1344.9097290039062) {
                        if (input[12] <= 207.81864166259766) {
                            memcpy(var57, (double[]){0.0, 0.5, 0.4, 0.1, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var57, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        memcpy(var57, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                }
            }
        }
    }
    add_vectors(var14, var57, 8, var13);
    double var58[8];
    if (input[6] <= 467.6750030517578) {
        if (input[16] <= 1.1817535161972046) {
            if (input[18] <= 1.1311686635017395) {
                if (input[2] <= 1.5) {
                    memcpy(var58, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                } else {
                    memcpy(var58, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            } else {
                memcpy(var58, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
            }
        } else {
            memcpy(var58, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0}, 8 * sizeof(double));
        }
    } else {
        if (input[16] <= 2.066505789756775) {
            if (input[10] <= 1508.3713989257812) {
                if (input[7] <= 761.6666564941406) {
                    if (input[8] <= 1464.0) {
                        if (input[5] <= 810.8999938964844) {
                            memcpy(var58, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var58, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        if (input[16] <= 1.3363964557647705) {
                            memcpy(var58, (double[]){0.25, 0.75, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var58, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                } else {
                    if (input[12] <= 813.9352111816406) {
                        memcpy(var58, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        memcpy(var58, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                }
            } else {
                memcpy(var58, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
            }
        } else {
            if (input[0] <= 0.5) {
                if (input[6] <= 2838.857177734375) {
                    if (input[5] <= 437.3333435058594) {
                        if (input[5] <= 150.25) {
                            memcpy(var58, (double[]){0.0, 0.0, 0.3333333333333333, 0.0, 0.6666666666666666, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var58, (double[]){0.0, 0.0, 0.1111111111111111, 0.7037037037037037, 0.18518518518518517, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        if (input[17] <= 1.3180108666419983) {
                            memcpy(var58, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var58, (double[]){0.0, 0.0, 0.9047619047619048, 0.09523809523809523, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                } else {
                    if (input[16] <= 8.271944046020508) {
                        if (input[15] <= 28.0619478225708) {
                            memcpy(var58, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var58, (double[]){0.0, 0.0, 0.45454545454545453, 0.18181818181818182, 0.36363636363636365, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        if (input[15] <= 31.407182693481445) {
                            memcpy(var58, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var58, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                }
            } else {
                if (input[16] <= 3.1869221925735474) {
                    memcpy(var58, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0}, 8 * sizeof(double));
                } else {
                    memcpy(var58, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0}, 8 * sizeof(double));
                }
            }
        }
    }
    add_vectors(var13, var58, 8, var12);
    double var59[8];
    if (input[16] <= 2.1284735202789307) {
        if (input[5] <= 267.1999969482422) {
            if (input[9] <= 2529.375) {
                if (input[2] <= 1.5) {
                    if (input[1] <= 6.5) {
                        memcpy(var59, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        memcpy(var59, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                } else {
                    memcpy(var59, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            } else {
                memcpy(var59, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0}, 8 * sizeof(double));
            }
        } else {
            if (input[11] <= 1464.3469848632812) {
                if (input[12] <= 777.7081604003906) {
                    if (input[10] <= 1270.0142822265625) {
                        if (input[6] <= 1131.4166870117188) {
                            memcpy(var59, (double[]){0.2, 0.0, 0.8, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var59, (double[]){0.0, 0.8235294117647058, 0.17647058823529413, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        if (input[9] <= 8254.60009765625) {
                            memcpy(var59, (double[]){0.96, 0.04, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var59, (double[]){0.3333333333333333, 0.6666666666666666, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                } else {
                    memcpy(var59, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            } else {
                if (input[10] <= 1588.405517578125) {
                    memcpy(var59, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                } else {
                    memcpy(var59, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            }
        }
    } else {
        if (input[1] <= 13.5) {
            if (input[10] <= 333.75) {
                if (input[14] <= 259.3922882080078) {
                    memcpy(var59, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                } else {
                    if (input[6] <= 535.0) {
                        memcpy(var59, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        if (input[9] <= 10309.02490234375) {
                            memcpy(var59, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var59, (double[]){0.0, 0.0, 0.0, 0.0, 0.08333333333333333, 0.0, 0.9166666666666666, 0.0}, 8 * sizeof(double));
                        }
                    }
                }
            } else {
                memcpy(var59, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0}, 8 * sizeof(double));
            }
        } else {
            if (input[16] <= 4.973653316497803) {
                if (input[12] <= 131.83069610595703) {
                    if (input[15] <= 9.311283111572266) {
                        memcpy(var59, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        memcpy(var59, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                } else {
                    if (input[1] <= 31.5) {
                        memcpy(var59, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        if (input[4] <= 2240.25) {
                            memcpy(var59, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var59, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                }
            } else {
                if (input[7] <= 106.96666717529297) {
                    memcpy(var59, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                } else {
                    if (input[3] <= 3442.0833740234375) {
                        if (input[5] <= 352.8541717529297) {
                            memcpy(var59, (double[]){0.0, 0.0, 0.0, 0.2608695652173913, 0.7391304347826086, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var59, (double[]){0.0, 0.0, 0.06666666666666667, 0.9333333333333333, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        if (input[10] <= 160.125) {
                            memcpy(var59, (double[]){0.0, 0.0, 0.058823529411764705, 0.058823529411764705, 0.8823529411764706, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var59, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                }
            }
        }
    }
    add_vectors(var12, var59, 8, var11);
    double var60[8];
    if (input[8] <= 1284.3499755859375) {
        if (input[6] <= 467.6750030517578) {
            if (input[14] <= 232.52616500854492) {
                if (input[10] <= 1178.0) {
                    memcpy(var60, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                } else {
                    memcpy(var60, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            } else {
                if (input[5] <= 327.9499969482422) {
                    memcpy(var60, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0}, 8 * sizeof(double));
                } else {
                    memcpy(var60, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            }
        } else {
            if (input[11] <= 130.39956283569336) {
                memcpy(var60, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0}, 8 * sizeof(double));
            } else {
                if (input[7] <= 142.625) {
                    if (input[14] <= 911.1303100585938) {
                        if (input[15] <= 38.81192588806152) {
                            memcpy(var60, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var60, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        if (input[18] <= 1.6292269825935364) {
                            memcpy(var60, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var60, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                } else {
                    if (input[5] <= 467.6999969482422) {
                        memcpy(var60, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        if (input[6] <= 594.125) {
                            memcpy(var60, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var60, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                }
            }
        }
    } else {
        if (input[10] <= 680.1875) {
            if (input[10] <= 52.904762268066406) {
                if (input[2] <= 5.5) {
                    if (input[3] <= 2333.0) {
                        if (input[11] <= 1286.4502563476562) {
                            memcpy(var60, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var60, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        memcpy(var60, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                } else {
                    memcpy(var60, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            } else {
                if (input[16] <= 3.9317632913589478) {
                    if (input[14] <= 80.83462524414062) {
                        memcpy(var60, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        memcpy(var60, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                } else {
                    if (input[14] <= 4141.011474609375) {
                        if (input[6] <= 1392.2833251953125) {
                            memcpy(var60, (double[]){0.0, 0.0, 0.0, 0.0, 0.14285714285714285, 0.0, 0.8571428571428571, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var60, (double[]){0.0, 0.0, 0.0, 0.5, 0.5, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        if (input[11] <= 1143.2125854492188) {
                            memcpy(var60, (double[]){0.0, 0.0, 0.8333333333333334, 0.16666666666666666, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var60, (double[]){0.0, 0.0, 0.1724137931034483, 0.8275862068965517, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                }
            }
        } else {
            if (input[5] <= 2217.763916015625) {
                if (input[16] <= 1.2622830867767334) {
                    if (input[5] <= 2008.2857055664062) {
                        if (input[7] <= 294.2333221435547) {
                            memcpy(var60, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var60, (double[]){0.9333333333333333, 0.06666666666666667, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        memcpy(var60, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                } else {
                    if (input[5] <= 804.5) {
                        if (input[7] <= 302.47618103027344) {
                            memcpy(var60, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var60, (double[]){0.0, 0.25, 0.75, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        memcpy(var60, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                }
            } else {
                memcpy(var60, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
            }
        }
    }
    add_vectors(var11, var60, 8, var10);
    double var61[8];
    if (input[3] <= 1309.2000122070312) {
        if (input[5] <= 254.1666717529297) {
            if (input[15] <= 78.3807144165039) {
                if (input[7] <= 94.33333206176758) {
                    if (input[17] <= 1.3879131078720093) {
                        if (input[13] <= 450.8737487792969) {
                            memcpy(var61, (double[]){0.0, 0.0, 0.3333333333333333, 0.6666666666666666, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var61, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        memcpy(var61, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                } else {
                    memcpy(var61, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0}, 8 * sizeof(double));
                }
            } else {
                if (input[13] <= 449.7712860107422) {
                    memcpy(var61, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0}, 8 * sizeof(double));
                } else {
                    memcpy(var61, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            }
        } else {
            if (input[16] <= 1.7003360986709595) {
                if (input[7] <= 209.0) {
                    memcpy(var61, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                } else {
                    memcpy(var61, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            } else {
                memcpy(var61, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0}, 8 * sizeof(double));
            }
        }
    } else {
        if (input[16] <= 3.9637885093688965) {
            if (input[9] <= 8199.39990234375) {
                if (input[8] <= 3341.90478515625) {
                    memcpy(var61, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                } else {
                    if (input[7] <= 1180.6339416503906) {
                        memcpy(var61, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        memcpy(var61, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                }
            } else {
                if (input[16] <= 2.1284735202789307) {
                    if (input[5] <= 624.6499938964844) {
                        memcpy(var61, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        if (input[1] <= 31.5) {
                            memcpy(var61, (double[]){0.030303030303030304, 0.9696969696969697, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var61, (double[]){0.0, 0.8461538461538461, 0.15384615384615385, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                } else {
                    if (input[13] <= 280.1448516845703) {
                        memcpy(var61, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        memcpy(var61, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                }
            }
        } else {
            if (input[2] <= 5.5) {
                if (input[4] <= 1774.5) {
                    if (input[14] <= 5291.183837890625) {
                        memcpy(var61, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        if (input[18] <= 1.610511600971222) {
                            memcpy(var61, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var61, (double[]){0.0, 0.0, 0.0, 0.6, 0.4, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                } else {
                    memcpy(var61, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            } else {
                if (input[12] <= 230.70333099365234) {
                    if (input[8] <= 3890.28564453125) {
                        if (input[4] <= 1513.8333740234375) {
                            memcpy(var61, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var61, (double[]){0.0, 0.0, 0.07692307692307693, 0.6153846153846154, 0.3076923076923077, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        memcpy(var61, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                } else {
                    if (input[7] <= 228.27083587646484) {
                        memcpy(var61, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        if (input[1] <= 34.5) {
                            memcpy(var61, (double[]){0.0, 0.0, 0.15384615384615385, 0.6923076923076923, 0.15384615384615385, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var61, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                }
            }
        }
    }
    add_vectors(var10, var61, 8, var9);
    double var62[8];
    if (input[3] <= 1309.2000122070312) {
        if (input[16] <= 3.1869221925735474) {
            if (input[16] <= 2.1303595304489136) {
                if (input[0] <= 0.5) {
                    if (input[15] <= 36.67770862579346) {
                        if (input[11] <= 98.41832733154297) {
                            memcpy(var62, (double[]){0.0, 0.0, 0.2, 0.4, 0.4, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var62, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        memcpy(var62, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                } else {
                    memcpy(var62, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0}, 8 * sizeof(double));
                }
            } else {
                memcpy(var62, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0}, 8 * sizeof(double));
            }
        } else {
            if (input[7] <= 94.33333206176758) {
                if (input[10] <= 150.0) {
                    if (input[16] <= 9.430427551269531) {
                        memcpy(var62, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        memcpy(var62, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                } else {
                    memcpy(var62, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            } else {
                memcpy(var62, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0}, 8 * sizeof(double));
            }
        }
    } else {
        if (input[14] <= 943.2168579101562) {
            if (input[10] <= 1397.2698364257812) {
                if (input[16] <= 2.133555769920349) {
                    if (input[9] <= 8247.8779296875) {
                        if (input[12] <= 700.5747833251953) {
                            memcpy(var62, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var62, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        if (input[10] <= 903.9874877929688) {
                            memcpy(var62, (double[]){0.0, 0.5833333333333334, 0.4166666666666667, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var62, (double[]){0.03333333333333333, 0.9333333333333333, 0.03333333333333333, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                } else {
                    if (input[5] <= 426.3809509277344) {
                        if (input[12] <= 66.0533447265625) {
                            memcpy(var62, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var62, (double[]){0.0, 0.0, 0.0, 0.3333333333333333, 0.6666666666666666, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        if (input[16] <= 2.9097867012023926) {
                            memcpy(var62, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var62, (double[]){0.0, 0.0, 0.6, 0.4, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                }
            } else {
                if (input[12] <= 166.40697479248047) {
                    memcpy(var62, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                } else {
                    memcpy(var62, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            }
        } else {
            if (input[5] <= 426.8095245361328) {
                if (input[9] <= 9771.8330078125) {
                    if (input[8] <= 3875.5) {
                        if (input[15] <= 20.698360443115234) {
                            memcpy(var62, (double[]){0.0, 0.0, 0.0, 0.3333333333333333, 0.6666666666666666, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var62, (double[]){0.0, 0.0, 0.125, 0.875, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        memcpy(var62, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                } else {
                    if (input[15] <= 31.222859382629395) {
                        memcpy(var62, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        if (input[12] <= 114.23931121826172) {
                            memcpy(var62, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var62, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                }
            } else {
                if (input[14] <= 3949.191650390625) {
                    if (input[13] <= 743.0388336181641) {
                        memcpy(var62, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        memcpy(var62, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                } else {
                    if (input[15] <= 146.34658813476562) {
                        if (input[3] <= 3969.0714111328125) {
                            memcpy(var62, (double[]){0.0, 0.0, 0.3333333333333333, 0.6666666666666666, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var62, (double[]){0.0, 0.0, 0.0, 0.25, 0.75, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        memcpy(var62, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                }
            }
        }
    }
    add_vectors(var9, var62, 8, var8);
    double var63[8];
    if (input[0] <= 0.5) {
        if (input[10] <= 321.3541717529297) {
            if (input[12] <= 288.5127716064453) {
                if (input[16] <= 4.58631157875061) {
                    if (input[10] <= 262.9166717529297) {
                        if (input[8] <= 374.5) {
                            memcpy(var63, (double[]){0.0, 0.0, 0.5, 0.0, 0.5, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var63, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        memcpy(var63, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                } else {
                    if (input[10] <= 37.68333435058594) {
                        if (input[6] <= 2250.8499755859375) {
                            memcpy(var63, (double[]){0.0, 0.0, 0.0, 0.42857142857142855, 0.5714285714285714, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var63, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        if (input[14] <= 4373.6015625) {
                            memcpy(var63, (double[]){0.0, 0.0, 0.0, 0.2631578947368421, 0.7368421052631579, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var63, (double[]){0.0, 0.0, 0.14285714285714285, 0.6190476190476191, 0.23809523809523808, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                }
            } else {
                if (input[8] <= 3745.7261962890625) {
                    memcpy(var63, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                } else {
                    memcpy(var63, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            }
        } else {
            if (input[17] <= 1.3655873537063599) {
                if (input[9] <= 6806.84423828125) {
                    memcpy(var63, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                } else {
                    if (input[5] <= 624.6499938964844) {
                        memcpy(var63, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        if (input[10] <= 907.0486145019531) {
                            memcpy(var63, (double[]){0.0, 0.25, 0.75, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var63, (double[]){0.13793103448275862, 0.8620689655172413, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                }
            } else {
                if (input[16] <= 1.3390154242515564) {
                    memcpy(var63, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                } else {
                    if (input[7] <= 500.6319580078125) {
                        if (input[14] <= 80.83462524414062) {
                            memcpy(var63, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var63, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        memcpy(var63, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                }
            }
        }
    } else {
        if (input[9] <= 8662.0) {
            memcpy(var63, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0}, 8 * sizeof(double));
        } else {
            if (input[11] <= 136.84809494018555) {
                memcpy(var63, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0}, 8 * sizeof(double));
            } else {
                if (input[16] <= 3.7330199480056763) {
                    memcpy(var63, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0}, 8 * sizeof(double));
                } else {
                    memcpy(var63, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0}, 8 * sizeof(double));
                }
            }
        }
    }
    add_vectors(var8, var63, 8, var7);
    double var64[8];
    if (input[5] <= 450.05714416503906) {
        if (input[16] <= 3.1827051639556885) {
            if (input[15] <= 54.11503219604492) {
                if (input[15] <= 4.092676162719727) {
                    if (input[1] <= 9.5) {
                        memcpy(var64, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        memcpy(var64, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                } else {
                    memcpy(var64, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            } else {
                if (input[5] <= 294.9499969482422) {
                    memcpy(var64, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0}, 8 * sizeof(double));
                } else {
                    if (input[11] <= 378.07166290283203) {
                        memcpy(var64, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        memcpy(var64, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                }
            }
        } else {
            if (input[0] <= 0.5) {
                if (input[10] <= 162.875) {
                    if (input[14] <= 260.6284866333008) {
                        if (input[13] <= 142.4377784729004) {
                            memcpy(var64, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var64, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        if (input[3] <= 3487.75) {
                            memcpy(var64, (double[]){0.0, 0.0, 0.0, 0.3055555555555556, 0.6944444444444444, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var64, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                } else {
                    if (input[3] <= 1755.8125) {
                        memcpy(var64, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        memcpy(var64, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                }
            } else {
                memcpy(var64, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0}, 8 * sizeof(double));
            }
        }
    } else {
        if (input[10] <= 1272.2444458007812) {
            if (input[10] <= 682.4375) {
                if (input[5] <= 707.5178527832031) {
                    if (input[14] <= 4719.172119140625) {
                        if (input[5] <= 474.27381896972656) {
                            memcpy(var64, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var64, (double[]){0.0, 0.0, 0.0, 0.9, 0.1, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        memcpy(var64, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                } else {
                    memcpy(var64, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            } else {
                if (input[9] <= 9809.111328125) {
                    memcpy(var64, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                } else {
                    if (input[10] <= 811.6428527832031) {
                        memcpy(var64, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        memcpy(var64, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                }
            }
        } else {
            if (input[9] <= 8255.46630859375) {
                memcpy(var64, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
            } else {
                if (input[5] <= 908.6499938964844) {
                    memcpy(var64, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                } else {
                    memcpy(var64, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            }
        }
    }
    add_vectors(var7, var64, 8, var6);
    double var65[8];
    if (input[6] <= 511.5500030517578) {
        if (input[3] <= 732.066650390625) {
            if (input[18] <= 1.2495181560516357) {
                if (input[1] <= 9.5) {
                    if (input[14] <= 193.33326721191406) {
                        if (input[1] <= 7.5) {
                            memcpy(var65, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var65, (double[]){0.0, 0.0, 0.0, 0.5, 0.5, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        memcpy(var65, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0}, 8 * sizeof(double));
                    }
                } else {
                    memcpy(var65, (double[]){0.0, 0.0, 0.8, 0.0, 0.2, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            } else {
                memcpy(var65, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
            }
        } else {
            if (input[17] <= 1.3100243210792542) {
                memcpy(var65, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
            } else {
                memcpy(var65, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0}, 8 * sizeof(double));
            }
        }
    } else {
        if (input[16] <= 6.413812875747681) {
            if (input[12] <= 38.695695877075195) {
                memcpy(var65, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0}, 8 * sizeof(double));
            } else {
                if (input[0] <= 0.5) {
                    if (input[10] <= 1439.625) {
                        if (input[16] <= 1.7974448204040527) {
                            memcpy(var65, (double[]){0.08, 0.92, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var65, (double[]){0.0, 0.13043478260869565, 0.7391304347826086, 0.13043478260869565, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        if (input[6] <= 624.4166564941406) {
                            memcpy(var65, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var65, (double[]){0.9333333333333333, 0.06666666666666667, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                } else {
                    memcpy(var65, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            }
        } else {
            if (input[10] <= 46.54166603088379) {
                if (input[4] <= 1631.3250122070312) {
                    if (input[8] <= 2469.3499755859375) {
                        if (input[14] <= 5626.052490234375) {
                            memcpy(var65, (double[]){0.0, 0.0, 0.0, 0.5, 0.5, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var65, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        memcpy(var65, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                } else {
                    memcpy(var65, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            } else {
                if (input[1] <= 16.5) {
                    memcpy(var65, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                } else {
                    if (input[3] <= 3612.4166259765625) {
                        if (input[6] <= 1422.125) {
                            memcpy(var65, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var65, (double[]){0.0, 0.0, 0.0, 0.9047619047619048, 0.09523809523809523, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        if (input[3] <= 4251.699951171875) {
                            memcpy(var65, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var65, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                }
            }
        }
    }
    add_vectors(var6, var65, 8, var5);
    double var66[8];
    if (input[16] <= 2.1155017614364624) {
        if (input[18] <= 1.2492852807044983) {
            if (input[8] <= 362.375) {
                if (input[2] <= 1.5) {
                    memcpy(var66, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                } else {
                    memcpy(var66, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            } else {
                memcpy(var66, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0}, 8 * sizeof(double));
            }
        } else {
            if (input[9] <= 7851.222412109375) {
                if (input[16] <= 1.1573817133903503) {
                    memcpy(var66, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                } else {
                    if (input[10] <= 1450.9000244140625) {
                        memcpy(var66, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        if (input[15] <= 27.442898750305176) {
                            memcpy(var66, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var66, (double[]){0.16666666666666666, 0.8333333333333334, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                }
            } else {
                if (input[5] <= 804.5) {
                    if (input[3] <= 1527.5999755859375) {
                        memcpy(var66, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        if (input[18] <= 1.5103209018707275) {
                            memcpy(var66, (double[]){0.0, 0.1, 0.9, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var66, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                } else {
                    if (input[15] <= 175.9742660522461) {
                        if (input[9] <= 8459.30810546875) {
                            memcpy(var66, (double[]){0.42857142857142855, 0.5714285714285714, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var66, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        if (input[6] <= 2168.0875244140625) {
                            memcpy(var66, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var66, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                }
            }
        }
    } else {
        if (input[12] <= 38.695695877075195) {
            if (input[5] <= 159.08333587646484) {
                memcpy(var66, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
            } else {
                if (input[3] <= 1305.300048828125) {
                    memcpy(var66, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0}, 8 * sizeof(double));
                } else {
                    memcpy(var66, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            }
        } else {
            if (input[3] <= 1167.36669921875) {
                if (input[17] <= 1.386402428150177) {
                    memcpy(var66, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0}, 8 * sizeof(double));
                } else {
                    if (input[13] <= 851.7123107910156) {
                        memcpy(var66, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        memcpy(var66, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                }
            } else {
                if (input[5] <= 365.7857208251953) {
                    if (input[14] <= 5263.146728515625) {
                        if (input[2] <= 6.5) {
                            memcpy(var66, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var66, (double[]){0.0, 0.0, 0.0, 0.25, 0.75, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        if (input[3] <= 2361.0333251953125) {
                            memcpy(var66, (double[]){0.0, 0.0, 0.1111111111111111, 0.8888888888888888, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var66, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                } else {
                    if (input[16] <= 5.875387191772461) {
                        if (input[11] <= 353.7432556152344) {
                            memcpy(var66, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var66, (double[]){0.0, 0.0, 0.8636363636363636, 0.13636363636363635, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        if (input[12] <= 307.92510986328125) {
                            memcpy(var66, (double[]){0.0, 0.0, 0.09090909090909091, 0.7727272727272727, 0.13636363636363635, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var66, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                }
            }
        }
    }
    add_vectors(var5, var66, 8, var4);
    double var67[8];
    if (input[0] <= 0.5) {
        if (input[18] <= 1.459923803806305) {
            if (input[10] <= 1510.0) {
                if (input[5] <= 840.566650390625) {
                    if (input[14] <= 289.40272521972656) {
                        if (input[14] <= 163.84642028808594) {
                            memcpy(var67, (double[]){0.0, 0.2222222222222222, 0.3333333333333333, 0.3333333333333333, 0.1111111111111111, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var67, (double[]){0.0, 0.0, 0.8333333333333334, 0.16666666666666666, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        if (input[12] <= 150.42039108276367) {
                            memcpy(var67, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var67, (double[]){0.6666666666666666, 0.0, 0.0, 0.0, 0.3333333333333333, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                } else {
                    if (input[9] <= 9677.4375) {
                        if (input[9] <= 8365.0302734375) {
                            memcpy(var67, (double[]){0.5454545454545454, 0.45454545454545453, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var67, (double[]){0.05263157894736842, 0.9473684210526315, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        memcpy(var67, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                }
            } else {
                memcpy(var67, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
            }
        } else {
            if (input[15] <= 90.43233108520508) {
                if (input[10] <= 46.54166603088379) {
                    if (input[4] <= 1488.0416870117188) {
                        if (input[3] <= 1705.0250244140625) {
                            memcpy(var67, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var67, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        memcpy(var67, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                } else {
                    if (input[16] <= 3.741472601890564) {
                        if (input[17] <= 1.3964818120002747) {
                            memcpy(var67, (double[]){0.3333333333333333, 0.3333333333333333, 0.0, 0.3333333333333333, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var67, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        if (input[16] <= 9.064586162567139) {
                            memcpy(var67, (double[]){0.0, 0.0, 0.0, 0.8918918918918919, 0.10810810810810811, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var67, (double[]){0.0, 0.0, 0.0, 0.375, 0.625, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                }
            } else {
                if (input[13] <= 238.76990509033203) {
                    if (input[6] <= 827.8333129882812) {
                        memcpy(var67, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        memcpy(var67, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                } else {
                    if (input[12] <= 575.8831176757812) {
                        if (input[17] <= 1.39711993932724) {
                            memcpy(var67, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var67, (double[]){0.10000000000000002, 0.0, 0.7000000000000001, 0.20000000000000004, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        memcpy(var67, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                }
            }
        }
    } else {
        if (input[5] <= 254.1666717529297) {
            if (input[16] <= 2.7840328216552734) {
                memcpy(var67, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0}, 8 * sizeof(double));
            } else {
                memcpy(var67, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0}, 8 * sizeof(double));
            }
        } else {
            memcpy(var67, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0}, 8 * sizeof(double));
        }
    }
    add_vectors(var4, var67, 8, var3);
    double var68[8];
    if (input[0] <= 0.5) {
        if (input[10] <= 321.3541717529297) {
            if (input[16] <= 8.213507175445557) {
                if (input[4] <= 813.3666687011719) {
                    if (input[17] <= 1.4508041739463806) {
                        if (input[17] <= 1.2064939141273499) {
                            memcpy(var68, (double[]){0.0, 0.0, 0.5, 0.0, 0.5, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var68, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        memcpy(var68, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                } else {
                    if (input[15] <= 73.1324462890625) {
                        if (input[2] <= 6.5) {
                            memcpy(var68, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var68, (double[]){0.0, 0.0, 0.0, 0.7333333333333333, 0.26666666666666666, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        if (input[13] <= 1714.1417236328125) {
                            memcpy(var68, (double[]){0.0, 0.0, 0.42857142857142855, 0.5714285714285714, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var68, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                }
            } else {
                if (input[1] <= 25.5) {
                    if (input[8] <= 2515.0) {
                        if (input[5] <= 137.7750015258789) {
                            memcpy(var68, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var68, (double[]){0.0, 0.0, 0.0, 0.9166666666666666, 0.08333333333333333, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        if (input[9] <= 9943.26806640625) {
                            memcpy(var68, (double[]){0.0, 0.0, 0.0, 0.5, 0.5, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var68, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                } else {
                    if (input[7] <= 131.14286041259766) {
                        memcpy(var68, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        if (input[10] <= 54.071428298950195) {
                            memcpy(var68, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var68, (double[]){0.0, 0.0, 0.0, 0.42857142857142855, 0.5714285714285714, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                }
            }
        } else {
            if (input[9] <= 8199.39990234375) {
                if (input[18] <= 1.389697551727295) {
                    if (input[2] <= 7.5) {
                        if (input[10] <= 1510.0) {
                            memcpy(var68, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var68, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        if (input[16] <= 1.2359124422073364) {
                            memcpy(var68, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var68, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                } else {
                    memcpy(var68, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            } else {
                if (input[12] <= 520.3931274414062) {
                    if (input[8] <= 1929.5119018554688) {
                        if (input[9] <= 9673.29150390625) {
                            memcpy(var68, (double[]){0.0, 0.9166666666666666, 0.08333333333333333, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var68, (double[]){0.0, 0.25, 0.75, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        if (input[6] <= 1865.6607055664062) {
                            memcpy(var68, (double[]){0.125, 0.125, 0.75, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var68, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                } else {
                    if (input[16] <= 1.2622830867767334) {
                        memcpy(var68, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        memcpy(var68, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                }
            }
        }
    } else {
        if (input[18] <= 1.3422484993934631) {
            memcpy(var68, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0}, 8 * sizeof(double));
        } else {
            if (input[10] <= 333.75) {
                memcpy(var68, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0}, 8 * sizeof(double));
            } else {
                memcpy(var68, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0}, 8 * sizeof(double));
            }
        }
    }
    add_vectors(var3, var68, 8, var2);
    double var69[8];
    if (input[3] <= 1309.2000122070312) {
        if (input[6] <= 488.6999969482422) {
            if (input[14] <= 355.22520446777344) {
                if (input[3] <= 314.3333435058594) {
                    if (input[1] <= 6.5) {
                        memcpy(var69, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        if (input[2] <= 1.5) {
                            memcpy(var69, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var69, (double[]){0.0, 0.0, 0.25, 0.5, 0.25, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                } else {
                    memcpy(var69, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            } else {
                if (input[6] <= 391.7250061035156) {
                    memcpy(var69, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0}, 8 * sizeof(double));
                } else {
                    if (input[14] <= 1369.5332641601562) {
                        memcpy(var69, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        memcpy(var69, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0}, 8 * sizeof(double));
                    }
                }
            }
        } else {
            if (input[5] <= 254.1666717529297) {
                if (input[7] <= 109.625) {
                    if (input[2] <= 5.0) {
                        memcpy(var69, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        memcpy(var69, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                } else {
                    if (input[0] <= 0.5) {
                        memcpy(var69, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        memcpy(var69, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0}, 8 * sizeof(double));
                    }
                }
            } else {
                if (input[9] <= 7947.800048828125) {
                    memcpy(var69, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                } else {
                    memcpy(var69, (double[]){0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            }
        }
    } else {
        if (input[5] <= 624.6499938964844) {
            if (input[10] <= 127.48571014404297) {
                if (input[7] <= 146.125) {
                    if (input[12] <= 93.92603302001953) {
                        if (input[9] <= 10728.70849609375) {
                            memcpy(var69, (double[]){0.0, 0.0, 0.0, 0.1111111111111111, 0.8888888888888888, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var69, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        if (input[16] <= 13.349425315856934) {
                            memcpy(var69, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var69, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                } else {
                    if (input[14] <= 4648.834716796875) {
                        if (input[6] <= 3208.3094482421875) {
                            memcpy(var69, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var69, (double[]){0.0, 0.0, 0.23076923076923078, 0.5384615384615384, 0.23076923076923078, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        memcpy(var69, (double[]){0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                }
            } else {
                if (input[13] <= 463.9627227783203) {
                    if (input[16] <= 3.741472601890564) {
                        memcpy(var69, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        memcpy(var69, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                } else {
                    memcpy(var69, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                }
            }
        } else {
            if (input[14] <= 672.5199584960938) {
                if (input[10] <= 1409.5555419921875) {
                    if (input[16] <= 1.956442654132843) {
                        if (input[3] <= 1429.9166870117188) {
                            memcpy(var69, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var69, (double[]){0.027777777777777776, 0.9444444444444444, 0.027777777777777776, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    } else {
                        if (input[10] <= 399.35416412353516) {
                            memcpy(var69, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var69, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                } else {
                    if (input[5] <= 767.4833374023438) {
                        memcpy(var69, (double[]){0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        if (input[16] <= 1.2015577554702759) {
                            memcpy(var69, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        } else {
                            memcpy(var69, (double[]){0.75, 0.25, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                        }
                    }
                }
            } else {
                if (input[15] <= 110.57186889648438) {
                    memcpy(var69, (double[]){0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                } else {
                    if (input[4] <= 1921.1875) {
                        memcpy(var69, (double[]){1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    } else {
                        memcpy(var69, (double[]){0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 8 * sizeof(double));
                    }
                }
            }
        }
    }
    add_vectors(var2, var69, 8, var1);
    mul_vector_number(var1, 0.02857142857142857, 8, var0);
    memcpy(output, var0, 8 * sizeof(double));
}


#ifdef __cplusplus
}

// --- FUNZIONE HELPER C++ PER INFERENZA REAL-TIME ---
/**
 * @brief Esegue l'inferenza e restituisce direttamente l'ID della classe vincente.
 * @param input Array con le 19 feature calcolate in streaming.
 * @return int ID della classe predetta (da 0 a 7).
 */
inline int predict_tomato_class(const double* input) {
    double class_scores[8] = {0.0};
    
    // Chiamata alla funzione di inferenza pura generata da m2cgen
    score(input, class_scores);
    
    // Trova la classe con il punteggio più alto (ArgMax)
    int best_class = 0;
    double max_score = class_scores[0];
    for (int i = 1; i < 8; ++i) {
        if (class_scores[i] > max_score) {
            max_score = class_scores[i];
            best_class = i;
        }
    }
    return best_class;
}
#endif

#endif // TOMATO_CLASSIFIER_H
