#include <iostream>
#include "MacUILib.h"
#include "objPos.h"
#include "GameMechs.h"
#include "Player.h"
#include "Food.h"
#include "objPosArrayList.h"

using namespace std;

#define DELAY_CONST 100000

GameMechs* myGM;
Player* myPlayer;
Food* myFood;

bool exitFlag;

void Initialize(void);
void GetInput(void);
void RunLogic(void);
void DrawScreen(void);
void LoopDelay(void);
void CleanUp(void);



int main(void)
{

    Initialize();

    while(myGM->getExitFlagStatus() == false)  
    {
        GetInput();
        RunLogic();
        DrawScreen();
        LoopDelay();
    }

    CleanUp();

}


void Initialize(void)
{
    MacUILib_init();
    MacUILib_clearScreen();

    myGM = new GameMechs(30, 15); //board size
    myFood = new Food(myGM);
    myPlayer = new Player(myGM, myFood);
    
    
    //here goes the player food item gneeratin
    objPosArrayList* playerBody = myPlayer -> getPlayerPos();
    myFood->generateFood(playerBody);

}

void GetInput(void)
{
    myGM->getInput();
}

void RunLogic(void)
{
    myPlayer -> updatePlayerDir();
    myPlayer -> movePlayer();

    myGM->clearInput(); //clear input to ensure non repeating input

}

void DrawScreen(void)
{
    MacUILib_clearScreen();

    bool drawn;

    objPosArrayList* playerBody = myPlayer->getPlayerPos();
    objPos tempBody;

    objPos foodPos;
    myFood->getFoodPos(foodPos);

    for(int row = 0; row < myGM->getBoardSizeY(); row++){
        for (int col = 0; col < myGM->getBoardSizeX(); col++){

            drawn = false;

            for(int k = 0; k < playerBody->getSize(); k++){

                playerBody->getElement(tempBody, k);
                if (tempBody.x == col && tempBody.y == row){

                    MacUILib_printf("%c", tempBody.symbol);
                    drawn = true;
                    break;
                }
            }
            if(drawn) continue;
            //if player body drawn, dont draw below

            if (row == 0 || row == myGM->getBoardSizeY() - 1 || col == 0 || col == myGM->getBoardSizeX() - 1) {
                
                MacUILib_printf("#");
            }
            else if (row == foodPos.y && col == foodPos.x) {
                
                MacUILib_printf("%c", foodPos.symbol);
            }
            else{
                
                MacUILib_printf(" ");
            }
        }
        MacUILib_printf("\n");
    }   

    MacUILib_printf("Press Spacebar to quit\n");
    MacUILib_printf("your score is %d\n", myGM->getScore());

    if (myGM->getLoseFlagStatus() == true){
        MacUILib_printf("you ran into your tail. you Lose!\n");
        MacUILib_Delay(999999);
        myGM->setExitTrue();
    }
}

void LoopDelay(void)
{
    MacUILib_Delay(DELAY_CONST); // 0.1s delay
}


void CleanUp(void)
{
    MacUILib_clearScreen();    
  
    MacUILib_uninit();

    delete myPlayer;
    delete myGM;
    delete myFood;
}
