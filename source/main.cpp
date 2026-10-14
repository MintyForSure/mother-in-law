#include <filesystem.h>
#include <nds.h>
#include "engine/game.h"
#include "engine/window.h"
#include "engine/sound.h"
#include <nf_lib.h>

#include "nitrofs/soundbank_info.h"

int main(int argc, char *argv[])
{
    consoleDebugInit(DebugDevice_NOCASH);
    NF_SetRootFolder("NITROFS");
    nitroFSInit(NULL);
    gameInit();
    audioInit();
    //windowSysInit();
    //consoleDemoInit();
    gameState='x';
    while (1)
    {
        gameLogic();
        NF_SpriteOamSet(0);
        NF_SpriteOamSet(1);
        scanKeys();
        // Wait for the screen refresh
        swiWaitForVBlank();

        // Update OAM
        oamUpdate(&oamMain);
        oamUpdate(&oamSub);
    }
    soundDisable();
    return 0;
}
