#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "generate_data.h"
#include "metrics.h"
#include <math.h>
#include <stdint.h>


#define MIN_YEAR 2000
#define MAX_YEAR 2025
#define MAX_WORD_LENGTH 100

 
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

//Structure pour stocker une couleur RGBA
typedef struct {
    uint8_t r, g, b, a;
} Color;


int load_words_from_file(const char* filename, char*** words) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Erreur lors de l'ouverture du fichier %s\n", filename);
        return 0;
    }

    int count = 0;
    char word[MAX_WORD_LENGTH];
    while (fscanf(file, "%s", word) != EOF) {
        count++;
    }

    *words = (char**)malloc(count * sizeof(char*));
    fseek(file, 0, SEEK_SET); 

    int index = 0;
    while (fscanf(file, "%s", word) != EOF) {
        (*words)[index] = (char*)malloc((strlen(word) + 1) * sizeof(char));
        strcpy((*words)[index], word);
        index++;
    }

    fclose(file);
    return count;
}

void free_words(char** words, int count) {
    for (int i = 0; i < count; i++) {
        free(words[i]);
    }
    free(words);
}

//vérifier si une année est bissextile
int is_leap_year(int year) {
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
        return 1;
    }
    return 0;
}


//nombre de jours dans un mois donné d'une année
int days_in_month(int month, int year) {
    switch (month) {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            return 31;
        case 4: case 6: case 9: case 11:
            return 30;
        case 2:
            return is_leap_year(year) ? 29 : 28;
        default:
            return 0;
    }
}

void generate_random_date(int* year, int* month, int* day) {
    *year = rand() % (MAX_YEAR - MIN_YEAR + 1) + MIN_YEAR; 
    *month = rand() % 12 + 1; 
    *day = rand() % days_in_month(*month, *year) + 1;
}

void generate_sinusoidal_data(float* data, int num_elements, float a, float b, float h, float k) {
    for (int i = 0; i < num_elements; i++) {
        data[i] = a * sin(b * (i - h)) + k;
    }
}

