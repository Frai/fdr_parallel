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
	printf("Elapsed time: %0.3lf sec.\n",(double)(clock()-startTime)/CLOCKS_PER_SEC);
}//end printElapsed

const char * byte_to_binary(const unsigned char x) {
	static char b[9];
	b[0] = '\0';

	int z;
	for(z = 256; z > 0; z >>= 1) {
		strcat(b, ((x & z) == z) ? "1" : "0");
	}

	return b;
}

void printTree(stNode * pNode, int level) {
	map<stCellId*, stCell*, myCoparisson> * pmCell = pNode->getRoot();
	map<stCellId*, stCell*, myCoparisson>::iterator iter;
	stNode * pAuxNode;
	stCell * pCell;

	while(level--) {
		for(iter = pmCell->begin(); iter != pmCell->end(); iter++) {
			pCell = iter->second;
			pAuxNode = pCell->nextLevel;
			printTree(pAuxNode, level);
		}
	}

	if(pNode) {
		stCellId * pCellId;
		int data = 0;
		for(iter = pmCell->begin(); iter != pmCell->end(); iter++) {
			pCellId = iter->first;
			pCell = iter->second;
			data++;
			printf("data: %d - %d ", data, pCell->getSumOfPoints());
			unsigned char * index = pCellId->getIndex();
			for(int i = 0; i < sizeof(index) / sizeof(index[0]); i++) {
				printf("%s ", byte_to_binary(index[i]));
			}
			printf("\n");
		}
	}
}

void printTreeRecursive(stNode * pNode, double level) {
	if(pNode) {
		map<stCellId*, stCell*, myCoparisson> * pmCell = pNode->getRoot();
		map<stCellId*, stCell*, myCoparisson>::iterator iter;
		stCellId * pCellId;
		stCell * pCell;
		int data = 0;
		double r = log(level);
		double S = 0;
		double logS;
		for(iter = pmCell->begin(); iter != pmCell->end(); iter++) {
			pCellId = iter->first;
			pCell = iter->second;
			data++;
			// printf("data: %d - %d ", data, pCell->getSumOfPoints());
			// unsigned char * index = pCellId->getIndex();
			// for(int i = 0; i < sizeof(index) / sizeof(index[0]); i++) {
			// 	printf("%s ", byte_to_binary(index[i]));
			// }
			// printf("\n");

			S += pow(pCell->getSumOfPoints(), 2);

			printTreeRecursive(pCell->nextLevel, pow(level, 2));
		}

		logS = log(S);

		printf("level: %lf, logR: %lf, S: %lf, logS: %lf\n", level, r, S, logS);
	}
}

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

	initClock(); // initiates the meassurement of run time

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

	// reads objects from the source database
	int numberOfObjects = 0;
	cin >> key;
	sprintf(databaseName,"database_%s.dat",key);
	FILE *database = fopen(databaseName, "w");

	while(cin) { // reads a point per iteration
        if(numberOfObjects) {
			cin >> key; // reads the key
        }
       	cin >> point; // reads the point
       	if(cin) {
			int posPoint = 0; char num[1000];
			for(int j = 0; j < numberOfDimensions; j++) {
				int posNum = 0;
				while(point[posPoint] != '_' && point[posPoint] != 0) {
					num[posNum] = point[posPoint];
					posNum++;
					posPoint++;
				}
				num[posNum] = 0;
				posPoint++;
				fputs(num,database);
				fputs((j==numberOfDimensions-1) ? "\n" : " ", database);
			}//end for

			numberOfObjects++;
			//cout << key << " " << point << "\n";
       	}
	}
	// closes the database in "w" mode
	fclose(database);

	if (numberOfObjects) {
		database = fopen(databaseName,"r"); // opens the database in "r" mode

		// creates an object of the class stFDR
		stFDR *sFDR = new stFDR(0, database, numberOfDimensions, NORMALIZE_FACTOR, numberOfObjects, H, 1, 1);

		stCountingTreeMap * pCTree = sFDR->getCalcTree();
		printf("Sum of Points: %d - Nro Objects: %d\n", pCTree->getSumOfPoints(), numberOfObjects);
		double *pdYInc = pCTree->getNormalizeYInc();
		double *pdSlope = pCTree->getNormalizeSlope();
		for(int i = 0; i < numberOfDimensions; i++) {
			printf("i: %d - YInc: %lf - Slope: %lf\n", i, pdYInc[i], pdSlope[i]);
		}

		printTreeRecursive(pCTree->getRoot(), 0.5);

		// the database file will not be used anymore, thus close it
		fclose(database);

		// disposes the used structures
		delete sFDR;
	} else {
		cout << "no point was assigned to this reducer\n";
	}

	//delete the temporary data file
	sprintf(rm_cmd,"rm -f %s",databaseName);
	system(rm_cmd);

	printElapsed(); // prints the elapsed time

	return 0; // success
}