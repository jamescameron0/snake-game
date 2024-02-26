#include "objPosArrayList.h"

// Check lecture contents on general purpose array list construction, 
// and modify it to support objPos array list construction.

objPosArrayList::objPosArrayList()
{
    aList = new objPos[ARRAY_MAX_CAP]; //256 element array on heap
    listSize  = 0; //no elements in list on startup
    arrayCapacity = ARRAY_MAX_CAP; //max capacity 
}

objPosArrayList::~objPosArrayList()
{
    delete[] aList; //delete for lists
}

int objPosArrayList::getSize()
{
    return listSize;
}

void objPosArrayList::insertHead(objPos thisPos)
{
    if(listSize < arrayCapacity){

        for(int i = listSize; i > 0; i--){

        aList[i].setObjPos(aList[i-1]); //shuffles elements to tail
    }
    aList[0].setObjPos(thisPos);

    listSize++;
    }
}

void objPosArrayList::insertTail(objPos thisPos)
{
    if(listSize != arrayCapacity){

        aList[listSize] = thisPos;
        listSize++;
    }
}

void objPosArrayList::removeHead()
{
    if(listSize!=0){
        for(int i = 0; i < listSize - 1; i++){
        aList[i] = aList[i+1];
        }
        listSize--;
    }

}

void objPosArrayList::removeTail()
{
    if(listSize!=0){
        listSize--;
    }

}

void objPosArrayList::getHeadElement(objPos &returnPos)
{
    returnPos.setObjPos(aList[0]);
}

void objPosArrayList::getTailElement(objPos &returnPos)
{
    returnPos.setObjPos(aList[listSize-1]);
}

void objPosArrayList::getElement(objPos &returnPos, int index)
{
    returnPos= aList[index];
}