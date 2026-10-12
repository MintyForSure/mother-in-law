//
// Created by tailofhell on 8/24/26.
//

#include <iostream>
#include <nds.h>
#include <nds/arm9/video.h>
#include "../battle/battle.h"
#include "game.h"
#include <nf_lib.h>
#include <maxmod9.h>
#include "nitrofs/soundbank_info.h"
//#include "soundbank_bin.h"
#include "hud.h"

#include <cmath>

#include "menus.h"
#include "window.h"
#include "../data/enemyData.h"

using namespace std;

int hSelected=0; //0:fight 1:item 2:skill 3:whatever
int vSelected=0;
int battleMenuState=2; //2: starting window 0:menupick 1:fightpick
s16 hudPos=8;
s16 turnHolderPos=179+128;
s16 enemyTurnHolderPos=179+128;
s16 mapleBustPos=192;
s16 crusherBustPos=192;
bool doDrawing;

struct battleElem{

};

static struct battleElem bHUD;

void battleHudInit() {
    printf("battle hud initializing\n");
    //ulDrawGradientRect(0, 0, 256, 192, RGB15(24, 0, 0), RGB15(0, 0, 0),RGB15(0, 0, 0), RGB15(0, 0, 24));
    //Load battle hud elements
    // bHUD.b_iconsFight=ulLoadImageFilePNG(static_cast<const char *>((void*)battleIcons_png),(int)battleIcons_png_size,UL_IN_VRAM,UL_PF_PAL4);
    // bHUD.b_iconsSkill=ulLoadImageFilePNG(static_cast<const char *>((void*)battleIcons_png),(int)battleIcons_png_size,UL_IN_VRAM,UL_PF_PAL4);
    // bHUD.b_iconsItem=ulLoadImageFilePNG(static_cast<const char *>((void*)battleIcons_png),(int)battleIcons_png_size,UL_IN_VRAM,UL_PF_PAL4);
    // bHUD.b_iconsDefend=ulLoadImageFilePNG(static_cast<const char *>((void*)battleIcons_png),(int)battleIcons_png_size,UL_IN_VRAM,UL_PF_PAL4);
    //
    // bHUD.b_turnIcons=ulLoadImageFilePNG(reinterpret_cast<const char *>(pressTurnIcons_png),pressTurnIcons_png_size,UL_IN_VRAM,UL_PF_PAL4);
    // bHUD.b_enemyTurnIcons=ulLoadImageFilePNG(reinterpret_cast<const char *>(pressTurnIcons_png),pressTurnIcons_png_size,UL_IN_VRAM,UL_PF_PAL4);
    // bHUD.b_turnHolder=ulLoadImageFilePNG(reinterpret_cast<const char *>(turnHolder_png),turnHolder_png_size,UL_IN_VRAM,UL_PF_PAL4);
    // bHUD.b_enemyTurnHolder=ulLoadImageFilePNG(reinterpret_cast<const char *>(turnHolderEnemy_png),turnHolderEnemy_png_size,UL_IN_VRAM,UL_PF_PAL4);
    //
    // bHUD.b_mapleBattleTab=ulLoadImageFilePNG(static_cast<const char *>((void*)battleTab_png),(int)battleTab_png_size,UL_IN_VRAM,UL_PF_PAL4);
    // bHUD.b_crusherBattleTab=ulLoadImageFilePNG(static_cast<const char *>((void*)battleTab_png),(int)battleTab_png_size,UL_IN_VRAM,UL_PF_PAL4);
    // bHUD.b_crusherBattleTab=ulLoadImageFilePNG(static_cast<const char *>((void*)battleTab_png),(int)battleTab_png_size,UL_IN_VRAM,UL_PF_PAL4);
    //
    // bHUD.b_numbers=ulLoadImageFilePNG(static_cast<const char *>((void*)numbers_png),numbers_png_size,UL_IN_VRAM,UL_PF_PAL4);
    //
    // bHUD.maple=ulLoadImageFilePNG(reinterpret_cast<const char *>(maple_png),static_cast<int>(maple_png_size),UL_IN_VRAM,UL_PF_PAL4);
    // bHUD.crusher=ulLoadImageFilePNG(reinterpret_cast<const char *>(crusher_png),static_cast<int>(crusher_png_size),UL_IN_VRAM,UL_PF_PAL4);

    NF_LoadSpriteGfx("gfx/battleOptions",1,64,32);
    NF_LoadSpritePal("gfx/battleOptions",1);

    NF_VramSpriteGfx(1,1,1,false);
    NF_VramSpritePal(1,1,1);

    NF_CreateSprite(1,9,1,1,192*2,16);
    NF_CreateSprite(1,10,1,1,192*2,16);
    NF_CreateSprite(1,11,1,1,192*2,16);
    NF_CreateSprite(1,12,1,1,192*2,16);


    NF_LoadTextFont16("fnt/font16","down",256,256,0);
    NF_CreateTextLayer16(1,0,0,"down");
}

