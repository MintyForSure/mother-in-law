//
// Created by tailofhell on 8/26/26.
//

#include "game.h"

#include <stdbool.h>
#include <ulib/ulib.h>

#include "hud.h"
#include "../../../../../opt/wonderful/thirdparty/blocksds/core/libs/libnds/include/nds/arm9/console.h"
#include "../../../../../opt/wonderful/thirdparty/blocksds/external/ulibrary/include/ulib/ulib.h"
int partyMembers=1;
char gameState;
bool debug;
int mapleHP[]={40,40,40}; //Target HP, Current HP, Max HP

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
            battleHudInit();
            initSwitch_b=true;
            printf("initSwitch_b=true");
        }
        else {
            hudRender('b');
            if (ul_keys.pressed.X && mapleHP[1]!=30) {
                printf("Lowering Maple's HP to 30. \n");
                mapleHP[0]=30;
            }
            else if (ul_keys.pressed.X && mapleHP[1]==30) {
                printf("Raising Maple's HP to 40. \n");
                mapleHP[0]=40;
            }
        }
    }
    if (ul_keys.held.L && ul_keys.held.R) {
        debug=true;
        consoleDemoInit();
        //printf("debug enabled, probably");
    }
}