void generate_data(int num_elements, float disorder_level, const char* disorder_option, const char* data_type) {

    FILE* output_file = fopen("output.csv", "w");
    if (!output_file) {
        printf("Erreur lors de l'ouverture du fichier pour l'\u00e9criture\n");
        return;
    }

    if (strcmp(data_type, "int") == 0) {
        int* data = (int*)malloc(num_elements * sizeof(int));
        int* original_data = (int*)malloc(num_elements * sizeof(int));
        int N = 500;
        for (int i = 0; i < num_elements; i++) {
            data[i] = rand() % (N + 1);
            original_data[i] = data[i];
        }

        printf("Données avant permutation :\n");
        for (int i = 0; i < num_elements; i++) {
            printf("%d ", data[i]);
        }
        printf("\n");

        //Calculer l'entropie avant permutation
        double entropy_before = calculate_entropy(data, num_elements);

        apply_disorder_int(data, num_elements, disorder_level, disorder_option);

        printf("\nGenerated Integers (%s):\n", disorder_option);
        fprintf(output_file, "int\n");

        for (int i = 0; i < num_elements; i++) {
            printf("%d ", data[i]);
            fprintf(output_file, "%d", data[i]);
            if (i < num_elements - 1) {
                fprintf(output_file, "\n");
            }
        }
        printf("\n");
        fprintf(output_file, "\n");

        int num_inversions = count_inversions((float*)data, num_elements);
        int max_inversions = (num_elements * (num_elements - 1)) / 2;
        float disorder_coefficient = (float)num_inversions / max_inversions;

        //Calculer l'entropie après permutation
        double entropy_after = calculate_entropy(data, num_elements);

        float permutation_entropy = calculate_permutation_entropy(
            (float*)original_data, (float*)data, num_elements
        );

        printf("Entropie par permutation : %.4f\n", permutation_entropy);

        printf("\nEntropie avant permutation : %.4f\n", entropy_before);
        printf("Entropie après permutation : %.4f\n", entropy_after);
        printf("Variation d'entropie : %.4f\n", entropy_after - entropy_before);

        printf("Nombre d'inversions : %d\n", num_inversions);
        printf("Coefficient de désordre (D) : %.4f\n", disorder_coefficient);

        free(data);
        free(original_data);

    } else if (strcmp(data_type, "float") == 0) {
        float* data = (float*)malloc(num_elements * sizeof(float));
        float* original_data = (float*)malloc(num_elements * sizeof(float));
        int N = 500;
        for (int i = 0; i < num_elements; i++) {
            data[i] = (float)(rand() % (N + 1)) / 10;
            original_data[i] = data[i];
        }

        printf("Données avant permutation :\n");
        for (int i = 0; i < num_elements; i++) {
            printf("%.2f ", data[i]);
        }
        printf("\n");

        //Calculer l'entropie avant permutation
        //double entropy_before = calculate_entropy(data, num_elements);

        apply_disorder_float(data, num_elements, disorder_level, disorder_option);

        printf("\nGenerated Floats (%s):\n", disorder_option);
        fprintf(output_file, "float\n");

        for (int i = 0; i < num_elements; i++) {
            printf("%.2f ", data[i]);
            fprintf(output_file, "%.2f", data[i]);
            if (i < num_elements - 1) {
                fprintf(output_file, "\n");
            }
        }
        printf("\n");
        fprintf(output_file, "\n");

        //Calculer l'entropie après permutation
        //double entropy_after = calculate_entropy(data, num_elements);

        int num_inversions = count_inversions(data, num_elements);
        int max_inversions = (num_elements * (num_elements - 1)) / 2;
        float disorder_coefficient = (float)num_inversions / max_inversions;

        float permutation_entropy = calculate_permutation_entropy(
            original_data, data, num_elements
        );

        printf("\n");
        printf("Entropie par permutation : %.4f\n", permutation_entropy);

        printf("Nombre d'inversions : %d\n", num_inversions);
        printf("Coefficient de désordre (D) : %.4f\n", disorder_coefficient);

        /* printf("\nEntropie avant permutation : %.4f\n", entropy_before);
        printf("Entropie après permutation : %.4f\n", entropy_after);
        printf("Variation d'entropie : %.4f\n", entropy_after - entropy_before); */

        free(data);
        free(original_data);
    } else if (strcmp(data_type, "string") == 0) {
        char** words = NULL;
        int num_words = load_words_from_file("liste.de.mots.francais.frgut.txt", &words);
        if (num_words == 0) {
            printf("Aucun mot n'a pu être chargé.\n");
            return;
        }

        if (num_elements > num_words) {
            printf("Nombre d'éléments demandé dépasse le nombre de mots disponibles.\n");
            num_elements = num_words; 
        }

        char** data = (char**)malloc(num_elements * sizeof(char*));
        char** original_data = (char**)malloc(num_elements * sizeof(char*));
        for (int i = 0; i < num_elements; i++) {
            int random_index = rand() % num_words;
            data[i] = (char*)malloc((strlen(words[random_index]) + 1) * sizeof(char));
            strcpy(data[i], words[random_index]);
            original_data[i] = data[i]; 
        }

        printf("Données avant permutation :\n");
        for (int i = 0; i < num_elements; i++) {
            printf("%s ", data[i]);
        }
        printf("\n");

        apply_disorder_char(data, num_elements, disorder_level, disorder_option);

        printf("\nGenerated Strings (%s):\n", disorder_option);
        fprintf(output_file, "string\n");

        for (int i = 0; i < num_elements; i++) {
            printf("%s ", data[i]);
            fprintf(output_file, "%s\n", data[i]);
        }
        printf("\n");

        float permutation_entropy = calculate_permutation_entropy_char(original_data, data, num_elements);
        printf("Entropie par permutation : %.4f\n", permutation_entropy);


        free_words(words, num_words);
        for (int i = 0; i < num_elements; i++) {
            free(data[i]);
        }
        free(data);
        free(original_data);
    } else if (strcmp(data_type, "coordinate") == 0) {
        Coordinate* data = (Coordinate*)malloc(num_elements * sizeof(Coordinate));
        Coordinate* original_data = (Coordinate*)malloc(num_elements * sizeof(Coordinate));
        int N = 10;

         for (int i = 0; i < num_elements; i++) {
            data[i].x = rand() % (N + 1);  
            data[i].y = rand() % (N + 1); 
            original_data[i] = data[i];
        }

         printf("Données avant permutation :\n");
        for (int i = 0; i < num_elements; i++) {
            printf("(%d, %d) ", data[i].x, data[i].y);
        }
        printf("\n");

        apply_disorder_coordinate(data, num_elements, disorder_level, disorder_option);

        printf("\nGenerated Coordinates (%s):\n", disorder_option);
        fprintf(output_file, "coordonnees\n");

        for (int i = 0; i < num_elements; i++) {
            printf("(%d, %d) ", data[i].x, data[i].y);
            fprintf(output_file, "(%d,%d)", data[i].x, data[i].y);
            if (i < num_elements - 1) {
                fprintf(output_file, "\n");
            }
        }
        printf("\n");
        fprintf(output_file, "\n");

        int num_inversions = count_inversions((float*)data, num_elements);
        int max_inversions = (num_elements * (num_elements - 1)) / 2;
        float disorder_coefficient = (float)num_inversions / max_inversions;

        float permutation_entropy = calculate_permutation_entropy(
            (float*)original_data, (float*)data, num_elements
        );

        printf("Entropie par permutation : %.4f\n", permutation_entropy);

        printf("Nombre d'inversions : %d\n", num_inversions);
        printf("Coefficient de désordre (D) : %.4f\n", disorder_coefficient);

        free(data);
        free(original_data);

    } else if (strcmp(data_type, "date") == 0) {
        char** data = (char**)malloc(num_elements * sizeof(char*));
        char** original_data = (char**)malloc(num_elements * sizeof(char*));

        for (int i = 0; i < num_elements; i++) {
            int year, month, day;
            generate_random_date(&year, &month, &day);

            data[i] = (char*)malloc(11 * sizeof(char)); 
            sprintf(data[i], "%04d-%02d-%02d", year, month, day);

            original_data[i] = data[i];  
        }

        printf("Données avant permutation :\n");
        for (int i = 0; i < num_elements; i++) {
            printf("%s ", data[i]);
        }
        printf("\n");

        apply_disorder_char(data, num_elements, disorder_level, disorder_option);

        printf("\nGenerated Dates (%s):\n", disorder_option);
        fprintf(output_file, "date\n");

        for (int i = 0; i < num_elements; i++) {
            printf("%s ", data[i]);
            fprintf(output_file, "%s\n", data[i]);
        }
        printf("\n");

        float permutation_entropy = calculate_permutation_entropy_char(
            original_data, data, num_elements
        );

        printf("Entropie par permutation : %.4f\n", permutation_entropy);

        for (int i = 0; i < num_elements; i++) {
            free(data[i]);
        }
        free(data);
        free(original_data);

    } else if(strcmp(data_type, "gps") == 0) {
        GPS* data = (GPS*)malloc(num_elements * sizeof(GPS));
        GPS* original_data = (GPS*)malloc(num_elements * sizeof(GPS));

        double longIni = 2.294481;
        double latIni = 48.858370;
        double radius = 2;

        double xMin = longIni - radius;
        double xMax = longIni + radius;
        double xRange = xMax - xMin;

        for (int i = 0; i < num_elements; i++) {
            data[i].longi = xMin + (rand() / (double)RAND_MAX) * xRange;
            double xOffset = fabs(data[i].longi - longIni);
            if (xOffset > radius) {
                xOffset = radius; 
            }
            double yDelta = sqrt(radius * radius - xOffset * xOffset);
            double yMin = latIni - yDelta;
            double yMax = latIni + yDelta;
            double yRange = yMax - yMin;
    
            data[i].lat = yMin + (rand() / (double)RAND_MAX) * yRange;
            original_data[i] = data[i];
        }

        printf("Données avant permutation :\n");
        for (int i = 0; i < num_elements; i++) {
            printf("(%f, %f) ", data[i].longi, data[i].lat);
        }
        printf("\n");

        apply_disorder_coordinate_gps(data, num_elements, disorder_level, disorder_option);

        printf("\nGenerated Coordinates GPS (%s):\n", disorder_option);
        fprintf(output_file, "gps\n");

        for (int i = 0; i < num_elements; i++) {
            printf("(%f, %f) ", data[i].longi, data[i].lat);
            fprintf(output_file, "(%f,%f)", data[i].longi, data[i].lat);
            if (i < num_elements - 1) {
                fprintf(output_file, "\n");
            }
        }
        printf("\n");
        fprintf(output_file, "\n");

        int num_inversions = count_inversions((float*)data, num_elements);
        int max_inversions = (num_elements * (num_elements - 1)) / 2;
        float disorder_coefficient = (float)num_inversions / max_inversions;

        float permutation_entropy = calculate_permutation_entropy(
            (float*)original_data, (float*)data, num_elements
        );

        printf("Entropie par permutation : %.4f\n", permutation_entropy);

        printf("Nombre d'inversions : %d\n", num_inversions);
        printf("Coefficient de désordre (D) : %.4f\n", disorder_coefficient);

    } else if(strcmp(data_type, "image") == 0) {
        char** data = (char**)malloc(num_elements * sizeof(char*));
        char** original_data = (char**)malloc(num_elements * sizeof(char*));
    
        int width = 32;
        int height = 4;
        int blocks = 4;
    
        for (int i = 0; i < num_elements; i++) {
            char filename[30];
            char command[256];
            sprintf(filename, "output_%d.png", i);
            generateImage(width, height, blocks, filename);
            data[i] = encodeBase64(filename);
            original_data[i] = strdup(data[i]);

            snprintf(command, sizeof(command), "rm -f %s", filename);
            system(command);
        }
    
        printf("Données avant permutation :\n");
        for (int i = 0; i < num_elements; i++) {
            printf("%s\n", data[i]);
        }
        printf("\n");
    
        apply_disorder_char(data, num_elements, disorder_level, disorder_option);
    
        printf("\nGenerated base64 images (%s):\n", disorder_option);
        fprintf(output_file, "image\n");
    
        for (int i = 0; i < num_elements; i++) {
            printf("%s\n", data[i]);
            fprintf(output_file, "%s\n", data[i]);
        }
        printf("\n");
    
        float permutation_entropy = calculate_permutation_entropy_char(original_data, data, num_elements);
        printf("Entropie par permutation : %.4f\n", permutation_entropy);
    
        for (int i = 0; i < num_elements; i++) {
            free(data[i]);
            free(original_data[i]);
        }
        free(data);
        free(original_data);

    } else if (strcmp(data_type, "sin") == 0) {
        float* data = (float*)malloc(num_elements * sizeof(float));
        float* original_data = (float*)malloc(num_elements * sizeof(float));

        float a = 10.0, b = 0.1, h = 0, k = 0;
        generate_sinusoidal_data(data, num_elements, a, b, h, k);
        
        for (int i = 0; i < num_elements; i++) {
            original_data[i] = data[i];
        }

        printf("Données sinusoidales avant permutation :\n");
        for (int i = 0; i < num_elements; i++) {
            printf("%.2f ", data[i]);
        }
        printf("\n");

        apply_disorder_float(data, num_elements, disorder_level, disorder_option);

        printf("\nGenerated Sinusoidal Data (%s):\n", disorder_option);
        fprintf(output_file, "sin\n");

        for (int i = 0; i < num_elements; i++) {
            printf("%.2f ", data[i]);
            fprintf(output_file, "%.2f", data[i]);
            if (i < num_elements - 1) {
                fprintf(output_file, "\n");
            }
        }
        printf("\n");
        fprintf(output_file, "\n");

        //Calcul du coefficient de Pearson entre original_data et data
        float pearson_coefficient = calculate_pearson_correlation(original_data, data, num_elements);
        printf("Coefficient de Pearson : %.4f\n", pearson_coefficient);

        //Calcul du test statistique t
        float t_statistic = calculate_t_statistic(pearson_coefficient, num_elements);
        printf("Valeur du test t : %.4f\n", t_statistic);

        free(data);
        free(original_data);
    }
      else {
        printf("Invalid data type. You must use 'int', 'float', 'string', 'coordinate' or 'date'.\n");
    }

    fclose(output_file);
}


