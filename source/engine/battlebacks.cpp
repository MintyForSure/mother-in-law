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

void battlebackInit(std::string enemy) {
    if (enemy=="rat") {
        UL_IMAGE *ratImg = ulLoadImageFilePNG(bgtiles_png, (int) bgtiles_png_size, UL_IN_VRAM, UL_PF_PAL4);
        ratMap=ulCreateMap(ratImg,ratBattleback,32,32,8,6,UL_MF_U16);
    }
}

void renderBattleback(std::string enemy) {
    if (enemy=="rat") {
        ulDrawMap(ratMap);
        ratMap->scrollX+=1;
    }
}