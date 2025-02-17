#ifndef METRICS_H
#define METRICS_H

#include <stddef.h>

float calculate_permutation_entropy(float* original, float* permuted, int size);
float calculate_permutation_entropy_char(char** original, char** permuted, int size);
double calculate_entropy(int* data, int num_elements);
float calculate_pearson_correlation(float* x, float* y, int n);
float calculate_t_statistic(float r, int n);
int count_inversions(float* arr, int num_elements);

#endif