//fonction permettant d'appliquer un désordre au tableau de donnnees
//par defaut l'option de desordre est "aléatoire total"
//fonctionne juste sur les int et les floats
void shuffle_array(void* array, size_t n, size_t size, float disorder_level) {
    char* arr = (char*)array;
    size_t disorder_limit = (size_t)(n * (disorder_level / 100.0));
    for (size_t i = 0; i < disorder_limit; i++) {
        size_t j = rand() % n;
        size_t k = rand() % n;
        for (size_t byte = 0; byte < size; byte++) {
            char temp = arr[j * size + byte];
            arr[j * size + byte] = arr[k * size + byte];
            arr[k * size + byte] = temp;
        }
    }
}

void shuffle_array_char(char** array, size_t n, float disorder_level) {
    size_t disorder_limit = (size_t)(n * (disorder_level / 100.0));
    for (size_t i = 0; i < disorder_limit; i++) {
        size_t j = rand() % n; 
        size_t k = rand() % n;

        char* temp = array[j];
        array[j] = array[k];
        array[k] = temp;
    }
}

void shuffle_array_coordinate(Coordinate* array, size_t n, float disorder_level) {
    size_t disorder_limit = (size_t)(n * (disorder_level / 100.0));
    for (size_t i = 0; i < disorder_limit; i++) {
        size_t j = rand() % n; 
        size_t k = rand() % n; 

        Coordinate temp = array[j];
        array[j] = array[k];
        array[k] = temp;
    }
}

