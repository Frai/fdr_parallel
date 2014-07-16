/**********************************************************************
* GBDI Arboretum - Copyright (c) 2002-2009 GBDI-ICMC-USP
*
*                           Homepage: http://gbdi.icmc.usp.br/arboretum
**********************************************************************/
/* ====================================================================
 * The GBDI-ICMC-USP Software License Version 1.0
 *
 * Copyright (c) 2009 Grupo de Bases de Dados e Imagens, Instituto de
 * Ciências Matemáticas e de Computação, University of São Paulo -
 * Brazil (the Databases and Image Group - Intitute of Matematical and
 * Computer Sciences).  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:
 *       "This product includes software developed by Grupo de Bases
 *        de Dados e Imagens, Instituto de Ciências Matemáticas e de
 *        Computação, University of São Paulo - Brazil (the Databases
 *        and Image Group - Intitute of Matematical and Computer
 *        Sciences)"
 *
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names of the research group, institute, university, authors
 *    and collaborators must not be used to endorse or promote products
 *    derived from this software without prior written permission.
 *
 * 5. The names of products derived from this software may not contain
 *    the name of research group, institute or university, without prior
 *    written permission of the authors of this software.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE AUTHORS OF THIS SOFTWARE OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * ====================================================================
 *                                            http://gbdi.icmc.usp.br/
 */
/**
* @file
* This file implements the class stFDR.
*
* @version 1.0
* @author Robson Leonardo Ferreira Cordeiro (robson@icmc.usp.br)
* @author Agma Juci Machado Traina (agma@icmc.usp.br)
* @author Christos Faloutsos (christos@cs.cmu.edu)
* @author Caetano Traina Jr (caetano@icmc.usp.br)
*/
// Copyright (c) 2002-2009 GBDI-ICMC-USP

//------------------------------------------------------------------------------
// class stFDR
//------------------------------------------------------------------------------

#include "stFDR.h"

stFDR::stFDR (double ** objectsArray, FILE * database, int numberOfDimensions,
              int normalizeFactor, int numberOfObjects,
              int centralConvolutionValue, int neighbourhoodConvolutionValue,
              int H, int initialLevel, char memory) {
    // stores the number of dimensions (database), the number of grid levels (counting tree) 
	// and the kind of clustering (soft or hard)
    this->numberOfDimensions = numberOfDimensions;
    this->H = H;
	this->initialLevel = initialLevel;

    // builds the counting tree and inserts objects on it
    calcTree = new stCountingTreeMap(numberOfDimensions, H);
    fastDistExponent(numberOfObjects, objectsArray, database, normalizeFactor, memory);

    // builds auxiliar vectors used to search for the relevant attributes
    attributesRelevance = new double[numberOfDimensions];

    // stores the convolution matrix (center and direct neighbours)
    this->centralConvolutionValue = centralConvolutionValue;
    this->neighbourhoodConvolutionValue = neighbourhoodConvolutionValue;
}//end stFDR::stFDR

//---------------------------------------------------------------------------
stFDR::~stFDR() {
    // disposes the used structures
    delete[] attributesRelevance;
    delete calcTree;
}//end stFDR::~stFDR

//---------------------------------------------------------------------------
double stFDR::calcCThreshold(double * attributesRelevance) {
    double * sortedRelevance = new double[numberOfDimensions];

    for(int i = 0; i < numberOfDimensions; i++) {
        sortedRelevance[i] = attributesRelevance[i];
    }//end for

    qsort(sortedRelevance, numberOfDimensions, sizeof(double), compare);
    // sorts the relevances vector
    double cThreshold = sortedRelevance[minimumDescriptionLength(sortedRelevance)];
    delete[] sortedRelevance;
    return cThreshold;
}//end stFDR::calcCThreshold

