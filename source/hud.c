//
// Created by tailofhell on 8/24/26.
//
#include <nds.h>
#include <ulib/ulib.h>
#include "hud.h"
#include "battleIcons_png.h"
#include "battleTab_png.h"
#include "game.h"
#include <stdio.h>

static struct battleElem;

struct battleElem{
    UL_IMAGE *b_iconsFight;
    UL_IMAGE *b_iconsSkill;
    UL_IMAGE *b_iconsItem;
    UL_IMAGE *b_iconsDefend;

    UL_IMAGE *b_battleTab;
}

int battleHudInit(void *arg) {
    printf("battle hud initializing");
    //Load battle hud elements
    bHUD->b_iconsFight=ulLoadImageFilePNG((void*)battleIcons_png,(int)battleIcons_png_size,UL_IN_VRAM,UL_PF_PAL4);
    bHUD->b_iconsSkill=ulLoadImageFilePNG((void*)battleIcons_png,(int)battleIcons_png_size,UL_IN_VRAM,UL_PF_PAL4);
    bHUD->b_iconsItem=ulLoadImageFilePNG((void*)battleIcons_png,(int)battleIcons_png_size,UL_IN_VRAM,UL_PF_PAL4);
    bHUD->b_iconsDefend=ulLoadImageFilePNG((void*)battleIcons_png,(int)battleIcons_png_size,UL_IN_VRAM,UL_PF_PAL4);

    bHUD->b_battleTab=ulLoadImageFilePNG((void*)battleTab_png,(int)battleTab_png_size,UL_IN_VRAM,UL_PF_PAL4);
    ulSetImageTileSize(bHUD->b_iconsFight,0,16,16,16);
    ulSetImageTileSize(bHUD->b_iconsItem,16,16,16,16);
    ulSetImageTileSize(bHUD->b_iconsSkill,32,16,16,16);
    ulSetImageTileSize(bHUD->b_iconsDefend,48,16,16,16);
    return 0;
}
void hudRender(char hudType, void *arg) {
    if (hudType=='b'){
        if (partyMembers==1) {
            printf("meow");
            ulDrawImage(bHUD->b_battleTab);
            bHUD->b_battleTab->x=128;
            bHUD->b_battleTab->y=96;
        }
    }
}