void shuffle_array_coordinate_gps(GPS* array, size_t n, float disorder_level) {
    size_t disorder_limit = (size_t)(n * (disorder_level / 100.0));
    for (size_t i = 0; i < disorder_limit; i++) {
        size_t j = rand() % n; 
        size_t k = rand() % n; 

        GPS temp = array[j];
        array[j] = array[k];
        array[k] = temp;
    }
}

void rev(int* arr, int n) {
    int l = 0, r = n - 1;
    while (l < r) {
        int temp = arr[l];
        arr[l] = arr[r];
        arr[r] = temp;
        l++;
        r--;
    }
}

void decroissant_float(float* arr, int size) {
    for (int j = 0; j < size; j++){
        for (int i = j+1; i < size; i++){
            if (arr[j] < arr[i]) {
                float temp = arr[j];
                arr[j] = arr[i];
                arr[i] = temp;
            }
        }
    }
}


void decroissant_int(int* arr, int size) {
    for (int j = 0; j < size; j++){
        for (int i = j+1; i < size; i++){
            if (arr[j] < arr[i]) {
                int temp = arr[j];
                arr[j] = arr[i];
                arr[i] = temp;
            }
        }
    }
}


void decroissant_string(char** arr, int size) {
    for (int j = 0; j < size; j++) {
        for (int i = j + 1; i < size; i++) {
            if (strcmp(arr[j], arr[i]) < 0) {
                char* temp = arr[j];
                arr[j] = arr[i];
                arr[i] = temp;
            }
        }
    }
}


