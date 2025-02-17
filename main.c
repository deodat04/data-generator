#include <time.h>
#include <stdlib.h>
#include "generate_data.h"
#include "metrics.h"



int main(int argc, char *argv[]) {
    
    srand(time(NULL));

    int nb_elem = atof(argv[1]);
    float disorder_level = atof(argv[2]);

    generate_data(nb_elem, disorder_level, argv[3], argv[4]);

    //srand((unsigned)time(NULL));

    //generate_data(10, 100.0, "Fin", "int");

    //generate_data(15, 70.0, "Fin", "float");

    //generate_data(10, 70.0, "Milieu", "string");

    //generate_data(15, 80.0, "Fin", "string");

    //generate_data(10, 70.0, "Debut", "string");

    //generate_data(10, 80.0, "Aléatoire total", "string");

    //generate_data(10, 70.0, "Debut", "coordinate");

    //generate_data(10, 70.0, "Fin", "date");


    return 0;
}