//---------------------------------------------------------------------------
int stFDR::minimumDescriptionLength(double * sortedRelevance) {
    int cutPoint = -1;
    double preAverage, postAverage, descriptionLength, minimumDescriptionLength;
    for(int i = 0;i < numberOfDimensions; i++) {
        descriptionLength = 0;
        // calculates the average of both sets
        preAverage = 0;

        for(int j = 0; j < i; j++) {
            preAverage += sortedRelevance[j];
        }//end for

        if(i) {
            preAverage /= i;      
            descriptionLength += (ceil(preAverage)) ? 
                (log10(ceil(preAverage)) / log10((double)2)) : 0;
                // changes the log base from 10 to 2
        }//end if

        postAverage=0;

        for(int j = i; j < numberOfDimensions; j++) {
            postAverage += sortedRelevance[j];
        }//end for

        if(numberOfDimensions - i) {
            postAverage /= (numberOfDimensions - i);      
            descriptionLength += (ceil(postAverage)) ?
                (log10(ceil(postAverage)) / log10((double)2)) : 0;
                // changes the log base from 10 to 2
        }//end if

        // calculates the description length
        for(int j = 0; j < i; j++) {      
            descriptionLength += (ceil(fabs(preAverage - sortedRelevance[j]))) ?
                     (log10(ceil(fabs(preAverage-sortedRelevance[j]))) / log10((double)2)) : 0;
                     // changes the log base from 10 to 2
        }//end for

        for(int j = i; j < numberOfDimensions; j++) {      
            descriptionLength += (ceil(fabs(postAverage - sortedRelevance[j]))) ?
                (log10(ceil(fabs(postAverage - sortedRelevance[j]))) / log10((double)2)) : 0;
                // changes the log base from 10 to 2
        }//end for

        // verify if this is the best cut point
        if(cutPoint == -1 || descriptionLength < minimumDescriptionLength) {
            cutPoint = i;
            minimumDescriptionLength = descriptionLength;
        }//end if

    }//end for

    return cutPoint;
}//end stFDR::minimumDescriptionLength

//---------------------------------------------------------------------------
void stFDR::walkThroughConvolution(int level) {
    // pointers to the parents of a cell
    stCell ** parentsVector = new stCell*[level];
    for(int i = 0; i < level; i++) {
        parentsVector[i] = 0;
    }//end for
    
    int biggestConvolutionValue = -MAXINT;
    betaClusterCenter = 0;
    // iniciate the recursive process to analyze a level
    walkThroughConvolutionRecursive(calcTree->getRoot(), parentsVector, level, 0,
                                    &biggestConvolutionValue);
    // disposes parentsVector
    delete[] parentsVector;

}//end stFDR::walkThroughConvolution

