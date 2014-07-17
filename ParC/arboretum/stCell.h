#ifndef __STCELL_H
#define __STCELL_H

#include "stCellId.h"
#include "stNode.h"

class stNode; //informs that the class exists

class stCell {
    public:
        stCell(int dimensionality) {
            sumOfPoints = 0;
            nextLevel = 0;
            id = 0;
        }

        ~stCell() {
            if(nextLevel) {
                //delete nextLevel;
            }
        }

        void insertPoint() {
            sumOfPoints++;
        }

        int getSumOfPoints() {
            return sumOfPoints;
        }

        stCellId * getId() {
            return id;
        }

        void setId(stCellId * id) {
            this->id = id;
        }

        stNode * nextLevel;

    private:
        int sumOfPoints;
        stCellId * id;
};

#endif //__STCELL_H
