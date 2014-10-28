#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <map>
#include <cfloat>
#include <string>
#include <cstring>
#include <iostream>
#include "CPointSet.h"

using namespace std;

#define MAXDOUBLE DBL_MAX

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
//        number of points in the plot: N
//        Minimum Least Squared Error acceptable: MinLSE
//
//    returns the Fractal Dimension in two flavors:
//      - value of A and B (y=Ax+B in the resulting line): AMin, BMim
//      - error return the error obtained for <A,B>
//---------------------------------------------------------------------------
void NewFractCalc(int N, double * LogR, double * LogSqR, double MinLSE,
                  double * A, double * B, double * error) {
    fprintf(stderr, "NewFractCalc\n");

    int i, j, k, aux, nPairs, NP, maxCount, maxIdx;
    CPointSet ** points;
    double * slopeError, fitError, sError, max, min, mError, minFitError;
    float maxError, minSlope, add;

    nPairs = N - 1;  // number of pairs of points
    NP = 2;          // initial number of points in each set
    points = new CPointSet*[nPairs];  // set of points  - to fit the line for each CPointSet
    for(i = 0; i < nPairs; i++)
        points[i] = new CPointSet(N);

    minSlope = 0.10; // minimum slope considered
    max = 0; // used to find max slope error
    min = MAXDOUBLE;   // used to find min slope error
    slopeError = new double[nPairs];  // error considering slopes of consecutive fitting lines
    mError = 0; // mean slope error
    maxError = 0.30; // max slope error allowed - < 0.30
    sError = 0;  // step error - used to evaluate slope error in each step of the process
                 // sError is incremented (by add) in each step of the process
    fitError = MinLSE; // for fitting error
    minFitError = MinLSE / 10; // minimum fitting error


    // fits the line considering all the points
    // originally, point 0 is the last on the right of the plot
    for(i = N - 1, j = 0; i >= 0; i--, j++) {
        points[0]->logR[j] = LogR[i]; // here, it is in position 0
        points[0]->logSqR[j] = LogSqR[i];
    }// end for

    points[0]->firstPoint = 0;
    points[0]->count = N;
    points[0]->lastPoint = N - 1;
    points[0]->error = linerror(points[0]->logR, points[0]->logSqR, N, points[0]->A, points[0]->B);

    // the procedure usually returnns maxdouble when
    // the slope is zero - flat region
    if(points[0]->error == MAXDOUBLE) {
        points[0]->error = 0;
    }

    maxIdx = 0;

    // does the iteractive process if the fitting error for all the points is higher than
    // minFitError (=10% of MLSE)
    if(points[0]->error > minFitError) {
        // fits a line considering each pair of sequencial points and gets the
        // corresponding fitting error  - it will be zero if there are only 2 points
        for(i = N - 1, j = 0; i > 0; i--, j++) {  // point 0 is the last on the right of the plot
            aux = i;
            points[j]->firstPoint = j;
            points[j]->count = NP;

            for(k = 0; k < NP; k++, aux--) {
                points[j]->logR[k] = LogR[aux];
                points[j]->logSqR[k] = LogSqR[aux];
            }

            points[j]->lastPoint = j + 1;
            points[j]->error =
                    linerror(points[j]->logR, points[j]->logSqR, NP, points[j]->A, points[j]->B);

            if(points[j]->error == MAXDOUBLE) {
                points[j]->error = 0;
            }
        }// end for

        aux = 0;
        // gets the error of slopes of consecutive fitting lines
        for(i = 1; i < nPairs; i++)  {
            // sequence of points with very low slope
            // gets the error to quantify how significant is the difference between the slope of
            // consecutive sets of points
            if(points[i-1]->A < minSlope && points[i]->A < minSlope) {
                slopeError[i] = 0.0;
            } else {
                slopeError[i] = (fabs(points[i-1]->A - points[i]->A)) /
                                   (points[i-1]->A + points[i]->A);

                if(slopeError[i] > max) {
                    max = slopeError[i];   // gets the highest error
                }

                if(slopeError[i] < min) {
                    min = slopeError[i];  // gets the lowest error
                }

                mError += slopeError[i];
                aux++;
            } // end else
        } // end for

        // compute initial parameters
        // excludes min and max slope errors and computes mean slope error (mError), which is
        // used to define the initial step error (sError)
        if(nPairs >= 4 && aux > 2) {  // if there is at least three slopeErrors to consider
            mError -= (max + min);
            mError /= (aux - 2);
        } else mError /= nPairs -1;

        if(mError >= maxError) {
            mError = maxError;  // if mean slope error exceeds the max slope error defined
        }

        sError = mError / 2;  // step error - 50% of mean slope error
        add = sError / 2; // increment of sError in each step

        // iterative process to joint sets of points whose fitting lines present close slopes
        // the step error (sError) increases in each step of the process, until it reachs
        // mean error
        while(sError <= mError) {
            j = 0;
            i = 1;
            for(k = 0; k < nPairs; k++) {
                slopeError[k] = -1;
            }

            while(j < nPairs - 1 && i < nPairs) {
                if(points[i]->count > 0) {
                    // consecutive sets of points with very
                    if(points[j]->A < minSlope && points[i]->A < minSlope) {
                        slopeError[i]=0.0; // low slope
                    } else {
                        slopeError[i] = (fabs(points[j]->A - points[i]->A)) /
                                            (points[j]->A + points[i]->A);
                    }

                    if(slopeError[i] < sError) {
                        // joins the sets of points and computes the slope of the new fitting line
                        *points[j] += *points[i];
                        points[j]->error = linerror(points[j]->logR, points[j]->logSqR,
                                                    points[j]->count, points[j]->A, points[j]->B);

                        if(points[j]->error == MAXDOUBLE) {
                            points[j]->error = 0;
                        }

                        // if fitting error is ok ...
                        if(points[j]->error < fitError)  {
                            points[i]->Clear();
                            slopeError[i] = -1;
                        } else { // if the fitting error is high, undo join
                            *points[j] -= *points[i] ;
                            points[j]->error = linerror(points[j]->logR, points[j]->logSqR,
                                                        points[j]->count, points[j]->A,
                                                        points[j]->B);

                            if(points[j]->error == MAXDOUBLE) {
                                points[j]->error = 0;
                            }

                            j = i;

                        }  // end else
                    } else {
                        j = i;
                    }

                    i++;

                } else {
                    i++;
                } // end if points->count
            }// end while j && i

            i = 1;
            j = 0;

            // updates slopeError after last iteraction
            while(j < nPairs - 1 && i < nPairs) {
                while(j < nPairs && points[j]->count == 0) {
                    slopeError[i] = -1;
                    j++;
                    i++;
                }

                while(i < nPairs && points[i]->count == 0) {
                    slopeError[i] = -1;
                    i++;
                }

                if(i < nPairs && j < nPairs - 1) {
                    // consecutive sets of points with very
                    if (points[j]->A < minSlope && points[i]->A < minSlope) {
                        slopeError[i] = 0.0; // low slope
                    } else {
                        slopeError[i] = (fabs(points[j]->A - points[i]->A)) /
                                            (points[j]->A + points[i]->A);
                    }

                    j = i;
                    i++;
                }
            }

            // if there is no slopeError > current sError, stop the process
            i = 0;
            while(i < nPairs && (slopeError[i] < sError || slopeError[i] >= mError))
                i++;

            if(i == nPairs) {
                sError = 1; // stop
            } else {
                sError += add;  // increment step error and repeat...
            }

        } // end while sError

        maxCount = 2;
        maxIdx = -1;
    } // end if minFitError

    // gets the line fitting the higher number of points
    // if there is more than one, gets that whose points corresponds to higher randii
    for (i = 0; i < nPairs; i++)
        if (points[i] && points[i]->count >= maxCount && points[i]->A >= minSlope) {
            maxCount = points[i]->count;
            maxIdx = i;
        } // end if

    // return A, B and error
    if (maxIdx > -1) {
        *A = points[maxIdx]->A;
        *B = points[maxIdx]->B;
        *error = points[maxIdx]->error;
    } else {
        *A = 0.0;
        *B = 0.0;
        *error = 0.0;
    }

    for(i = 0; i < nPairs; i++)
        delete points[i];

    delete [] points;
    delete [] slopeError;
    return;
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
                double *AMin, double *BMin,
                double *AFirst, double *BFirst, double *error ) {
    fprintf(stderr, "FractCalc\n");
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
    FILE * pIn;
    FILE * pOut;
    FILE * dimensionality_file;
    FILE * dimensions;
    FILE * dimensions_tmp;

    char * dimensions_str;
    char * dimensions_tmp_str;

    int lines;
    int dimensionality;

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

    double maxR;
    double size;
    double aux;

    map<double, double> m;
    map<double, double>::reverse_iterator rit;

    if(argc != 4) {
        printf("Usage: ./merge <File_Mode> <Number_Of_Lines> <Input_Filename>");
        return -1;
    }

    if(strcmp(argv[1], "w") == 0) {
        pOut = freopen("../result.txt", argv[1], stdout);
    } else {
        pOut = freopen("../result_tmp.txt", argv[1], stdout);
    }

    if(pOut == NULL) {
        fprintf(stderr, "Could not open the file. Exiting...\n");
        return -1;
    }

    fprintf(stderr, "----- MERGER OUTPUT -----\n");

    lines = atoi(argv[2]);
    pIn = freopen(argv[3], "r", stdin);
    dimensionality_file = fopen("dimensionality", "r");
    dimensions = fopen("dimensions", "r");
    dimensions_tmp = fopen("dimensions_tmp", "r");

    if(pIn == NULL || pOut == NULL || dimensions == NULL || dimensionality_file == NULL) {
        printf("Error opening the file in merge.\n");
        return 1;
    }

    fscanf(dimensionality_file, "%d", &dimensionality);
    dimensions_str = (char *) malloc((dimensionality + 1) * sizeof(char));
    dimensions_tmp_str = (char *) malloc((dimensionality + 1) * sizeof(char));
    fscanf(dimensions, "%s", dimensions_str);
    fscanf(dimensions_tmp, "%s", dimensions_tmp_str);

    for(int i = 0; i < lines; i++) {
        cin >> r;
        cin >> sqr;

        if(m.find(r) == m.end()) {
            m[r] = sqr;
            fprintf(stderr, "New Radius: %lf - %lf\n", r, sqr);
        } else {
            m[r] = m[r] + sqr;
            fprintf(stderr, "Old Radius: %lf - %lf\n", r, sqr);
        }
    }


    FILE * pfSize = fopen("size", "r");
    fscanf(pfSize, "%lf", &size);
    fclose(pfSize);

    logR = (double *) malloc((m.size() + 1) * sizeof(double));
    logSqR = (double *) malloc((m.size() + 1) * sizeof(double));

    int i = 0;
    aux = size * (size - 1.0) / 2.0;
    for(rit = m.rbegin(); rit != m.rend(); rit++) {
        if(aux < 1) {
            continue;
        }
        cout << rit->first << " " << log(aux) << "\n";
        logR[i] = rit->first;
        logSqR[i] = log(aux);
        i++;
        aux = rit->second;
    }

    if(aux >= 1) {
        logR[i] = log(1.0 / pow(2, i + 1));
        logSqR[i] = log(aux);
        cout << logR[i] << " " << logSqR[i] << "\n";
    }
    maxR = logR[i];

    
    NewFractCalc (m.size() + 2, logR, logSqR, minLSE, &aMin, &bMin, &error);

    if(aMin == 0 && bMin == 0) {
        FractCalc(m.size() + 1, logR, logSqR, minLSE, minLen, &aMin, &bMin, &aFirst, &bFirst, &error);
    }

    cout << "Dimension: " << dimensions_str << " " << dimensions_tmp_str << "\n";
    cout << aMin << " * x + " << bMin << "\n";

    free(logR);
    free(logSqR);
    free(dimensions_str);
    free(dimensions_tmp_str);

    fclose(pIn);
    fclose(pOut);
    fclose(dimensionality_file);
    fclose(dimensions);
    fclose(dimensions_tmp);
    return 0;
}
