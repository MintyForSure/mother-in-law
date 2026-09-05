#include <ulib/ulib.h> // Include for µLibrary
#include <nds.h>
#include "engine/hud.h"
#include "engine/menus.h"
#include "engine/game.h"
#include "engine/window.h"

int main(int argc, char *argv[])
{
    gameInit();
    windowInit(48,8,32,32);
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
