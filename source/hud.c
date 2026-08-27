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

#include "menus.h"
#include "../../../../../opt/wonderful/thirdparty/blocksds/external/ulibrary/include/ulib/ulib.h"
int hSelected=0; //0:fight 1:item 2:skill 3:whatever
int vSelected=0;
int battleMenuState=0; //0:menupick 1:fightpick
s16 hudPos=8;

struct battleElem{
    UL_IMAGE *b_iconsFight;
    UL_IMAGE *b_iconsSkill;
    UL_IMAGE *b_iconsItem;
    UL_IMAGE *b_iconsDefend;

    UL_IMAGE *b_battleTab;
};

static struct battleElem bHUD;

void battleHudInit() {
    printf("battle hud initializing\n");
    //Load battle hud elements
    bHUD.b_iconsFight=ulLoadImageFilePNG((void*)battleIcons_png,(int)battleIcons_png_size,UL_IN_VRAM,UL_PF_PAL4);
    bHUD.b_iconsSkill=ulLoadImageFilePNG((void*)battleIcons_png,(int)battleIcons_png_size,UL_IN_VRAM,UL_PF_PAL4);
    bHUD.b_iconsItem=ulLoadImageFilePNG((void*)battleIcons_png,(int)battleIcons_png_size,UL_IN_VRAM,UL_PF_PAL4);
    bHUD.b_iconsDefend=ulLoadImageFilePNG((void*)battleIcons_png,(int)battleIcons_png_size,UL_IN_VRAM,UL_PF_PAL4);

    bHUD.b_battleTab=ulLoadImageFilePNG((void*)battleTab_png,(int)battleTab_png_size,UL_IN_VRAM,UL_PF_PAL4);
    ulSetImageTileSize(bHUD.b_iconsFight,0,16,16,16);
    ulSetImageTileSize(bHUD.b_iconsItem,16,16,16,16);
    ulSetImageTileSize(bHUD.b_iconsSkill,32,16,16,16);
    ulSetImageTileSize(bHUD.b_iconsDefend,48,16,16,16);
    ulImageSetRotCenter(bHUD.b_battleTab);
}
void hudRender(char hudType) {
    switch (hudType) {
        case 'b':
            ulDrawFillRect(0,0,256,30,RGB15(0,0,0));
            ulDrawFillRect(0,162,256,192,RGB15(0,0,0));
            ulSetImageTileSize(bHUD.b_iconsFight,0,16,16,16);
            ulSetImageTileSize(bHUD.b_iconsItem,16,16,16,16);
            ulSetImageTileSize(bHUD.b_iconsSkill,32,16,16,16);
            ulSetImageTileSize(bHUD.b_iconsDefend,48,16,16,16);
            switch (partyMembers) {
                case 1:
                    ulSetTextColor(RGB15(0,0,0));
                    ulDrawImageXY(bHUD.b_battleTab,128,192-32);
                    ulDrawString(112,154,"Maple");
                    break;
                default:
                    printf("how do you have that many party members? \n");
                    break;
            }
            if (battleMenuState==0){
                ulDrawImageXY(bHUD.b_iconsFight,16,hudPos);
                ulDrawImageXY(bHUD.b_iconsItem,32,hudPos);
                ulDrawImageXY(bHUD.b_iconsSkill,48,hudPos);
                ulDrawImageXY(bHUD.b_iconsDefend,64,hudPos);
                switch (hSelected) {
                    case 0:
                        ulSetImageTileSize(bHUD.b_iconsFight,0,0,16,16); //focus
                        ulDrawImageXY(bHUD.b_iconsFight,16,hudPos);
                        ulSetTextColor(RGB15(31,31,31));
                        menuInput(true,false);
                        ulDrawString(16,36,"Fight");
                        if (ul_keys.pressed.left) {
                            hSelected=3;
                        }
                        else if (ul_keys.pressed.A) {
                            battleMenuState=1; //fight
                            break;
                        }
                        break;
                    case 1:
                        ulSetImageTileSize(bHUD.b_iconsItem,16,0,16,16); //focus
                        ulDrawImageXY(bHUD.b_iconsItem,32,hudPos);
                        menuInput(false,false);
                        ulSetTextColor(RGB15(31,31,31));
                        ulDrawString(16,36,"Item");
                        break;
                    case 2: //skill/psi
                        ulSetImageTileSize(bHUD.b_iconsSkill,32,0,16,16); //focus
                        ulDrawImageXY(bHUD.b_iconsSkill,48,hudPos);
                        menuInput(false,false);
                        ulSetTextColor(RGB15(31,31,31));
                        ulDrawString(16,36,"Skill");
                        break;
                    case 3: //guard/defend
                        ulSetImageTileSize(bHUD.b_iconsDefend,48,0,16,16);
                        ulDrawImageXY(bHUD.b_iconsDefend,64,hudPos);
                        menuInput(false,true);
                        ulSetTextColor(RGB15(31,31,31));
                        ulDrawString(16,36,"Defend");
                        if (ul_keys.pressed.right) {
                            hSelected=0;
                        }
                        break;
                    default:
                        hSelected=0;
                        printf("fallback \n");
                }
            }
            else if (battleMenuState==1) {
                if (ul_keys.pressed.B) {
                    battleMenuState=0;
                }
            }
    }
}
