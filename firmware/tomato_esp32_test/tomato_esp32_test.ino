#include "esp_timer.h"

// ==========================================================================
// Dichiarazioni extern "C": le funzioni vere e proprie (score_standard,
// score_cherry, tomato_check_batch_anomaly) vivono in tomato_core.c, 
// compilato con il compilatore C (contengono sintassi C99 non valida in C++).
// Qui in questo file .ino (C++) NON includiamo tomato_classifier.h direttamente.
// ==========================================================================
extern "C" {
    void score_standard(double* input, double* output);
    void score_cherry(double* input, double* output);
    int tomato_check_batch_anomaly(int mode, double transit_len);
}

enum TomatoBatchMode { BATCH_STANDARD = 0, BATCH_CHERRY = 1 };
static const int LABELS_STD[5]    = {0, 1, 2, 3, 4};
static const int LABELS_CHERRY[3] = {5, 6, 7};

int predict_tomato_class(TomatoBatchMode mode, double* input) {
    if (mode == BATCH_STANDARD) {
        double scores[5] = {0};
        score_standard(input, scores);
        int best = 0;
        for (int i = 1; i < 5; i++) if (scores[i] > scores[best]) best = i;
        return LABELS_STD[best];
    } else {
        double scores[3] = {0};
        score_cherry(input, scores);
        int best = 0;
        for (int i = 1; i < 3; i++) if (scores[i] > scores[best]) best = i;
        return LABELS_CHERRY[best];
    }
}

// --- FEATURE ORDER (uguale per entrambi i lotti) ---
// [0]=transit_len, [1]=valid_slices, [2]=IR1_mean, [3]=IR2_mean, [4]=Green_mean, [5]=Red_mean, [6]=Blue_mean, [7]=IR3_mean, [8]=SAT_mean, [9]=HUE_mean, [10]=IR1_std, [11]=Green_std, [12]=Red_std, [13]=SAT_std, [14]=HUE_std, [15]=Red_Green_ratio, [16]=IR1_IR2_ratio, [17]=IR3_IR2_ratio

struct TestCase {
    const char* name;
    TomatoBatchMode mode;
    double features[18];
    int expected_label;
};

