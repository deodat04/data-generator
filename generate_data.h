#ifndef GENERATEDATA_H
#define GENERATEDATA_H

#include <stddef.h>

typedef struct {
    int x;
    int y;
} Coordinate;


typedef struct {
    double longi;
    double lat;
} GPS;

void generate_data(int num_elements, float disorder_level, const char* disorder_option, const char* data_type);
void shuffle_array(void* array, size_t n, size_t size, float disorder_level);
void shuffle_array_char(char** array, size_t n, float disorder_level);
void shuffle_array_coordinate(Coordinate* array, size_t n, float disorder_level);
void shuffle_array_coordinate_gps(GPS* array, size_t n, float disorder_level);
void apply_disorder_int(int* array, int size, float disorder_level, const char* disorder_option);
void apply_disorder_float(float* array, int size, float disorder_level, const char* disorder_option);
void apply_disorder_char(char** array, int size, float disorder_level, const char* disorder_option);
void apply_disorder_coordinate(Coordinate* array, int size, float disorder_level, const char* disorder_option);
void apply_disorder_date(char** array, int size, float disorder_level, const char* disorder_option);
void apply_disorder_coordinate_gps(GPS* array, int size, float disorder_level, const char* disorder_option);
int is_leap_year(int year);
int days_in_month(int month, int year);
void generate_random_date(int* year, int* month, int* day);
int load_words_from_file(const char* filename, char*** words);
void free_words(char** words, int count);
void rev(int* arr, int n);
void decroissant_float(float* arr, int size);
void decroissant_int(int* arr, int size);
void decroissant_string(char** arr, int size);
void decroissant_coordinate(Coordinate* arr, int size);
void decroissant_coordinate_gps(GPS* arr, int size);
void generateImage(int width, int height, int blocks, const char* filename);
char* encodeBase64(const char* filename);


#endif 
