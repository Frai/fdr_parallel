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

	string cellId;
	string tmpCellId;
	string key;
	int sum;
	int len;
	double level;

	int x;

	map<string, int> m;
	map<string, int> m2;
	map<string, int> calcLog2;
	map<string, int>::iterator it;

	map<int, int> calcLog;
	map<int, int>::iterator it2;

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

		if(m.find(cellId) == m.end()) {
			m[cellId] = sum;
		} else {
			m[cellId] = m[cellId] + sum;
		}

		// for(int i = 0; i < numberOfDimensions; i++) {
		// 	tmpCellId = cellId;
		// 	for(int j = 0; j < cellId.length(); j++) {
		// 		if(j % numberOfDimensions == i) {
		// 			tmpCellId[j] = 'x';
		// 		}
		// 	}

		// 	if(m2.find(tmpCellId) == m2.end()) {
		// 		m2[tmpCellId] = sum;
		// 	} else {
		// 		m2[tmpCellId] = m2[tmpCellId] + sum;
		// 	}
		// }

	} // end while

	m[cellId] = m[cellId] - sum;
	
	for(it = m.begin(); it != m.end(); it++) {
		// cout << it->first << " " << it->second << "\n";
		len = (it->first).length();
		if(calcLog.find(len) == calcLog.end()) {
			calcLog[len] = pow(it->second, 2);
		} else {
			calcLog[len] = calcLog[len] + pow(it->second, 2);
		}
	}

	// cout << "\n";

	for(it2 = calcLog.begin(); it2 != calcLog.end(); it2++) {
		cout << log10(1.0 / pow(2, (it2->first / numberOfDimensions)));
		cout << " " << log10(it2->second) << "\n"; 
	}

	// cout << "\nCounting removing one variable at a time...\n\n";

	// int i;
	// for(it = m2.begin(); it != m2.end(); it++) {
	// 	cout << it->first << " " << pow(it->second, 2) << "\n";

	// 	for(i = 0; i < numberOfDimensions; i++) {
	// 		if((it->first)[i] == 'x') {
	// 			break;
	// 		}
	// 	}

	// 	len = (it->first).length();
	// 	key = to_string(len) + "_" + to_string(i);
	// 	if(calcLog2.find(key) == calcLog2.end()) {
	// 		calcLog2[key] = pow(it->second, 2);
	// 	} else {
	// 		calcLog2[key] = calcLog2[key] + pow(it->second, 2);
	// 	}
	// }

	// cout << "\n";

	// for(it = calcLog2.begin(); it != calcLog2.end(); it++) {
	// 	i = 0;
	// 	x = 0;
	// 	while((it->first)[i] != '_') {
	// 		x = (10 * x) + ((it->first)[i] - 48);
	// 		i++;
	// 	}

	// 	cout << 1.0 / x << " " << log(1.0 / x);
	// 	cout << " " << log(it->second) << "\n"; 
	// }

	return 0; // success
}