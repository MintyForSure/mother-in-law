#include <ulib/ulib.h> // Include for µLibrary
#include <nds.h>
#include "engine/hud.h"
#include "engine/menus.h"
#include "engine/game.h"
#include "engine/window.h"
#include "engine/sound.h"
#include <maxmod9.h>
#include <nf_lib.h>

#include "soundbank.h"
#include "soundbank_bin.h"

int main(int argc, char *argv[])
{
    gameInit();
    audioInit();
    NF_Set2D(1,0);
    NF_SetRootFolder("NITROFS");
    NF_InitSpriteBuffers();
    NF_InitSpriteSys(1);
    windowInit(128,4,64,32);
    //consoleDemoInit();
    gameState='b';
    while (1)
    {
        ulStartDrawing2D();
        gameLogic();
        ulReadKeys(0);
        ulEndDrawing();
        ulSyncFrame();
    }
    return 0;
}