void decroissant_coordinate(Coordinate* arr, int size) {
    for (int j = 0; j < size; j++) {
        for (int i = j + 1; i < size; i++) {
            if (arr[j].x < arr[i].x || (arr[j].x == arr[i].x && arr[j].y < arr[i].y)) {
                Coordinate temp = arr[j];
                arr[j] = arr[i];
                arr[i] = temp;
            }
        }
    }
}

void decroissant_coordinate_gps(GPS* arr, int size) {
    for (int j = 0; j < size; j++) {
        for (int i = j + 1; i < size; i++) {
            if (arr[j].longi < arr[i].longi || (arr[j].longi == arr[i].longi && arr[j].lat < arr[i].lat)) {
                GPS temp = arr[j];
                arr[j] = arr[i];
                arr[i] = temp;
            }
        }
    }
}


//fonction desordre to int 
void apply_disorder_int(int* array, int size, float disorder_level, const char* disorder_option) {
    if (strcmp(disorder_option, "Aléatoire total") == 0) {
        shuffle_array(array, size, sizeof(int), disorder_level);
    } else if (strcmp(disorder_option, "Debut") == 0) { 
        int start_limit = (int)(size * (disorder_level / 100.0)); //calcul du nombres affectés ; proportionnels au niveau de désordre.
        shuffle_array(array, start_limit, sizeof(int), 100.0); //desordre debut du segment
    } else if (strcmp(disorder_option, "Milieu") == 0) { 
        int start = (size / 4);
        int end = (3 * size / 4);   
        shuffle_array(array + start, end - start, sizeof(int), 100.0); //desordre in middle 3/4
    } else if (strcmp(disorder_option, "Fin") == 0) {
        int start = (int)(size * (1.0 - (disorder_level / 100.0)));
        shuffle_array(array + start, size - start, sizeof(int), 100.0); //desordre end of segment
    } else if (strcmp(disorder_option, "Reverse") == 0){
        //rev(array, size);
        decroissant_int(array, size);
    }
}

