//
// Created by tailofhell on 9/8/26.
//

#include "battlebacks.h"

#include <string>
#include <nds.h>
#include <nf_lib.h>
#include "../data/battlebacks/ratBattleback.h"

#define MAP_WIDTH   8
#define MAP_HEIGHT  6

//static UL_MAP *ratMap;
static int tics;

// Variables
u16 mapa[32][24];   // Holds the map
s16 bgx[192];       // Horizontal scroll of each line
s8 i[192];          // Scroll speed of each line

// Function that runs after a scanline is drawn. By modifying the values of the
// scroll registers it's possible to add a wave effect.
void hblank_handler(void)
{
    u32 vline = REG_VCOUNT; // Get the current line

    if (vline < 192)
    {
        // If this is a line inside the screen, handle the effect
        bgx[vline] += i[vline];

        if ((bgx[vline] < 1) || (bgx[vline] > 63))
            i[vline] *= -1;

        NF_ScrollBg(0, 3, (bgx[vline] / 8) - 4, 0);
    }
}

void battlebackInit(std::string enemy) {
    if (enemy=="rat") {
        //UL_IMAGE *ratImg = ulLoadImageFilePNG(reinterpret_cast<const char *>(bgtiles_png), (int) bgtiles_png_size, UL_IN_VRAM, UL_PF_PAL4);
        //ratMap=ulCreateMap(ratImg,ratBattleback,32,32,8,6,UL_MF_U16);
        //NF_LoadTiledBg("bg/bg","nfRatMap",256,256);
        //NF_CreateTiledBg(1,2,"nfRatMap");
        NF_LoadTiledBg("bg/bg","testbg",256,256);
        NF_CreateTiledBg(0,3,"testbg");
        s8 inc = 1;
        int x = 0;

        for (int y = 0; y < 192; y++)
        {
            x += inc;
            bgx[y] = x;

            if ((x < 1) || (x > 63))
                inc *= -1;

            i[y] = inc;
        }
        irqSet(IRQ_HBLANK, hblank_handler);
        irqEnable(IRQ_HBLANK);
    }
}

void renderBattleback(std::string enemy) {
    if (tics>59) {
        tics=0;
    }
    else {
        tics++;
    }
    if (enemy=="rat") {
        //NF_CreateTiledBg(0,0,"testbg");
    }
}