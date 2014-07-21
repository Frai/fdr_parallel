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
* This file defines the class stFDR.
*
* @version 1.0
* @author Robson Leonardo Ferreira Cordeiro (robson@icmc.usp.br)
* @author Agma Juci Machado Traina (agma@icmc.usp.br)
* @author Christos Faloutsos (christos@cs.cmu.edu)
* @author Caetano Traina Jr (caetano@icmc.usp.br)
*
*/
// Copyright (c) 2002-2009 GBDI-ICMC-USP

#ifndef __STFDR_H
#define __STFDR_H

#include "arboretum/stCommon.h"
#include "arboretum/stCountingTreeMap.h"

#ifndef __GNUG__
	#include "Utile.h"	
#endif //__GNUG__

#ifdef __GNUG__
	#include "Utile.cpp"
#endif //__GNUG__

#include <math.h>
#include <stdio.h>
#include <time.h>

//----------------------------------------------------------------------------
// class stFDR
//----------------------------------------------------------------------------
/**
* This class is used to find clusters in subspaces of the original data space.
*
* @version 1.0
* @author Robson Leonardo Ferreira Cordeiro (robson@icmc.usp.br)
* @author Agma Juci Machado Traina (agma@icmc.usp.br)
* @author Christos Faloutsos (christos@cs.cmu.edu)
* @author Caetano Traina Jr (caetano@icmc.usp.br)
*/
//---------------------------------------------------------------------------
class stFDR {

    public:

        /**
        * Creates the needed structures to find correlation clusters.
        *
        * @param objectsArray Array with the database objects.
        * @param numberOfDimensions Number of dimensions in the database.
        * @param normalizeFactor Determines how data will be normalized.
        * @param numberOfObjects Number of objects in the database.
        * @param H The number of grid levels to build the counting tree.
        *
        */
        stFDR(double ** objectsArray, FILE * database, int numberOfDimensions,
              int normalizeFactor, int numberOfObjects,
              int H, int initialLevel, char memory);

        /**
        * Disposes the allocated memory.
        */
        ~stFDR();

        /**
        * Gets the used counting tree.
        */
        stCountingTreeMap * getCalcTree() {
            return calcTree;
        }//end getCalcTree

        /**
        * Gets the time spent in the normalization.
        */
        clock_t getTimeNormalization() {
            return timeNormalization;
        }//end getTimeNormalization

    private:
        /**
        * Time spent in the normalization.
        */
        clock_t timeNormalization;

        /**
        * Counting-tree pointer.
        */
        stCountingTreeMap *calcTree;

        /**
        * Number of dimensions in the database.
        */
        int numberOfDimensions;

        /**
        * Number of grid levels in the counting tree.
        */
        int H;

        int initialLevel;

        /**
        * Normalize data points and insert them in the counting tree.
        * Method from the stFractalDimension class (adapted).
        *
        * @param numberOfObjects Number of objects in the database.
        * @param objectsArray Array with the database objects.
        * @param normalizeFactor Determines how data will be normalized.
        *
        */
        void fastDistExponent(int numberOfObjects, double ** objectsArray, FILE * database,
                              int normalizeFactor, char memory);

        /**
        * Finds the minimum and maximum data values in each dimension.
        * Method from the stFractalDimension class (adapted).
        *
        * @param numberOfObjects Number of objects in the database.
        * @param objectsArray Array with the database objects.
        * @param min Vector to receive the minimum data value in each dimension.
        * @param max Vector to receive the maximum data value in each dimension.
        *
        */
        void minMax(int numberOfObjects, double ** objectsArray, FILE * database,
                    double * min, double * max, char memory);

};//end stFDR
//----------------------------------------------------------------------------
#endif //__STFDR_H