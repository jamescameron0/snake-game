#include "Food.h"
#include "MacUILib.h"
#include "objPosArrayList.h"
#include "objPos.h"

#include <cstdlib>
#include <time.h>

Food::Food(GameMechs* thisGMRef)
{
    mainGameMechsRef = thisGMRef;
    foodPos.setObjPos(-1,-1,'o');
}

Food::~Food()
{

}

void Food::generateFood(objPosArrayList* blockOff)
{
    int Count = 0;
    objPos currElement;

    int sizex = mainGameMechsRef->getBoardSizeX();  //variables for board size x and y
    int sizey = mainGameMechsRef->getBoardSizeY();

    srand(time(NULL));

    while(Count == 0){
        foodPos.x = rand() % (sizex - 2 ) + 1; // start by generating random positon
        foodPos.y = rand() % (sizey - 2 ) + 1;

        bool Pos = true;

        for (int i = 0; i < blockOff->getSize() - 1 ; i++){   //for each element in the obj list
            blockOff->getElement(currElement, i);
            if (foodPos.x == currElement.x && foodPos.y == currElement.y){  //compare food pos generated to each player body position
                Pos = false;                                                // if they overlap, we regenerate the food by setting false and breaking out
                break;
            }
        }
        if(Pos){                //finally increase count if no overlap to end while loop
            Count++;
        }
    }
    

}

void Food::getFoodPos(objPos &returnPos)
{
    returnPos = foodPos;
}