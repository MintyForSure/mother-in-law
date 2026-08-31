//
// Created by tailofhell on 8/24/26.
//

#include <iostream>
#include <nds.h>
#include <nds/arm9/video.h>
#include <ulib/ulib.h>
#include "battleIcons_png.h"
#include "battleTab_png.h"
#include "game.h"
#include "numbers_png.h"
#include "battle.h"


#include "hud.h"
#include "menus.h"

using namespace std;

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
    UL_IMAGE *b_numbers;
};

static struct battleElem bHUD;

void battleHudInit() {
    printf("battle hud initializing\n");
    ulDrawGradientRect(0, 0, 256, 192, RGB15(24, 0, 0), RGB15(0, 0, 0),RGB15(0, 0, 0), RGB15(0, 0, 24));
    //Load battle hud elements
    bHUD.b_iconsFight=ulLoadImageFilePNG(static_cast<const char *>((void*)battleIcons_png),(int)battleIcons_png_size,UL_IN_VRAM,UL_PF_PAL4);
    bHUD.b_iconsSkill=ulLoadImageFilePNG(static_cast<const char *>((void*)battleIcons_png),(int)battleIcons_png_size,UL_IN_VRAM,UL_PF_PAL4);
    bHUD.b_iconsItem=ulLoadImageFilePNG(static_cast<const char *>((void*)battleIcons_png),(int)battleIcons_png_size,UL_IN_VRAM,UL_PF_PAL4);
    bHUD.b_iconsDefend=ulLoadImageFilePNG(static_cast<const char *>((void*)battleIcons_png),(int)battleIcons_png_size,UL_IN_VRAM,UL_PF_PAL4);

    bHUD.b_battleTab=ulLoadImageFilePNG(static_cast<const char *>((void*)battleTab_png),(int)battleTab_png_size,UL_IN_VRAM,UL_PF_PAL4);
    bHUD.b_numbers=ulLoadImageFilePNG(static_cast<const char *>((void*)numbers_png),numbers_png_size,UL_IN_VRAM,UL_PF_PAL4);
    ulSetImageTileSize(bHUD.b_iconsFight,0,16,16,16);
    ulSetImageTileSize(bHUD.b_iconsItem,16,16,16,16);
    ulSetImageTileSize(bHUD.b_iconsSkill,32,16,16,16);
    ulSetImageTileSize(bHUD.b_iconsDefend,48,16,16,16);
    ulImageSetRotCenter(bHUD.b_battleTab);
}

int mTics = 0;
int mapleHPTicker[]={0,0,0};
int animSpeed=2;
int hpHandler(char member) {
    //cout<<(mapleHP[1]/10)%10<<endl;
    if (member=='m') {
        ulSetImageTileSize(bHUD.b_numbers,0,(((mapleHP[1]/100)%10)*8),6,8);
        ulDrawImageXY(bHUD.b_numbers,bHUD.b_battleTab->x-4,167);
        ulSetImageTileSize(bHUD.b_numbers,0,(((mapleHP[1]/10)%10)*8),6,8);
        ulDrawImageXY(bHUD.b_numbers,bHUD.b_battleTab->x+4,167);
        ulSetImageTileSize(bHUD.b_numbers,0,(((mapleHP[1]/1)%10)*8),6,8);
        ulDrawImageXY(bHUD.b_numbers,bHUD.b_battleTab->x+12,167);
        mTics++;
        //cout << mTics <<endl;
        if (mapleHP[0] < mapleHP[1]) {
            //cout<<(mapleHP[1]/10)%10<<endl;
            //ulSetImageTileSize(bHUD.b_numbers,0,(((mapleHP[1]/10)%10)*8),6,8);
            if (mTics % 6==1) {
                //cout << mapleHP[1] << "\n";
                mapleHP[1]--;
            }
        }
        else if (mapleHP[0]>mapleHP[1]) {
            if (mTics % 4==1) {
                //cout << mapleHP[1] << "\n";
                mapleHP[1]++;
            }
        }
        else {
            mTics=0;
        }
    }
    else if (member=='a') {
        cout << "ehhh whatever";
    }
    return 0;
}

void hudRender(char hudType) {
    ulDrawGradientRect(0, 0, 256, 192, RGB15(24, 0, 28), RGB15(0, 0, 0),RGB15(0, 0, 0), RGB15(0, 0, 24));
    switch (hudType) { //i love switch cases
        case 'b':
            //ulDrawFillRect(0,0,256,30,RGB15(0,0,0));
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
                    hpHandler('m');
                    //ulSetImageTileSize(bHUD.b_numbers,0,0,6,8);
                    //ulDrawString(124,167,reinterpret_cast<const char *>(mapleHP[1]));
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
                if (ul_keys.pressed.B) {
                    hSelected=0;
                }
                switch (hSelected) {
                    case 0:
                        ulSetImageTileSize(bHUD.b_iconsFight,0,0,16,16); //focus
                        ulDrawImageXY(bHUD.b_iconsFight,16,hudPos);
                        ulSetTextColor(RGB15(31,31,31));
                        menuInput(true,false);
                        ulDrawString(160,hudPos+6,"Fight");
                        if (ul_keys.pressed.left) {
                            hSelected=3;
                        }
                        else if (ul_keys.pressed.A) {
                            battleMenuState=1; //fight
                            cout << "fight selected\n";
                            break;
                        }
                        break;
                    case 1:
                        ulSetImageTileSize(bHUD.b_iconsItem,16,0,16,16); //focus
                        ulDrawImageXY(bHUD.b_iconsItem,32,hudPos);
                        menuInput(false,false);
                        ulSetTextColor(RGB15(31,31,31));
                        ulDrawString(160,hudPos+6,"Item");
                        break;
                    case 2: //skill/psi
                        ulSetImageTileSize(bHUD.b_iconsSkill,32,0,16,16); //focus
                        ulDrawImageXY(bHUD.b_iconsSkill,48,hudPos);
                        menuInput(false,false);
                        ulSetTextColor(RGB15(31,31,31));
                        ulDrawString(160,hudPos+6,"Skill");
                        break;
                    case 3: //guard/defend
                        ulSetImageTileSize(bHUD.b_iconsDefend,48,0,16,16);
                        ulDrawImageXY(bHUD.b_iconsDefend,64,hudPos);
                        menuInput(false,true);
                        ulSetTextColor(RGB15(31,31,31));
                        ulDrawString(160,hudPos+6,"Defend");
                        if (ul_keys.pressed.right) {
                            hSelected=0;
                        }
                        break;
                    default:
                        hSelected=0;
                        cout << "fallback \n";
                }
            }
            else if (battleMenuState==1) {
                if (ul_keys.pressed.B) {
                    battleMenuState=0;
                }
            }
    }
}
