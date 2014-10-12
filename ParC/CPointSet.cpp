//---------------------------------------------------------------------------

// #pragma hdrstop

#include "CPointSet.h"
//---------------------------------------------------------------------------
// #pragma package(smart_init)

//==============================================================================
//                             Class CPointSet
//==============================================================================
//---------------------------------------------------------------------------
//                          Constructor
//---------------------------------------------------------------------------
CPointSet::CPointSet(int NP) {
     A=B=error=0.0;
     count = firstPoint = lastPoint = 0;
     logR = new double[NP];
     logSqR = new double[NP];
     for (int i=0; i<NP; i++){
         logR[i]=0;
         logSqR[i]=0;
     }
     maxCount = NP;
}

//---------------------------------------------------------------------------
//                    Constructor por cópia
//---------------------------------------------------------------------------
CPointSet::CPointSet(const CPointSet & set) {
     int i;
     count = set.count;
     maxCount = set.maxCount;
     logR = new double[maxCount];
     logSqR = new double[maxCount];
     for (i=0; i<count; i++){
         logR[i]=set.logR[i];
         logSqR[i]=set.logSqR[i];
     }

     for (i = count; i < maxCount; i++){
         logR[i] = 0;
         logSqR[i] = 0;
     }

     firstPoint = set.firstPoint;
     lastPoint = set.lastPoint;
     A = set.A;
     B = set.B;
     error = set.error;
}


//---------------------------------------------------------------------------
//                          Destructor
//---------------------------------------------------------------------------
CPointSet::~CPointSet() {
     delete [] logR;
     delete [] logSqR;
}

//---------------------------------------------------------------------------
//  Insert a set of point in the end
//---------------------------------------------------------------------------
CPointSet & CPointSet::operator += (const CPointSet & set){
     int i, j, aux;
     aux = this->count;
     assert((this->count += set.count -1) <= maxCount);
     for (i = aux, j=1; i < this->count; i++, j++){
          this->logR[i] = set.logR[j];
          this->logSqR[i] = set.logSqR[j];
     }
     this->lastPoint = set.lastPoint;
     this->A = 0;
     this->B = 0;
     this->error = 0;
     return *this;
}

//---------------------------------------------------------------------------
//  Remove a sets of points from the end
//---------------------------------------------------------------------------
CPointSet & CPointSet::operator -= (const CPointSet & set){
     int i,j,aux;
     bool equal;
     assert (this->maxCount >= set.count);
     aux = this->count - set.count + 1;
     for (i = aux-1 , j=0, equal = 1; j < set.count && equal == 1; i++, j++)
         equal = (this->logR[i] == set.logR[j] && this->logSqR[i] == set.logSqR[j]);
     if (equal) {
             aux = this->count;
             this->count -= set.count -1;
             for (i = this->count; i < aux; i++){
                 this->logR[i] = 0;
                 this->logSqR[i] = 0;
             } // end for
     } // end if equal

     this->lastPoint = set.firstPoint;
     this->A = 0;
     this->B = 0;
     this->error = 0;
     return *this;
}

//---------------------------------------------------------------------------
//  Copy
//---------------------------------------------------------------------------
CPointSet & CPointSet::operator = (const CPointSet & set){
     int i;
 //    assert (this->maxCount >= set.count);
     this->count = set.count;
     for (i = 0; i < this->count; i ++){
          this->logR[i] = set.logR[i];
          this->logSqR[i] = set.logSqR[i];
     }
     for (i = this->count; i < maxCount; i ++){
         this->logR[i] = 0;
         this->logSqR[i] = 0;
     }
     this->firstPoint = set.firstPoint;
     this->lastPoint = set.lastPoint;
     this->A = set.A;
     this->B = set.B;
     this->error = set.error;
     return *this;
}


//---------------------------------------------------------------------------
//  Clear the point set
//---------------------------------------------------------------------------
void CPointSet::Clear(){

     for (int i = 0; i < count; i++)  {
                 logR[i] = 0;
                 logSqR[i] = 0;
     }

     lastPoint = lastPoint = count = 0;
     A = B = error = 0.0;
}

