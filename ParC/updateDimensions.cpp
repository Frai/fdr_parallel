#include <cstdio>
#include <cstdlib>
#include <cfloat>
#include <cstring>
#include <cmath>

int main(int argc, char *argv[]) {
    if(argc != 2) {
        printf("Usage: ./updateDimensions dimensionality\n");
        return EXIT_FAILURE;
    }

    int dimensionality = atoi(argv[1]);
    
    FILE * dimensions = fopen("dimensions", "r");
    FILE * result_tmp = fopen("../result_tmp.txt", "r");
    FILE * result = fopen("../result.txt", "r");

    double actual_min;
    double tmp_min;
    double min_calc = 100;

    char * tmp_dimension = (char *) malloc((dimensionality + 1) * sizeof(char));
    char * min_dimension = (char *) malloc((dimensionality + 1) * sizeof(char));
    char * actual_dimension = (char *) malloc((dimensionality + 1) * sizeof(char));
    char * line = (char * ) malloc((2 * dimensionality + 51) * sizeof(char));

    if(dimensions == NULL || result == NULL || result_tmp == NULL) {
        printf("Error opening the file in updateDimensions.\n");
        return EXIT_FAILURE;
    }

    fscanf(dimensions, "%s", min_dimension);
    fclose(dimensions);

    dimensions = fopen("dimensions", "w");

    while(fgets(line, 2 * dimensionality + 50, result) != NULL) {
        while(line[0] != 'D') {
            if(fgets(line, 2 * dimensionality + 50, result) == NULL) {
                break;
            }
        }

        fgets(line, 2 * dimensionality + 50, result);
        sscanf(line, "%lf * x + %*lf, %*lf * x + %*lf", &actual_min);
    }

    fclose(result);
    result = fopen("../result.txt", "a");

    while(fgets(line, 2 * dimensionality + 50, result_tmp) != NULL) {
        while(line[0] != 'D') {
            if(fgets(line, 2 * dimensionality + 50, result_tmp) == NULL) {
                break;
            }
        }

        sscanf(line, "Dimension: %s %s\n", actual_dimension, tmp_dimension);

        fgets(line, 2 * dimensionality + 50, result_tmp);
        sscanf(line, "%lf", &tmp_min);

        if(strcmp(actual_dimension, tmp_dimension) == 0) {
            continue;
        }

        if(fabs(tmp_min - actual_min) <= fabs(min_calc - actual_min)) {
            min_calc = tmp_min;
            strcpy(min_dimension, tmp_dimension);
        }
    }

    for(int i = 0; i < dimensionality; i++) {
        if(actual_dimension[i] == '1') {
            min_dimension[i] = '1';
        }
    }

    fprintf(result, "Dimension: %s %s %s\n%lf\n", min_dimension, tmp_dimension, actual_dimension, min_calc);
    fprintf(dimensions, "%s", min_dimension);

    free(line);
    free(tmp_dimension);
    free(min_dimension);
    free(actual_dimension);

    fclose(dimensions);
    fclose(result);
    fclose(result_tmp);

    return EXIT_SUCCESS;
}