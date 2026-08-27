#include <ulib/ulib.h> // Include for µLibrary
#include <nds.h>
#include "hud.h"
#include "menus.h"
#include "game.h"
int main(int argc, char *argv[])
{
    gameInit();
    consoleDemoInit();
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
