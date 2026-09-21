//
// Created by tailofhell on 9/8/26.
//

#include "battlebacks.h"

#include <ulib/ulib.h>
#include <string>
#include <nds.h>
#include "bgtiles_png.h"
#include "cheesyRat_png.h"
#include "../data/battlebacks/ratBattleback.h"

#define MAP_WIDTH   8
#define MAP_HEIGHT  6

static UL_MAP *ratMap;
static int tics;

void battlebackInit(std::string enemy) {
    if (enemy=="rat") {
        UL_IMAGE *ratImg = ulLoadImageFilePNG(bgtiles_png, (int) bgtiles_png_size, UL_IN_VRAM, UL_PF_PAL4);
        ratMap=ulCreateMap(ratImg,ratBattleback,32,32,8,6,UL_MF_U16);
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
        ulDrawMap(ratMap);
        if (tics%2!=1) {
            ratMap->scrollX+=1;
            ratMap->scrollY+=1;
        }
    }
}