//fonction desordre to float
void apply_disorder_float(float* array, int size, float disorder_level, const char* disorder_option) {
    if (strcmp(disorder_option, "Aléatoire total") == 0) {
        shuffle_array(array, size, sizeof(float), disorder_level);
    } else if (strcmp(disorder_option, "Debut") == 0) {
        int start_limit = (int)(size * (disorder_level / 100.0));
        shuffle_array(array, start_limit, sizeof(float), 100.0);
    } else if (strcmp(disorder_option, "Milieu") == 0) {
        int start = (size / 4);
        int end = (3 * size / 4);
        shuffle_array(array + start, end - start, sizeof(float), 100.0);
    } else if (strcmp(disorder_option, "Fin") == 0) {
        int start = (int)(size * (1.0 - (disorder_level / 100.0)));
        shuffle_array(array + start, size - start, sizeof(float), 100.0);
    } else if (strcmp(disorder_option, "Reverse") == 0){
        //rev(array, size);
        decroissant_float(array, size);
    }
}


void apply_disorder_char(char** array, int size, float disorder_level, const char* disorder_option) {
    if (strcmp(disorder_option, "Aléatoire total") == 0) {
        shuffle_array_char(array, size, disorder_level);
    } else if (strcmp(disorder_option, "Debut") == 0) {
        int start_limit = (int)(size * (disorder_level / 100.0));
        shuffle_array_char(array, start_limit, 100.0);
    } else if (strcmp(disorder_option, "Milieu") == 0) {
        int start = size / 4;
        int end = (3 * size) / 4;
        shuffle_array_char(array + start, end - start, 100.0);
    } else if (strcmp(disorder_option, "Fin") == 0) {
        int start = (int)(size * (1.0 - (disorder_level / 100.0)));
        shuffle_array_char(array + start, size - start, 100.0);
    } else if (strcmp(disorder_option, "Reverse") == 0){
        decroissant_string(array, size);
    }
}

