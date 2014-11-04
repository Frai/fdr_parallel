#include <stdio.h>
#include <string.h>
#include <string>
#include <cstdlib>
#include <time.h>
#include <iostream>
#include <math.h>
#include <map>
#include <vector>

using namespace std;

#ifndef __GNUG__
	#include "stFDR.h"
#endif //__GNUG__

#ifdef __GNUG__
	#include "stFDR.cpp"
#endif //__GNUG__

// default values
#define NORMALIZE_FACTOR 0 // Independent

// global variables
clock_t startTime;

/**
 * Initiates the measurement of run time.
 */
void initClock() {
	startTime=clock();
}//end initClock

/**
 * Prints the elapsed time.
 */
void printElapsed() {
	printf("Elapsed time: %0.3lf sec.\n", (double) (clock() - startTime) / CLOCKS_PER_SEC);
}//end printElapsed

int stringToInt(string input, int numberOfDimensions) {
	unsigned int res = 0;

	for(int i = input.length() - 1; i >= 0; i--) {
		if(input[i] == '1') {
			res += pow(2, numberOfDimensions - i - 1);
		}
	}

	return res;
}

/**
 * Initiates the clustering process.
 *
 * @param argc Number of arguments received.
 * @param argv Array with the arguments received.
 * @return success (0), error (1).
 */
int main(int argc, char **argv) {
	int H = 0;
	int numberOfDimensions = 0;
	int actualNumberOfDimensions = 0;
	double alpha = 0;

	string cellId;
	string tmpCellId;
	string key;
	double sum;
	double len;
	double level;

	map<string, double> m;
	map<string, double>::iterator it;

	map<double, double> calcLog;
	map<double, double>::iterator it2;

	// reads the input parameters
	FILE *parameters;
	parameters=fopen("parameters", "r");
	fscanf(parameters,"%lf",&alpha);
	fscanf(parameters,"%d",&H);
	fclose(parameters);

	// reads the data dimensionality
	FILE *dimensionality;
	dimensionality=fopen("dimensionality", "r");
	fscanf(dimensionality,"%d",&numberOfDimensions);
	fclose(dimensionality);

	actualNumberOfDimensions = numberOfDimensions;

	FILE * dimensions_file = fopen("dimensions", "r");
    char * dimensions = (char *) malloc((numberOfDimensions + 1) * sizeof(char));
    fscanf(dimensions_file, "%s", dimensions);

    FILE * dimensions_file_tmp = fopen("dimensions_tmp", "r");
    char * dimensions_tmp = (char *) malloc((numberOfDimensions + 1) * sizeof(char));
    fscanf(dimensions_file_tmp, "%s", dimensions_tmp);

	for(int i = 0; i < numberOfDimensions; i++) {
        if(dimensions[i] == '1' || dimensions_tmp[i] == '1') {
            actualNumberOfDimensions--;
        }
    }

	if(H < 2) { // first validations
		cout << "Error: MrCC needs at least two resolution levels (H >= 2) to perform the clustering process.";
	} // end if

	while(cin) {
		cin >> cellId;
		cin >> sum;

		if(m.find(cellId) == m.end()) {
			m[cellId] = sum;
		} else {
			m[cellId] = m[cellId] + sum;
		}
	} // end while

	m[cellId] = m[cellId] - sum;

	for(it = m.begin(); it != m.end(); it++) {
		len = (it->first).length();
		if(it->second > 1) {
			if(calcLog.find(len) == calcLog.end()) {
				calcLog[len] = (it->second * (it->second - 1.0) / 2.0);
			} else {
				calcLog[len] = calcLog[len] + (it->second * (it->second - 1.0) / 2.0);
			}
		}
	}

	for(it2 = calcLog.begin(); it2 != calcLog.end(); it2++) {
		cout << log(1.0 / pow(2, (it2->first / actualNumberOfDimensions)));
		cout << " " << it2->second << "\n";
	}

	free(dimensions);
    fclose(dimensions_file);
    free(dimensions_tmp);
    fclose(dimensions_file_tmp);

	return 0; // success
}