//---------------------------------------------------------------------------
void stFDR::walkThroughConvolutionRecursive(stNode * node, stCell ** nodeParents,
                                            int level, int currentLevel,
                                            int * biggestConvolutionValue) {
    map<stCellId *, stCell *, myCoparisson> * m = node->getRoot(); //node's map structure
    map<stCellId *, stCell *, myCoparisson>::iterator iter; //node's iterator
    if(node) {
        if(currentLevel < level) { // goes to the next level storing the pointer to the father
            for(iter = m->begin(); iter != m->end(); iter++) {
                nodeParents[currentLevel] = iter->second;
                walkThroughConvolutionRecursive(iter->second->nextLevel, nodeParents, level,
                                                (currentLevel + 1), biggestConvolutionValue);
            }//end for
        } else { // this is the level to be analyzed
            int newConvolutionValue;
            char clusterFoundBefore;
            double * maxCell = new double[numberOfDimensions];
            double * minCell = new double[numberOfDimensions];
            for(iter = m->begin(); iter != m->end(); iter++) {
                // Does not analyze cells analyzed before and cells that can't be the biggest convolution center.
                // It speeds up the algorithm, specially when neighbourhoodConvolutionValue <= 0
                if((!iter->second->getUsedCell()) && ((neighbourhoodConvolutionValue > 0) ||
                   ((iter->second->getSumOfPoints() * centralConvolutionValue) >
                    *biggestConvolutionValue))) {
                    // discovers the position of cell in the data space
                    cellPosition(iter->second, nodeParents, minCell, maxCell, level);
                    // verifies if this cell belongs to a cluster found before
                    clusterFoundBefore = 0;
                    for(int i = 0; !clusterFoundBefore && i < numBetaClusters; i++) {
                        clusterFoundBefore = 1;
                        for(int j = 0; clusterFoundBefore && j < numberOfDimensions; j++) {
                            // Does not cut off cells in a level upper than the level where a cluster was found
                            if(!(maxCell[j] <= maxBetaClusters[i][j] &&
                                 minCell[j] >= minBetaClusters[i][j])) {
                                clusterFoundBefore = 0;
                            }//end if
                        }//end for
                    }//end for

                    if(!clusterFoundBefore) { // the cell doesn't belong to any found cluster
                        // applies the convolution matrix to cell
                        if(neighbourhoodConvolutionValue) {
                            newConvolutionValue = applyConvolution(iter->second, nodeParents,
                                                                   currentLevel);
                        } else {
                            newConvolutionValue = centralConvolutionValue *
                                                                (iter->second->getSumOfPoints());
                            // when the   neighbourhood weight is zero
                        }//end if

                        if(newConvolutionValue > *biggestConvolutionValue) {
                            // until now, cell is the biggest convolution value
                            *biggestConvolutionValue = newConvolutionValue;
                            betaClusterCenter = iter->second;

                            for (int j=0 ; j<currentLevel ; j++) {
                                betaClusterCenterParents[j] = nodeParents[j];
                            }//end for

                        }//end if
                    }//end if
                }//end if        
            }//end while
            // disposes minCell and maxCell
            delete [] minCell;
            delete [] maxCell;
        }//end if
    }//end if
}//end stFDR::walkThroughConvolutionRecursive

//---------------------------------------------------------------------------
int stFDR::applyConvolution(stCell * cell, stCell ** cellParents, int level) {
    stCell * neighbour;
    int newValue = cell->getSumOfPoints() * centralConvolutionValue;
    // looks for the neighbours
    for(int k = 0; k < numberOfDimensions; k++) {
        neighbour = internalNeighbour(k, cell, ((level) ? cellParents[level-1] : 0));

        if(neighbour) {
            newValue += (neighbour->getSumOfPoints() * neighbourhoodConvolutionValue);
        }//end if

        neighbour = externalNeighbour(k, cell, cellParents, 0, level);
        if(neighbour) {
            newValue += (neighbour->getSumOfPoints() * neighbourhoodConvolutionValue);
        }//end if

    }//end for
    // return the cell value after applying the convolution matrix
    return newValue;

}//end stFDR::applyConvolution

//---------------------------------------------------------------------------
char stFDR::cellPosition(stCell * cell, stCell ** cellParents, 
                         double * min, double * max, int level) {
    if(cell) {
        if(level) {
            cellPosition(cellParents[level-1], cellParents, min, max, level - 1);

            for(int i = 0; i < numberOfDimensions; i++) {
                if(cell->getId()->getBitValue(i, numberOfDimensions)) {
                // bit in the position i is 1
                    min[i] += ((max[i] - min[i]) / 2);
                } else { // bit in the position i is 0
                    max[i] -= ((max[i] - min[i]) / 2);
                }//end if
            }//end for
        } else { // level zero
            for(int i = 0; i < numberOfDimensions; i++) {
                
                if(cell->getId()->getBitValue(i, numberOfDimensions)) {
                // bit in the position i is 1
                    min[i] = 0.5;
                    max[i] = 1;
                } else { // bit in the position i is 0
                    min[i] = 0;
                    max[i] = 0.5;
                }//end if
            }//end for
        }//end if

        return 1;
    }//end if

    return 0;
}//end stFDR::cellPosition

