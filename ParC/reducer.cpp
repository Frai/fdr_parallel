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
	int H = 0;
	int numberOfDimensions = 0;
	double alpha = 0;
	int aux;

	char cellId[10000];
	int sum;

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

	if(H < 2) { // first validations
		cout << "Error: MrCC needs at least two resolution levels (H >= 2) to perform the clustering process.";
	} // end if

	while(cin) {
		cin >> cellId;
		cin >> sum;
	
		cout << cellId << " " << sum << "\n";
	} // end while

	return 0; // success
}