// 16 frutti REALI presi da data/processed/tomatoes_features.csv (2 per ciascuna delle 8 classi)
TestCase test_cases[] = {
    {"tomato_0032", BATCH_STANDARD, {24.0, 9.0, 2622.777777777778, 2020.6666666666667, 1786.6666666666667, 1964.4444444444443, 975.6666666666666, 2644.1111111111113, 6336.444444444444, 1689.3333333333333, 1102.6455434292766, 732.2653207683674, 797.7122148856552, 143.84723764389003, 45.828484592008934, 1.0995024814082572, 1.297976458989449, 1.3085340306956965}, 0},
    {"tomato_0021", BATCH_STANDARD, {18.0, 7.0, 1851.5714285714287, 1418.0, 1141.0, 1482.857142857143, 573.0, 1921.2857142857142, 7686.428571428572, 1279.142857142857, 626.9236681720397, 372.92716000134214, 484.77397777088817, 242.49663718826915, 12.850792082313726, 1.2996118578974798, 1.3057626343538804, 1.3549264462175246}, 0},
    {"tomato_0043", BATCH_STANDARD, {24.0, 9.0, 3310.0, 2524.3333333333335, 1621.0, 2795.3333333333335, 626.2222222222222, 3512.1111111111113, 9784.222222222223, 929.3333333333334, 1425.4818483586523, 717.7368946905266, 1167.7885082496744, 300.0365718449077, 33.7860918130523, 1.7244499173897807, 1.3112372851793055, 1.3913024285744433}, 1},
    {"tomato_0069", BATCH_STANDARD, {36.0, 9.0, 3416.1111111111113, 2555.1111111111113, 1714.7777777777778, 2909.0, 754.7777777777778, 3714.777777777778, 9306.555555555555, 929.2222222222222, 1490.398725546661, 772.363706063694, 1337.2650260886958, 361.2105034156368, 59.40281512221828, 1.696429718610855, 1.3369716420104605, 1.453861535447576}, 1},
    {"tomato_0076", BATCH_STANDARD, {30.0, 7.0, 3326.5714285714284, 2358.8571428571427, 716.2857142857143, 2718.8571428571427, 289.42857142857144, 3691.285714285714, 9606.142857142857, 302.14285714285717, 1646.7135813636867, 369.3005615510282, 1373.1108754177417, 4241.730363447266, 149.5275098116604, 3.79577178585879, 1.4102470870447374, 1.5648619119706677}, 2},
    {"tomato_0094", BATCH_STANDARD, {30.0, 7.0, 3470.8571428571427, 2459.285714285714, 714.2857142857143, 2763.0, 303.57142857142856, 3780.8571428571427, 9581.142857142857, 292.2857142857143, 1771.1288705767504, 380.29230612153873, 1548.2962679452964, 4229.89997630249, 148.30791649550636, 3.8681999458452005, 1.4113273250773792, 1.5373801854419624}, 2},
    {"tomato_0117", BATCH_STANDARD, {24.0, 5.0, 1934.8, 1429.4, 227.4, 2008.6, 114.4, 2241.8, 9420.0, 96.8, 1165.6775712005442, 128.2470272559953, 1227.224836776049, 5266.428011470393, 66.51841850194576, 8.832893191165645, 1.353574917073073, 1.5683503458209715}, 3},
    {"tomato_0109", BATCH_STANDARD, {24.0, 5.0, 1961.8, 1414.2, 517.2, 1589.4, 154.0, 2175.4, 11281.6, 506.4, 316.39010730425815, 81.8852856134727, 261.7275300766047, 53.67774212837198, 18.622566955175646, 3.073085787450004, 1.3872153769819304, 1.5382548328506942}, 3},
    {"tomato_0168", BATCH_STANDARD, {30.0, 6.0, 3755.1666666666665, 2676.6666666666665, 286.5, 3911.6666666666665, 194.5, 4347.666666666667, 9906.666666666666, 28.333333333333332, 1908.7031635816677, 153.45455353295972, 2020.898183151904, 4854.0030352964, 13.880441875771341, 13.653286318093556, 1.4029265202879457, 1.6242839291745308}, 4},
    {"tomato_0192", BATCH_STANDARD, {30.0, 6.0, 3799.3333333333335, 2803.0, 403.0, 3664.8333333333335, 221.0, 4220.833333333333, 9797.0, 80.83333333333333, 1975.8776952702985, 216.26927659748623, 1950.7704546323914, 4800.241285602214, 42.00198408012015, 9.093879013385964, 1.3554524865425646, 1.5058270846503967}, 4},
    {"tomato_0271", BATCH_CHERRY, {9.0, 4.0, 1056.75, 812.75, 398.75, 960.5, 164.0, 1195.75, 10272.75, 626.25, 333.17600453814197, 97.35288045729995, 332.77069181845525, 755.9587180086843, 33.86615813266493, 2.4087773690588747, 1.300215302365853, 1.4712396004769044}, 5},
    {"tomato_0274", BATCH_CHERRY, {9.0, 4.0, 1022.5, 763.5, 412.75, 962.0, 142.75, 1191.75, 10525.75, 701.25, 392.4151713003291, 125.7838754901968, 368.92908080912605, 826.2648384950999, 36.10517045152028, 2.330708604949519, 1.3392272254194209, 1.5609037123653737}, 5},
    {"tomato_0279", BATCH_CHERRY, {3.0, 2.0, 545.0, 424.0, 159.5, 566.5, 114.5, 610.5, 10034.0, 210.5, 121.62236636408618, 2.1213203435596424, 60.10407640085654, 350.72496346852756, 23.33452377915607, 3.551723915252419, 1.2853773281750631, 1.439858456607112}, 6},
    {"tomato_0282", BATCH_CHERRY, {6.0, 3.0, 700.6666666666666, 555.6666666666666, 181.0, 705.6666666666666, 143.0, 826.3333333333334, 10029.333333333334, 143.66666666666666, 102.71481554933219, 3.605551275463989, 70.35860525432076, 268.0789684651396, 25.73583752927682, 3.8987106501633155, 1.2609477877453907, 1.48710255272161}, 6},
    {"tomato_0286", BATCH_CHERRY, {12.0, 5.0, 1024.8, 764.8, 191.4, 368.6, 139.4, 887.0, 7350.0, 525.8, 534.6617622385203, 24.31666095499133, 116.77885082496745, 1987.725333138359, 131.58913328994913, 1.9258097217445287, 1.3399581414754427, 1.1597803195635419}, 7},
    {"tomato_0289", BATCH_CHERRY, {12.0, 5.0, 791.8, 589.0, 201.2, 348.2, 150.6, 725.2, 6839.6, 577.0, 340.91303876501996, 20.30270917882636, 91.58984659884523, 1481.1131962142529, 180.39817072243278, 1.7306162161721563, 1.3443123710643061, 1.231239367890673}, 7},
};
const int N_CASES = sizeof(test_cases) / sizeof(test_cases[0]);

void setup() {
    Serial.begin(115200);
    delay(2000);
    Serial.println("=== Tomato Classifier - ESP32 Latency Benchmark ===");
    Serial.println("(replay offline di feature reali, senza sensore ottico fisico)\n");

    const int N_RUNS = 2000;  // ripetizioni per una media stabile
    int correct = 0;
    double total_latency_us = 0;

    for (int t = 0; t < N_CASES; t++) {
        TestCase &tc = test_cases[t];

        // controllo di coerenza lotto/transit_len
        int anomaly = tomato_check_batch_anomaly((int)tc.mode, tc.features[0]);

        // warm-up (prima chiamata esclusa dalla misura)
        predict_tomato_class(tc.mode, tc.features);

        int64_t start = esp_timer_get_time();  // microsecondi
        int pred = 0;
        for (int i = 0; i < N_RUNS; i++) {
            pred = predict_tomato_class(tc.mode, tc.features);
        }
        int64_t elapsed = esp_timer_get_time() - start;
        double avg_us = (double)elapsed / N_RUNS;
        total_latency_us += avg_us;

        bool ok = (pred == tc.expected_label);
        if (ok) correct++;

        Serial.printf("%-14s | predetto=%d atteso=%d %s | anomaly=%d | latenza media=%.2f us (%d run)\n",
            tc.name, pred, tc.expected_label, ok ? "OK" : "MISMATCH", anomaly, avg_us, N_RUNS);
    }

    Serial.println("");
    Serial.printf("Accuratezza sui casi di test: %d/%d\n", correct, N_CASES);
    Serial.printf("Latenza media di inferenza (tutte le classi): %.2f us\n", total_latency_us / N_CASES);
}

void loop() {
    // niente da fare in loop, il benchmark gira una volta in setup()
}