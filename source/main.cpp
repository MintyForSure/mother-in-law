#include <filesystem.h>
#include <ulib/ulib.h> // Include for µLibrary
#include <nds.h>
#include "engine/game.h"
#include "engine/window.h"
#include "engine/sound.h"
#include <nf_lib.h>

#include "soundbank.h"

int main(int argc, char *argv[])
{
    NF_SetRootFolder("NITROFS");
    nitroFSInit(NULL);
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
        NF_SpriteOamSet(1);
        oamUpdate(&oamSub);
        ulEndDrawing();
        ulSyncFrame();
    }
    soundDisable();
    return 0;
}
