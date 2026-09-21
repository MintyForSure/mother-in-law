#include <ulib/ulib.h> // Include for µLibrary
#include <nds.h>
#include <maxmod9.h>
#include "engine/hud.h"
#include "engine/menus.h"
#include "engine/game.h"
#include "engine/window.h"
#include "engine/sound.h"
#include <nf_lib.h>

#include "soundbank.h"

int main(int argc, char *argv[])
{
    gameInit();
    audioInit();
    windowSysInit();
    //consoleDemoInit();
    gameState='x';
    while (1)
    {
        ulStartDrawing2D();
        gameLogic();
        ulReadKeys(0);
        NF_UpdateTextLayers();
        ulEndDrawing();
        ulSyncFrame();
    }
    soundDisable();
    return 0;
}
