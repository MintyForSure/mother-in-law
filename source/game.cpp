//
// Created by tailofhell on 8/26/26.
//
#include <stdbool.h>
#include <ulib/ulib.h>
#include "game.h"
#include "data/enemyData.h"
#include "hud.h"
#include <iostream>
using namespace std;

int partyMembers=1;
char gameState;
bool debug;
int mapleHP[]={40,40,120}; //Target HP, Current HP, Max HP

bool initSwitch_b=false;

void gameInit() {
    // Initialization of µlibrary
    ulInit(UL_INIT_ALL);
    ulInitGfx();
    ulInitText();
    ulSetMainLcd(1);
    ulSetTransparentColor(RGB15(31,0,31));
}
void gameLogic() { //checks where the game is :)
    if (gameState=='o') {

    }
    else if (gameState=='b') {
        if (initSwitch_b==false){
            consoleDemoInit();
            battleHudInit();
            getEnemyData("cheesyRat");
            //battleInit();
            initSwitch_b=true;
            cout << "initSwitch_b=true" << endl;
        }
        else {
            hudRender('b');
            if (ul_keys.pressed.X) {
                cout << "Lowering Maple's HP to 12." << endl;
                mapleHP[0]=12;
            }
            else if (ul_keys.pressed.Y) {
                cout << "Setting Maple's HP to max" << endl;
                mapleHP[0]=mapleHP[2];
            }
        }
    }
    if (ul_keys.held.L && ul_keys.held.R) {
        debug=true;
        consoleDemoInit();
        //printf("debug enabled, probably");
    }
}