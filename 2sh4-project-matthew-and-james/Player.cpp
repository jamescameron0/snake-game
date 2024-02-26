#include "Player.h"
#include "GameMechs.h"
#include "Food.h"
#include "objPos.h"
#include "objPosArrayList.h"

Player::Player(GameMechs* thisGMRef, Food* thisFoodRef )
{
    mainGameMechsRef = thisGMRef;
    mainFoodRef = thisFoodRef;
    myDir = STOP;

    // more actions to be included
    objPos tempPos;
    tempPos.setObjPos(mainGameMechsRef->getBoardSizeX()/2,
                      mainGameMechsRef->getBoardSizeY()/2,
                      '@');

    playerPosList = new objPosArrayList();
    playerPosList->insertHead(tempPos); 

}


Player::~Player()
{
    // delete any heap members here
    delete playerPosList; 
}

objPosArrayList* Player::getPlayerPos()
{
    // return the reference to the playerPos arrray list
    return playerPosList;
}

void Player::updatePlayerDir()
{
    // PPA3 input processing logic   

    // get input using MacUILib_getChar()
    // in game mechanism input will be checked and got 

    char input = mainGameMechsRef->getInput();
    //int score = mainGameMechsRef->getScore();

    switch(input)
    {
        case ' ':
            mainGameMechsRef->setExitTrue();
            break;

        case 'w':
                if (myDir != DOWN) myDir = UP;
                break;
            
        case 'a':
            if (myDir != RIGHT) myDir = LEFT;
            break;

        case 's':
            if (myDir != UP) myDir = DOWN;
            break;

        case 'd':
            if (myDir != LEFT) myDir = RIGHT;
            break;

        default:
            break;


    }    
    
}

void Player::movePlayer()
{

    objPos currHead;
    playerPosList->getHeadElement(currHead);
    // PPA3 Finite State Machine logic
    switch(myDir){


        case UP:
            if(currHead.y > 1 ){
                currHead.y--;
            }
            else{
                currHead.y = (mainGameMechsRef->getBoardSizeY()-2);
            }
            break;

        case DOWN:
            if(currHead.y < mainGameMechsRef->getBoardSizeY()-2){
                currHead.y++;
            }
            else{
                currHead.y = 1;
            }
            break;
        
        case LEFT:
            if(currHead.x > 1){
                currHead.x--;
            }
            else{
                currHead.x = (mainGameMechsRef->getBoardSizeX()-2);
            }
            break;

        case RIGHT:
            if(currHead.x < mainGameMechsRef->getBoardSizeX()-2){
                currHead.x++;
            }
            else{
                currHead.x = 1;
            }
            break;

        default:
            break;
    }

    playerPosList -> insertHead(currHead);  //default movement
    playerPosList -> removeTail();

    if(checkSelfCollision()){     //check self collision
        mainGameMechsRef->setLoseTrue(); 
        mainGameMechsRef->setExitTrue();
    }

    if (checkFoodConsumption()){   //check food consumption 
        increasePlayerLength();    // if it's true we increase player length, regenerate food, increment score
        mainFoodRef->generateFood(playerPosList);
    }

}


bool Player::checkFoodConsumption() {
    objPos currHead;
    playerPosList->getHeadElement(currHead); //get head element

    objPos foodPos;
    mainFoodRef->getFoodPos(foodPos);

    if (currHead.isPosEqual(&foodPos)) {
        return true; // Food consumed
    }

    return false; // No food consumption
}


void Player::increasePlayerLength() {
    objPos currHead;
    playerPosList->getHeadElement(currHead);

    playerPosList->insertHead(currHead); // Insert head w/o removing tail

    mainGameMechsRef->incrementScore();   //increment score
}


bool Player::checkSelfCollision() {
    
    objPos currHead;
    playerPosList->getHeadElement(currHead); //take currhead position

    objPos body;

    for (int i = 1; i < playerPosList->getSize(); i++) {  //compare current head for all elements of body
        playerPosList->getElement(body, i);

        if (currHead.isPosEqual(&body)) {  //if equal, return true "self collision detected"
            // Self-collision detecte
            return true;
        }
    }

    return false; // No self-collision
}