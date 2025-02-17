#include <math.h>
#include "metrics.h"
#include <stdlib.h> 
#include <string.h>
#include <stdio.h>

float calculate_permutation_entropy(float* original, float* permuted, int size) {
    int* position_change = calloc(size, sizeof(int)); 
    int moved_count = 0;

    for (int i = 0; i < size; i++) {
        if (original[i] != permuted[i]) {
            position_change[i] = 1;
            moved_count++;
        }
    }

    //probabilité de déplacement
    float p_moved = (float)moved_count / size;
    float p_not_moved = 1.0 - p_moved;

    //calcul de l'entropie
    float entropy = 0.0;
    if (p_moved > 0) {
        entropy -= p_moved * log2f(p_moved);
    }
    if (p_not_moved > 0) {
        entropy -= p_not_moved * log2f(p_not_moved);
    }

    free(position_change);
    return entropy;
}


float calculate_permutation_entropy_char(char** original, char** permuted, int size) {
    int* position_change = calloc(size, sizeof(int)); 
    int moved_count = 0;

    for (int i = 0; i < size; i++) {
        if (strcmp(original[i], permuted[i]) != 0) { 
            position_change[i] = 1;
            moved_count++;
        }
    }

    float p_moved = (float)moved_count / size;
    float p_not_moved = 1.0 - p_moved;

    float entropy = 0.0;
    if (p_moved > 0) {
        entropy -= p_moved * log2f(p_moved);
    }
    if (p_not_moved > 0) {
        entropy -= p_not_moved * log2f(p_not_moved);
    }

    free(position_change);
    return entropy;
}

double calculate_entropy(int* data, int num_elements) {
    int max_value = 501;
    int* frequency = (int*)calloc(max_value, sizeof(int));
    if (frequency == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire.\n");
        exit(1);
    }

    for (int i = 0; i < num_elements; i++) {
        frequency[data[i]]++;
    }

    double entropy = 0.0;
    for (int i = 0; i < max_value; i++) {
        if (frequency[i] > 0) {
            double p = (double)frequency[i] / num_elements;
            entropy -= p * log2(p);
        }
    }

    free(frequency);
    return entropy;
}

//corrélation de Pearson
float calculate_pearson_correlation(float* x, float* y, int n) {
    if (n <= 1) return 0.0;

    float sum_x = 0, sum_y = 0, sum_xy = 0;
    float sum_x2 = 0, sum_y2 = 0;

    for (int i = 0; i < n; i++) {
        sum_x += x[i];
        sum_y += y[i];
        sum_xy += x[i] * y[i];
        sum_x2 += x[i] * x[i];
        sum_y2 += y[i] * y[i];
    }

    float numerator = (n * sum_xy) - (sum_x * sum_y);
    float denominator = sqrt((n * sum_x2 - sum_x * sum_x) * (n * sum_y2 - sum_y * sum_y));

    if (denominator == 0) return 0;

    return numerator / denominator;
}

//calculer le test statistique t
float calculate_t_statistic(float r, int n) {
    if (n <= 2) {
        return 0;
    }

    return (r * sqrt(n - 2)) / sqrt(1 - r * r);
}

int count_inversions(float* arr, int num_elements) {
    int count = 0;
    for (int i = 0; i < num_elements - 1; i++) {
        for (int j = i + 1; j < num_elements; j++) {
            if (arr[i] > arr[j]) {
                count++;
            }
        }
    }
    return count;
}
