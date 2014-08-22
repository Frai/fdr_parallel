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

// default values
#define NORMALIZE_FACTOR 0 // Independent

int main(int argc, char **argv) {
    int numberOfDimensions = 0;
    int H = 0;
    double alpha = 0.0;
    char num[1000];
    char point[10000];
    char key[1000];
    char databaseName[1000]; 
    char rm_cmd[1000];
    long pointId = 0;

    // reads the input parameters
    FILE *parameters;
    parameters = fopen("parameters", "r");
    fscanf(parameters, "%lf", &alpha);
    fscanf(parameters, "%d", &H);
    fclose(parameters);

    // reads the data dimensionality
    FILE *dimensionality;
    dimensionality = fopen("dimensionality", "r");
    fscanf(dimensionality, "%d", &numberOfDimensions);
    fclose(dimensionality);

    // first validations
    if(H < 2) {
        cout << "Error: MrCC needs at least two resolution levels (H >= 2) to perform the clustering process.";
    } // end if

    // reads objects from the source database
    int numberOfObjects = 0;
    cin >> key; // key from Record reader split
    sprintf(databaseName, "database_%s.dat", key);
    FILE *database = fopen(databaseName, "w");

    while(cin) {
        cin >> num; // pointId discarded

        cin >> point; // first value
        for(int i = 1; i < numberOfDimensions; i++) {
            cin >> num; // point values
            strcat(point, " ");
            strcat(point, num);
        }
        strcat(point, "\n");
        fputs(point, database);

        cin >> num; // discards the ground truth
        numberOfObjects++;
    } // end while

    fclose(database); // closes the database in "w" mode

    if(numberOfObjects) {
        database = fopen(databaseName, "r"); // opens the database in "r" mode

        // creates an object of the class stFDR
        stFDR *sFDR = new stFDR(0, database, numberOfDimensions, NORMALIZE_FACTOR, numberOfObjects, H, 1, 1);

        stCountingTreeMap * pCTree = sFDR->getCalcTree();
        pCTree->printTreeRecursive(pCTree->getRoot(), 0.5);

        fclose(database); // the database file will not be used anymore, thus close it
        delete sFDR; // disposes the used structures
    }
    // } else {
    //     cout << "0\t0\n"; // no point was assigned to this mapper
    // } // end if

    //delete the temporary data file
    // sprintf(rm_cmd, "rm -f %s", databaseName);
    // system(rm_cmd);

    return 0; // success
}