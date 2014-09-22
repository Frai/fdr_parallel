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
* @version 1.1
* @author Antonio Canabrava Fraideinberze (antoniocf@usp.br)
* @author Robson Leonardo Ferreira Cordeiro (robson@icmc.usp.br)
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
              int H, int initialLevel, char memory) {
    // stores the number of dimensions (database), the number of grid levels (counting tree)
    this->numberOfDimensions = numberOfDimensions;
    this->H = H;
	this->initialLevel = initialLevel;

    // builds the counting tree and inserts objects on it
    calcTree = new stCountingTreeMap(numberOfDimensions, H);
    fastDistExponent(numberOfObjects, objectsArray, database, normalizeFactor, memory);
}//end stFDR::stFDR

//---------------------------------------------------------------------------
stFDR::~stFDR() {
    // disposes the used structures
    delete calcTree;
}//end stFDR::~stFDR

//---------------------------------------------------------------------------
void stFDR::fastDistExponent(int numberOfObjects, double ** objectsArray, FILE * database,
                             int normalizeFactor, char memory) {
    // double * minD, * maxD, biggest;
    double * onePoint, * resultPoint, * a, * b; // y=Ax+B to normalize each dataset.
    // double normalizationFactor = 1.0;

    // minD = (double *) calloc ((1 + numberOfDimensions), sizeof(double));
    // maxD = (double *) calloc ((1 + numberOfDimensions), sizeof(double));
    // a = (double *) calloc(numberOfDimensions, sizeof(double));
    // b = (double *) calloc(numberOfDimensions, sizeof(double));
    onePoint = (double *) calloc(numberOfDimensions, sizeof(double));
    resultPoint = (double *) calloc(numberOfDimensions, sizeof(double));

    // normalizes the data
    // minMax(numberOfObjects, objectsArray, database, minD, maxD, memory);
    // biggest = 0; // for Normalize==0, 1 or 3
    // //  0->Independent
    // //  1->mantain proportion
    // //  2->Clip
    // //  3->Geo Referenced
    // if(normalizeFactor == 2) {
    //     biggest = MAXDOUBLE;
    // }//end if

    // for(int i = 0; i < numberOfDimensions; i++) {
    //     a[i] = (maxD[i] - minD[i]) * normalizationFactor; //a takes the range of each dimension
    //     b[i] = minD[i];
    //     if (a[i] == 0) {
    //         a[i] = 1;
    //     }//end if
    // }//end for

    // for(int i = 0; i < numberOfDimensions; i++) {
    //     if((normalizeFactor < 2 || normalizeFactor == 3) && biggest < a[i]) {
    //         biggest = a[i];
    //     }//end if
    
    //     if(normalizeFactor == 2 && biggest > a[i]) {
    //         biggest = a[i];
    //     }//end if
    // }//end for

    // if(normalizeFactor != 0) {
    //     for(int i = 0; i < numberOfDimensions; i++) {
    //         a[i] = biggest; // normalized keeping proportion
    //     }//end for

    //     /* when we have the proportional normalization, every A[i] are gonna receive
    //     the biggest range.*/
    // }//end if

    // if (normalizeFactor >= 0) {
    //     calcTree->setNormalizationVectors(a, b); // if there is some normalization
    // }//end if

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
    // delete[] a;
    // delete[] b;
    // delete[] minD;
    // delete[] maxD;

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