//appliquer un désordre sur un tableau de coordonnées
void apply_disorder_coordinate(Coordinate* array, int size, float disorder_level, const char* disorder_option) {
    if (strcmp(disorder_option, "Aléatoire total") == 0) {
        shuffle_array_coordinate(array, size, disorder_level);
    } else if (strcmp(disorder_option, "Debut") == 0) {
        int start_limit = (int)(size * (disorder_level / 100.0));
        shuffle_array_coordinate(array, start_limit, 100.0);
    } else if (strcmp(disorder_option, "Milieu") == 0) {
        int start = size / 4;
        int end = (3 * size) / 4;
        shuffle_array_coordinate(array + start, end - start, 100.0);
    } else if (strcmp(disorder_option, "Fin") == 0) {
        int start = (int)(size * (1.0 - (disorder_level / 100.0)));
        shuffle_array_coordinate(array + start, size - start, 100.0);
    } else if (strcmp(disorder_option, "Reverse") == 0){
        decroissant_coordinate(array, size);
    }
}

void apply_disorder_coordinate_gps(GPS* array, int size, float disorder_level, const char* disorder_option) {
    if (strcmp(disorder_option, "Aléatoire total") == 0) {
        shuffle_array_coordinate_gps(array, size, disorder_level);
    } else if (strcmp(disorder_option, "Debut") == 0) {
        int start_limit = (int)(size * (disorder_level / 100.0));
        shuffle_array_coordinate_gps(array, start_limit, 100.0);
    } else if (strcmp(disorder_option, "Milieu") == 0) {
        int start = size / 4;
        int end = (3 * size) / 4;
        shuffle_array_coordinate_gps(array + start, end - start, 100.0);
    } else if (strcmp(disorder_option, "Fin") == 0) {
        int start = (int)(size * (1.0 - (disorder_level / 100.0)));
        shuffle_array_coordinate_gps(array + start, size - start, 100.0);
    } else if (strcmp(disorder_option, "Reverse") == 0){
        decroissant_coordinate_gps(array, size);
    }
}

 //Génère une couleur aléatoire
 Color randomColor() {
    Color c;
    c.r = rand() % 256;
    c.g = rand() % 256;
    c.b = rand() % 256;
    c.a = 255;
    return c;
}

//Génère une image de dimensions width x height avec des blocs de couleurs
void generateImage(int width, int height, int blocks, const char* filename) {
    int cellSizeX = width / blocks;
    int cellSizeY = height / blocks;
    
    //Création d'un tableau de couleurs
    Color colors[blocks][blocks];
    for (int i = 0; i < blocks; i++) {
        for (int j = 0; j < blocks; j++) {
            colors[i][j] = randomColor();
        }
    }

    uint8_t *image = (uint8_t*)malloc(width * height * 4);
    if (!image) {
        printf("Erreur d'allocation mémoire\n");
        exit(1);
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int xx = x / cellSizeX;
            int yy = y / cellSizeY;
            Color c = colors[xx][yy];

            int index = (y * width + x) * 4;
            image[index] = c.r;
            image[index + 1] = c.g;
            image[index + 2] = c.b;
            image[index + 3] = c.a;
        }
    }

    //Sauvegarde de l'image en PNG
    if (!stbi_write_png(filename, width, height, 4, image, width * 4)) {
        printf("Erreur lors de la création de l'image PNG\n");
        free(image);
        exit(1);
    }

    printf("Image générée et supprimée: %s\n", filename);
    free(image);
}

//Convertit un fichier en base64
char* encodeBase64(const char* filename) {
    FILE* fp;
    char command[256];
    snprintf(command, sizeof(command), "openssl base64 -A -in %s", filename);
    
    fp = popen(command, "r");
    if (!fp) {
        perror("Erreur lors de l'ouverture du pipe");
        return NULL;
    }
    
    char* result = (char*)malloc(1024); 
    if (!result) {
        perror("Erreur d'allocation mémoire");
        fclose(fp);
        return NULL;
    }

    size_t length = 0;
    while (fgets(result + length, 1024 - length, fp)) {
        length += strlen(result + length);
    }

    fclose(fp);

    return result;
}