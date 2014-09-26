#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <map>
#include <cfloat>
#include <string>
#include <iostream>

using namespace std;

//---------------------------------------------------------------------------
//   Computes the linear regression and the mean squared error.
//    X - x-points (in)
//    Y - y-points(in)
//    n - number of points (in)
//    m - slope of the line (out)
//    b - y-increment (out)
// result - mean squared error.(out)
double linerror (double *X, double *Y, int n, double &m, double &b) {
    double Sx = 0;
    double Sy = 0;
    double Sxy = 0;
    double Sxx = 0;
    double Syy = 0;
    double err;

    m = b = err = 0.0;

    for (int i = 0; i < n; i++) {
        Sx += X[i];
        Sy += Y[i];
        Sxy += X[i] * Y[i];
        Sxx += X[i] * X[i];
        Syy += Y[i] * Y[i];
    }

    if (((n * Sxx) - (Sx * Sx)) <= 0 || ((n * Syy) - (Sy * Sy)) <= 0 || (n * Sxx - Sx * Sx) == 0)
        return DBL_MAX;

    m = (n * Sxy - Sx * Sy) / (n * Sxx - Sx * Sx);
    b = (Sy - m * Sx) / n;
    err = (n * Sxy - Sx * Sy) / (sqrt((n * Sxx) - (Sx * Sx)) * sqrt((n * Syy) - (Sy * Sy)));
    return (1 - fabs(err));
}

//---------------------------------------------------------------------------
// Calculate the Fractal Dimension,
//    given:
//        the array of points <r, Sq(r)> as: LogR, LogSqR
//        number of points: NR
//        Minimum Least Squared Error acceptable: MinLSE
//        Minimum number of points acceptable: MinLen
//
//    returns the Fractal Dimension in two flavors:
//      - the minumum value obtained with a minimum of MinLen points: AMin, BMim
//      - the first value that lie in the minimum LSE requested: AFirst, BFirst
//      - err return the error obtained for <AFirst, BFirst>
//    A and B stands for y=Ax+B in the resultant line.
void FractCalc (int NR, double *LogR, double *LogSqR, double MinLSE, int MinLen,
                double *AMin, double *BMin, double *AFirst, double *BFirst, double *error ) {
    double errMinusBottom;
    double errMinusTop;
    double FDA;
    double FDB;
    double err;
    double auxerr;

    int inf = 0;
    int sup = NR;

    /* -----------------------------------------------------------------
    *  errorclip[ bx_, minlse_:0.015, minlen_:5]
    *   Calculate which part of the points we need to keep.
    *   The idea is that we keep the part that looks closer to a line,
    *    - as judged using the mean squared error
    */
    *AFirst = -1;
    *BFirst = -1;

    err = linerror(LogR, LogSqR, (sup - inf + 1), FDA, FDB);
    auxerr = err;
    *AMin = FDA;
    *BMin = FDB;

    while(((sup - inf + 1) > MinLen) && (LogSqR[inf + 1] != LogSqR[sup - 1])) {
        errMinusBottom = linerror(&LogR[inf + 1], &LogSqR[inf + 1], sup - inf, FDA, FDB);
        errMinusTop = linerror(&LogR[inf], &LogSqR[inf], sup - inf, FDA, FDB);
        if (errMinusBottom < errMinusTop) {
            inf++;
            err = errMinusBottom;
        } else {
            sup--;
            err = errMinusTop;
        }
        if (err < auxerr) {
            auxerr = err;
            *AMin = FDA;
            *BMin = FDB;
        }
        if (err < MinLSE && *AFirst == -1.) {
            *AFirst = FDA;
            *BFirst = FDB;
        }
    }
    *error = err;
}

int main(int argc, const char **argv) {
    FILE *pIn;
    FILE *pOut;

    int lines;

    double *logR;
    double *logSqR;

    double minLSE = 0.015;
    double minLen = 5;
    double aMin;
    double bMin;
    double aFirst;
    double bFirst;
    double error;

    double r;
    double sqr;

    map<double, double> m;
    map<double, double>::iterator it;

    if(argc != 3) {
        printf("Usage: ./merge <Number_Of_Lines> <Input_Filename>");
        return -1;
    }

    lines = atoi(argv[1]);
    pIn = freopen(argv[2], "r", stdin);
    pOut = freopen("../result.txt", "w", stdout);
    if(pIn == NULL || pOut == NULL) {
        printf("Error opening the file.\n");
        return 1;
    }

    for(int i = 0; i < lines; i++) {
        cin >> r;
        cin >> sqr;

        // cout << r << " " << sqr << "\n";
        if(m.find(r) == m.end()) {
            m[r] = sqr;
        } else {
            m[r] = m[r] + sqr;
        }
    }

    logR = (double *) malloc(m.size() * sizeof(double));
    logSqR = (double *) malloc(m.size() * sizeof(double));

    int i = 0;
    for(it = m.begin(); it != m.end(); it++) {
        cout << it->first << " " << log(it->second) << "\n";
        logR[i] = it->first;
        logSqR[i] = log(it->second);
        i++;
    }

    FractCalc(m.size(), logR, logSqR, minLSE, minLen, &aMin, &bMin, &aFirst, &bFirst, &error);
    cout << aMin << " * x + " << bMin << ", " << aFirst << " * x + " << bFirst << "\n";

    free(logR);
    free(logSqR);

    fclose(pIn);
    fclose(pOut);
    return 0;
}
