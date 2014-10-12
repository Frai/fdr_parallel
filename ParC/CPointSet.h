//---------------------------------------------------------------------------

#ifndef CPointSetH
#define CPointSetH

#include <assert.h>

//---------------------------------------------------------------------------

class CPointSet {
    public:
        double *logR;
        double *logSqR;
        double A;
        double B;
        double error;   // fitting error
        int firstPoint; //index of the first point of the set
        int lastPoint; //index of the last point of the set
        int count; // current number of points in the set
        int maxCount; // maximun number of points in the set

        CPointSet(int NP);
        CPointSet(const CPointSet &);
        ~CPointSet();
        CPointSet & operator += (const CPointSet &);
        CPointSet & operator -= (const CPointSet & );
        CPointSet & operator = (const CPointSet & );
        void Clear();
};

//---------------------------------------------------------------------------
#endif
