#include <cstdio>
#include <cstdlib>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
    if(argc != 3) {
        printf("Usage: ./genDimensions file_mode dimensionality\n");
        return EXIT_FAILURE;
    }

    FILE * pFile;
    FILE * pFile2;
    char file_mode = argv[1][0];
    int dimensionality = atoi(argv[2]);
    int index;
    char * actual_dimension = (char *) malloc((dimensionality + 1) * sizeof(char));
    char * tmp_dimension = (char *) malloc((dimensionality + 1) * sizeof(char));

    if(file_mode == 'w') {
        if((pFile = fopen("dimensions", "w")) == NULL ||
           (pFile2 = fopen("dimensions_tmp", "w")) == NULL) {
            return EXIT_FAILURE;
        }

        for(int i = 0; i < dimensionality; i++) {
            fprintf(pFile, "0");
            fprintf(pFile2, "0");
        }

    } else {
        if((pFile = fopen("dimensions", "r")) == NULL || 
           (pFile2 = fopen("dimensions_tmp", "r")) == NULL) {
            return EXIT_FAILURE;
        }        

        fscanf(pFile, "%s", actual_dimension);
        fscanf(pFile2, "%s", tmp_dimension);
        
        fclose(pFile2);
        if((pFile2 = fopen("dimensions_tmp", "w")) == NULL) {
            return EXIT_FAILURE;
        }

        index = -1;
        do {
            for(int i = 0; i < dimensionality; i++) {
                if(tmp_dimension[i] == '1') {
                    index = i;
                }
            }

            if(index == -1) {
                index = dimensionality - 1;
            } else {
                index--;
            }


            for(int i = 0; i < dimensionality; i++) {
                if(index == i || (tmp_dimension[index + 1] == '1' && index == i)) {
                    tmp_dimension[i] = '1';
                } else {
                    tmp_dimension[i] = '0';
                }
            }


        } while(actual_dimension[index] == tmp_dimension[index]);
        // printf("\nindex: %d act: %s tmp: %s\n", index, actual_dimension, tmp_dimension);
        fprintf(pFile2, "%s", tmp_dimension);
    }

    fclose(pFile);
    fclose(pFile2);
    free(actual_dimension);

    return EXIT_SUCCESS;
}