static int mHPTics = 0;
static int mHPVis=mapleHP[1];
static int mPPTics = 0;

static int cHPTics=0;
static int cPPTics=0;

//static int mapleHPTicker[]={0,0,0};
int animSpeed=2;

static int hpHandler(char member) {
    //cout<<mHPVis<<endl;
    if (member=='m') {
        if (((mapleHP[2]/100)%10)*8==0) {

        }
        else {

        }

        mHPTics++;
        //cout << mTics <<endl;
        if (mapleHP[0] < mapleHP[1]) {
            if (mapleDefending==true && mHPTics % 10==1) {
                //cout << mapleHP[1] << "\n";
                mapleHP[1]--;
            }
            else if (mHPTics % 6==1) {
                mapleHP[1]--;
            }
        }
        else if (mapleHP[0]>mapleHP[1]) {
            if (mHPTics % 4==1) {
                //cout << mapleHP[1] << "\n";
                mapleHP[1]++;
            }
        }
        else {
            mHPTics=0;
        }
        if (mHPTics==80) {
            mHPTics=0;
        }
    }
    else if (member=='a') {
        cout << "ehhh whatever";
    }
    else if (member=='c') {
        if (((crusherHP[2]/100)%10)*8==0) {

        }
        else {

        }

        cHPTics++;
        //cout << mTics <<endl;
        if (crusherHP[0] < crusherHP[1]) {
            if (cHPTics % 6==1) {
                //cout << mapleHP[1] << "\n";
                mapleHP[1]--;
            }
        }
        else if (crusherHP[0]>crusherHP[1]) {
            if (cHPTics % 4==1) {
                //cout << mapleHP[1] << "\n";
                crusherHP[1]++;
            }
        }
        else {
            cHPTics=0;
        }
        if (cHPTics==80) {
            cHPTics=0;
        }
    }
    return 0;
}

static int ppHandler(char member) { //please be mature about this

}

void hudShake(std::string member, int intensity) {
    if (member=="maple") {

    }
}

void hudRender(char hudType) {

}

void hudRenderSub(const string& hudType) {
    if (hudType=="battle") {
        NF_MoveSprite(1,9,0,16-8);
        NF_MoveSprite(1,10,0,64-8);
        NF_MoveSprite(1,11,0,112-8);
        NF_MoveSprite(1,12,0,160-8);

        NF_SpriteFrame(1,9,1);
        NF_SpriteFrame(1,10,1);
        NF_SpriteFrame(1,11,1);
        NF_SpriteFrame(1,12,1);

        NF_WriteText16(1,0,2,2,"Fight");
        switch (vSelected) {
            case 0:
                NF_SpriteFrame(1,9,0);
                NF_MoveSprite(1,9,64,16-8);
                break;
            case 1:
                NF_SpriteFrame(1,10,0);
                NF_MoveSprite(1,10,64,64-8);
                break;
        }
    }
}