//---------------------------------------------------------------------------
char stFDR::cellPositionDimensionE_j(stCell * cell, stCell ** cellParents, 
                                     double * min, double * max, int level, int j) {
    if(cell) {
        if(level) {
            cellPositionDimensionE_j(cellParents[level-1], cellParents,
                                     min, max, level - 1, j);      
            if(cell->getId()->getBitValue(j, numberOfDimensions)) { // bit in the position j is 1
                *min += ((*max - *min) / 2);
            } else { // bit in the position j is 0
                *max -= ((*max - *min) / 2);
            }//end if
        } else { // level zero
            if (cell->getId()->getBitValue(j, numberOfDimensions)) { // bit in the position j is 1
                *min = 0.5;
                *max = 1;
            } else { // bit in the position j is 0
                *min = 0;
                *max = 0.5;
            }//end if

        }//end if

        return 1;
    }//end if

    return 0;

}//end stCorelationClustering::cellPositionDimensionE_j

//---------------------------------------------------------------------------
stCell * stFDR::externalNeighbour(int dimIndex, stCell * cell,
                                 stCell ** cellParents,
                                 stCell ** neighbourParents, int level) {

    if(level) {
        stCell * father = cellParents[level-1], * fathersNeighbour;
        if(cell->getId()->getBitValue(dimIndex, numberOfDimensions) ^
           father->getId()->getBitValue(dimIndex, numberOfDimensions)) {
            // XOR - different bit values -> starts going down in the tree
            // creates the index that the father's neighbour should have
            stCellId * fathersNeighbourId = new stCellId(numberOfDimensions);
            *fathersNeighbourId = * father->getId();
            fathersNeighbourId->invertBit(dimIndex, numberOfDimensions);
            // looks for the father's neighbour
            fathersNeighbour = calcTree->findInNode((level-1) ? cellParents[level-2] : 0,
                                                    fathersNeighbourId);
            delete fathersNeighbourId;
        } else {  // equal bit values -> continues going up in the tree
            fathersNeighbour = externalNeighbour(dimIndex, father, cellParents, 
                                                 neighbourParents, level-1);
            // recursively, finds the external neighbour of the father
        }//end if

        if(fathersNeighbour) { // father's neighbour was found
            // creates the index that the son's neighbour should have
            stCellId *sonsNeighbourId = new stCellId(numberOfDimensions);
            *sonsNeighbourId=*cell->getId();
            sonsNeighbourId->invertBit(dimIndex,numberOfDimensions);
            // looks for the son's neighbour
            cell = calcTree->findInNode(fathersNeighbour,sonsNeighbourId);

            if(neighbourParents) {
                neighbourParents[level-1] = fathersNeighbour;
                // if it existis, stores the pointer to the father's neighbour
            }//end if

            delete sonsNeighbourId;
            return cell; // return the external son's neighbour in the dimension i
        }//end if

        return 0; // there is no father's neighbour
    }//end if

    return 0; // a cell in level zero never has an external neighbour
}//end stFDR::externalNeighbour

//---------------------------------------------------------------------------
stCell *stFDR::internalNeighbour(int dimIndex, stCell * cell, stCell * father) {
  // creates the id that the neighbour should have
  stCellId * neighboursId = new stCellId(numberOfDimensions);
  *neighboursId= *cell->getId();
  neighboursId->invertBit(dimIndex, numberOfDimensions);
  cell = calcTree->findInNode(father, neighboursId);
  delete neighboursId;
  return cell;
}//end stFDR::internalNeighbour

