#ifndef __STCOUNTINGTREEMAP_H
#define __STCOUNTINGTREEMAP_H

#include "stNode.h"
#include "stCell.h"

class stCountingTreeMap {
    public:
        stCountingTreeMap(int dimensionality, int H) {
            //empty tree
            this->H = H;
            this->dimensionality = dimensionality;
            root = 0;
            sumOfPoints = 0;
            normalizeSlope = new double[dimensionality];
            normalizeYInc = new double[dimensionality];
            for (int i = 0; i < dimensionality; i++) {
                normalizeSlope[i] = 1;
                normalizeYInc[i] = 0;
            }
        }

        ~stCountingTreeMap() {
            delete [] normalizeSlope;
            delete [] normalizeYInc;
            if (root) {
                delete root;
            }
        }

        char insertPoint(double *point, double *normPoint) {
            double *min = new double[dimensionality];
            double *max = new double[dimensionality];
            char out = 0;

            // normalizes the point
            for (int i=0; i<dimensionality; i++) {
                normPoint[i] = (point[i]-normalizeYInc[i])/normalizeSlope[i];
                if (normPoint[i] < 0 || normPoint[i] > 1) {
                    out = 1; // invalid point
                }

                min[i] = 0;
                max[i] = 1;
            }

            if (!out) { // if the point is valid
                sumOfPoints++; // counts this point in level 0
                // recursively counts this point in all levels deeper than level 0
                insertPointRecursive(1, &root, min, max, normPoint);
            }//end if

            delete [] min;
            delete [] max;
            return !out; // return the "validity" of the point
        }

        stNode *getRoot() {
            return root;
        }

        double *getNormalizeYInc(){
            return normalizeYInc;
        }

        double *getNormalizeSlope(){
            return normalizeSlope;
        }

        int getSumOfPoints() {
            return sumOfPoints;
        }

        stCell *findInNode(stCell *father, stCellId *sonsId) {
            return (father)?father->nextLevel->findInNode(sonsId):root->findInNode(sonsId);
        }

        void setNormalizationVectors(double *slope, double *yInc) {
            for (int i=0; i<dimensionality; i++) {
                normalizeSlope[i] = slope[i];
                normalizeYInc[i] = yInc[i];
            }
        }

        void printTreeRecursive(stNode * pNode, double level) {
            char * index;
            if(pNode) {
                map<stCellId*, stCell*, myCoparisson> * pmCell = pNode->getRoot();
                map<stCellId*, stCell*, myCoparisson>::iterator iter;
                stCellId * pCellId;
                stCell * pCell;

                for(iter = pmCell->begin(); iter != pmCell->end(); iter++) {
                    pCellId = iter->first;
                    pCell = iter->second;

                    index = pCellId->getIndex();
                    cout << index << "\t" << pCell->getSumOfPoints() << "\n";

                    printTreeRecursive(pCell->nextLevel, pow(level, 2));
                } // end for
            } else {
                cout << "0\t0\n";
            } // end if
        }

    private:
        void insertPointRecursive(int currentLevel, stNode **pPCN,
                                  double *min, double *max, double *point) {
            if (currentLevel < H) {
                // mounts the id of the cell that covers normPoint in the current level
                // and stores in min/max this cell's lower and upper bounds
                double middle;
                stCellId *cellId = new stCellId(dimensionality);
                for (int i=0; i<dimensionality; i++) {
                    middle = (max[i]-min[i])/2 + min[i];
                    if (point[i] > middle) { // if point[i] == middle, considers that the point
        				// belongs to the cell in the lower half regarding i
                        cellId->invertBit(i,dimensionality); // turns the bit i to one
                        min[i] = middle;
                    } else {
                        // the bit i remains zero
                        max[i] = middle;
                    }
                }

                if (!*pPCN) { // the current node is empty
                    *pPCN = new stNode(); // creates a new node
                }
                stCell *cell = (*pPCN)->insertPoint(cellId,dimensionality); // inserts the point
                // and returns the corresponding cell

                delete cellId;
                insertPointRecursive(currentLevel+1,&cell->nextLevel,min,max,point);
            }
        }

        int sumOfPoints;
        int dimensionality;
        int H;
        stNode *root;
        double *normalizeSlope;
        double *normalizeYInc;
};

#endif //__STCOUNTINGTREEMAP_H
