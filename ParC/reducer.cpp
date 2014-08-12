#include <string.h>
#include <time.h>
#include <iostream>
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

/**
 * Initiates the clustering process.
 *
 * @param argc Number of arguments received.
 * @param argv Array with the arguments received.
 * @return success (0), error (1).
 */
int main(int argc, char **argv) {
	int H = 0, numberOfDimensions = 0;
	double alpha = 0;
	char point[10000], key[1000], databaseName[1000], rm_cmd[1000];

	char cellId[10000];
	int sum;

	// initClock(); // initiates the meassurement of run time

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

	// first validations
	if(H < 2) {
		cout << "Error: MrCC needs at least two resolution levels (H >= 2) to perform the clustering process.";
	}//end if

	while(cin) {
		cin >> cellId;
		cin >> sum;

		cout << "id: " << cellId << " sum: " << sum << "\n";
	}
	//delete the temporary data file
	sprintf(rm_cmd, "rm -f %s", databaseName);
	system(rm_cmd);

	// printElapsed(); // prints the elapsed time

	return 0; // success
}