//---------------------------------------------------------------------------
void stFDR::fastDistExponent(int numberOfObjects, double ** objectsArray, FILE * database,
                             int normalizeFactor, char memory) {
    double * minD, * maxD, biggest;
    double * onePoint, * resultPoint, * a, * b; // y=Ax+B to normalize each dataset.
    double normalizationFactor = 1.0;

    minD = (double *) calloc ((1 + numberOfDimensions), sizeof(double));
    maxD = (double *) calloc ((1 + numberOfDimensions), sizeof(double));
    a = (double *) calloc(numberOfDimensions, sizeof(double));
    b = (double *) calloc(numberOfDimensions, sizeof(double));
    onePoint = (double *) calloc(numberOfDimensions, sizeof(double));
    resultPoint = (double *) calloc(numberOfDimensions, sizeof(double));

    // normalizes the data
    minMax(numberOfObjects, objectsArray, database, minD, maxD, memory);
    biggest = 0; // for Normalize==0, 1 or 3
    // normalize=0->Independent, =1->mantain proportion, =2->Clip
    //          =3->Geo Referenced
    if(normalizeFactor == 2) {
        biggest = MAXDOUBLE;
    }//end if

    for(int i = 0; i < numberOfDimensions; i++) {
        a[i] = (maxD[i] - minD[i]) * normalizationFactor; //a takes the range of each dimension
        b[i] = minD[i];
        if (a[i] == 0) {
            a[i] = 1;
        }//end if
    }//end for

    for(int i = 0; i < numberOfDimensions; i++) {
        if((normalizeFactor < 2 || normalizeFactor == 3) && biggest < a[i]) {
            biggest = a[i];
        }//end if
    
        if(normalizeFactor == 2 && biggest > a[i]) {
            biggest = a[i];
        }//end if
    }//end for

    if(normalizeFactor != 0) {
        for(int i = 0; i < numberOfDimensions; i++) {
            a[i] = biggest; // normalized keeping proportion
        }//end for

        /* when we have the proportional normalization, every A[i] are gonna receive
        the biggest range.*/
    }//end if

    if (normalizeFactor >= 0) {
        calcTree->setNormalizationVectors(a, b); // if there is some normalization
    }//end if

    //process each point
    if (memory != 0) { // limited memory
        fseek(database,0,SEEK_SET); //go to the file begin
    }

    for(int i = 0; i < numberOfObjects; i++) {
        (memory == 0) ? copyPoint(objectsArray[i], onePoint, numberOfDimensions) :
                        readPoint(database, onePoint, numberOfDimensions);	 
        calcTree->insertPoint(onePoint,resultPoint); //add to the grid structure
    }//end for

    // disposes used memory
    delete[] onePoint;
    delete[] resultPoint;
    delete[] a;
    delete[] b;
    delete[] minD;
    delete[] maxD;

}//end stFDR::FastDistExponent

//---------------------------------------------------------------------------
void stFDR::minMax(int numberOfObjects, double ** objectsArray, FILE * database,
                   double * min, double * max, char memory) {
    timeNormalization = clock(); //start normalization time
    double * onePoint = new double[numberOfDimensions];
    for(int j = 0; j < numberOfDimensions; j++){
    // sets the values to the minimum/maximum possible here
        min[j] = MAXDOUBLE;
        max[j] = -MAXDOUBLE;
    }// end for

    // looking for the minimum and maximum values
    if(memory != 0) { // limited memory
        fseek(database,0,SEEK_SET); //go to the file begin
    }

    for(int i = 0; i < numberOfObjects; i++) {
        (memory == 0) ? copyPoint(objectsArray[i], onePoint, numberOfDimensions) :
                        readPoint(database, onePoint, numberOfDimensions);

        for(int j = 0; j < numberOfDimensions; j++) {
            if(onePoint[j] < min[j]) {
                min[j] = onePoint[j];
            }//end if

            if(onePoint[j] > max[j]) {
                max[j] = onePoint[j];
            }//end if

        }//end for
    }//end for

    delete[] onePoint;
    timeNormalization = (clock() - timeNormalization); //total time spent in the normalization
}//end stFDR::MinMax