#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <time.h>

using namespace std;

int main(int argc, char *argv[]) {
	if(argc != 3) {
		printf("Usage: ./generate <Dimension> <Number_of_Points>\n");
		return 0;
	}

	srand(time(NULL));

	FILE * pFile1;
	FILE * pFile2;

	pFile1 = fopen("synthetic.dat", "w");
	pFile2 = fopen("synthetic.stc", "w");

	int dimensions = atoi(argv[1]);
	int points = atoi(argv[2]);

	double number;

	fprintf(pFile2, "set verbose info\n");
	fprintf(pFile2, "set verbose graph\n");
	fprintf(pFile2, "set fitalg 1\n");
	fprintf(pFile2, "set norma independent\n");
	fprintf(pFile2, "set dimension %d\n", dimensions);
	fprintf(pFile2, "minsert %d\n", points);

	for(int i = 0; i < points; i++) {
		fprintf(pFile1, "%d ", i + 1);
                number = rand() / (RAND_MAX * 1.0);
		for(int j = 0; j < dimensions; j++) {
			fprintf(pFile1, "%lf ", number);
			fprintf(pFile2, "%lf\t", number);
		}
		fprintf(pFile1, "0\n");
		fprintf(pFile2, "\n");
	}

	fprintf(pFile2, "close\n");

	fclose(pFile1);
	fclose(pFile2);
	return 0;
}
