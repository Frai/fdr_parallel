#include <iostream>
#include <string.h>
#include <cstdio>
#include <stdlib.h>
#include <math.h>

using namespace std;

#ifndef __GNUG__
    #include "stFDR.h"
#endif //__GNUG__

#ifdef __GNUG__
    #include "stFDR.cpp"
#endif //__GNUG__

int main(int argc, char **argv) {
    char point[10000]; // one point
    char num[100]; // one value
    int dim = 0; // data dimensionality
    long s = 0; // data size
    int div = 0; // desired number of data divisions
    long pointId = 0; // point identifier

    // reads the data dimensionality
    FILE * dimensionality;
    dimensionality = fopen("dimensionality", "r");
    fscanf(dimensionality, "%d", &dim);
    fclose(dimensionality);

    // reads the data size
    FILE * size;
    size = fopen("size", "r");
    fscanf(size, "%ld", &s);
    fclose(size);

    // reads the desired number of data divisions
    FILE * divisions;
    divisions = fopen("divisions", "r");
    fscanf(divisions, "%d", &div);
    fclose(divisions);

    int numberOfObjects = 0;


    while(cin) { // reads one line per iteration
        cin >> num; // point identifier
        pointId = atol(num);

        cin >> point; // first value
        for(int i = 1; i < dim; i++) {
            cin >> num; // other values
            strcat(point, "_");
            strcat(point, num);
        }

        cin >> num; // discarts the ground truth
        if(cin) { //  outputs the point with the appropriate key
             // file position based data division
            sprintf(num, "%d", ((int) floor(pointId / ceil((double) (s + 1) / div))));
            cout << num << "\t" << point << "\n";
        }

        printf("User: %s - %s\n", num, point);
    }

    return 0; // success
}
