//
// Created by tailofhell on 8/28/26.
//
#include "battle.h"
#include <iostream>
#include <nds/arm9/video.h>
#include <ulib/ulib.h>
#include "bg_png.h"
#include "../data/enemyData.h"
#include "cursor_png.h"
#include "menus.h"
#include "game.h"
#include "cheesyRat_png.h"
#include "hud.h"
#include "window.h"
#include "windowAlt.h"
#include <maxmod9.h>

#include "soundbank.h"
#include "soundbank_bin.h"

using namespace std;

static s8 tics=0;
string mapleAction;
string aaronAction;

namespace {
    struct battleElements { //one magic variable wont hurt
        UL_IMAGE *battleBack;
        UL_IMAGE *enemy0;
        UL_IMAGE *enemy1;
        UL_IMAGE *enemy2;
        UL_IMAGE *cursor;
    };
}

static struct battleElements bElem;

void battleInit(const char *enemy0, const char *enemy1, const char *enemy2) {
    //std::string turnOrder[]={mapleStats[4]};
    bElem.battleBack=ulLoadImageFilePNG(reinterpret_cast<const char *>(bg_png),(int)bg_png_size,UL_IN_VRAM,UL_PF_PAL4);
    bElem.cursor=ulLoadImageFilePNG(reinterpret_cast<const char *>(cursor_png),(int)cursor_png_size,UL_IN_VRAM,UL_PF_PAL4);
    ulSetImageTileSize(bElem.cursor,0,0,8,8);
    string enemies[]={enemy0,enemy1,enemy2};
    //std::cout << enemies[0] << std::endl;
    if (enemies[0] == "rat") {
        getEnemyData("cheesyRat");
        bElem.enemy0=ulLoadImageFilePNG(cheesyRat_png,(int)cheesyRat_png_size,UL_IN_VRAM,UL_PF_PAL4);
    }
}

// int getTurnOrder() {
//     char16_t turnOrder[]={};
//
//
//     return *turnOrder;
// }

void battleProcess() {
    ulDrawImage(bElem.battleBack);

    ulImageSetRotCenter(bElem.enemy0);

    ulDrawImageXY(bElem.enemy0,128,192/2);
    if (battleMenuState==1) {
        tics++;
        if (tics>29) {
            ulSetImageTileSize(bElem.cursor,0,0,8,8);
        }
        else {
            ulSetImageTileSize(bElem.cursor,8,0,8,8);
        }
        if (tics==60) {
            tics=0; //reset
        }
        ulSetImageTint(bElem.enemy0,RGB15(31,31,31));
        ulDrawImageXY(bElem.cursor,bElem.enemy0->x-24,bElem.enemy0->y-24);
        if (bElem.enemy1!=nullptr && ul_keys.pressed.left) {
            cout << "meow" << endl;
            ulDrawImageXY(bElem.cursor,bElem.enemy1->x-24,bElem.enemy1->y-24);
        }
        else if (ul_keys.pressed.B) {
            ulSetImageTint(bElem.enemy0,RGB15(31,31,31));
            if (bElem.enemy1!=nullptr) {
                ulSetImageTint(bElem.enemy1,RGB15(31,31,31));
            }
            if (bElem.enemy2!=nullptr) {
                ulSetImageTint(bElem.enemy2,RGB15(31,31,31));
            }

        }
    }
}
