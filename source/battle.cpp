//
// Created by tailofhell on 8/28/26.
//
#include "battle.h"
#include <iostream>
#include <ulib/ulib.h>
#include "bg_png.h"
#include "data/enemyData.h"

extern "C" {
    #include "menus.h"
#include "game.h"
}

// void battleInit(char f) {
//     UL_IMAGE *battleBack=ulLoadImageFilePNG(static_cast<void *>(bg_png),(int)bg_png_size,UL_IN_VRAM,UL_PF_PAL4);
//
// }