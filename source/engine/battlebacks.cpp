//
// Created by tailofhell on 9/8/26.
//

#include "battlebacks.h"

#include <string>
#include <nds.h>
#include <nf_lib.h>
#include <nds/arm9/video.h>

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
        NF_ScrollBg(1, 3, (bgx[vline] / 8), 0);
        NF_ScrollBg(1, 2, (bgx[vline] / 8), 0);
    }
}

void battlebackInit(std::string enemy) {
    if (enemy=="rat") {
        NF_LoadTiledBg("bg/bg","testbg",256,256);
        NF_LoadTiledBg("bg/black","black",256,256);
        NF_CreateTiledBg(0,3,"testbg");
        NF_CreateTiledBg(1,3,"testbg");
        NF_CreateTiledBg(1,2,"black");
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
        REG_BLDCNT_SUB = BLEND_ALPHA | BLEND_SRC_BG2 | BLEND_DST_BG3;
        REG_BLDALPHA_SUB = BLDALPHA_EVA(8) | BLDALPHA_EVB(8);  // Equal blend: 8/16 of each layer
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