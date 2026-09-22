//
// Created by tailofhell on 8/24/26.
//

#include <iostream>
#include <nds.h>
#include <nds/arm9/video.h>
#include <ulib/ulib.h>
#include "battle.h"
#include "ui/battleIcons_png.h"
#include "ui/battleTab_png.h"
#include "game.h"
#include "ui/numbers_png.h"
#include <nf_lib.h>
#include <maxmod9.h>
#include "soundbank.h"
//#include "soundbank_bin.h"
#include "hud.h"
#include "menus.h"
#include "window.h"
#include "sound.h"

using namespace std;

int hSelected=0; //0:fight 1:item 2:skill 3:whatever
int vSelected=0;
int battleMenuState=2; //2: starting window 0:menupick 1:fightpick
s16 hudPos=8;
int partyMemberCount=1;
bool doDrawing;

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
    //ulDrawGradientRect(0, 0, 256, 192, RGB15(24, 0, 0), RGB15(0, 0, 0),RGB15(0, 0, 0), RGB15(0, 0, 24));
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
    ulImageSetRotCenter(bHUD.b_battleTab);
}

static int mHPTics = 0;
static int mHPVis=mapleHP[1];
static int mPPTics = 0;
//static int mapleHPTicker[]={0,0,0};
int animSpeed=2;

static int hpHandler(char member) {
    //cout<<mHPVis<<endl;
    if (member=='m') {
        if (((mapleHP[2]/100)%10)*8==0) {
            ulSetImageTileSize(bHUD.b_numbers,0,(((mapleHP[1]/100)%10)*8),6,8);
        }
        else {
            ulSetImageTileSize(bHUD.b_numbers,0,(((mapleHP[1]/100)%10)*8),6,8);
            ulDrawImageXY(bHUD.b_numbers,bHUD.b_battleTab->x-4,167);
        }
        ulSetImageTileSize(bHUD.b_numbers,0,(((mapleHP[1]/10)%10)*8),6,8); //Max HP will never be in the single digits, lol.
        ulDrawImageXY(bHUD.b_numbers,bHUD.b_battleTab->x+4,167);
        ulSetImageTileSize(bHUD.b_numbers,0,(((mapleHP[1]/1)%10)*8),6,8);
        ulDrawImageXY(bHUD.b_numbers,bHUD.b_battleTab->x+12,167);
        mHPTics++;
        //cout << mTics <<endl;
        if (mapleHP[0] < mapleHP[1]) {
            if (mHPTics % 6==1) {
                //cout << mapleHP[1] << "\n";
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
    return 0;
}

static int ppHandler(char member) {
    //cout<<(mapleHP[1]/10)%10<<endl;
    if (member=='m') {
        if (((maplePP[2]/100)%10)*8==0) {
            ulSetImageTileSize(bHUD.b_numbers,0,(((maplePP[1]/100)%10)*8),6,8);
        }
        else {
            ulSetImageTileSize(bHUD.b_numbers,0,(((maplePP[1]/100)%10)*8),6,8);
            ulDrawImageXY(bHUD.b_numbers,bHUD.b_battleTab->x-4,167+11);
        }
        ulSetImageTileSize(bHUD.b_numbers,0,(((maplePP[1]/10)%10)*8),6,8); //ditto
        ulDrawImageXY(bHUD.b_numbers,bHUD.b_battleTab->x+4,167+11);
        ulSetImageTileSize(bHUD.b_numbers,0,(((maplePP[1]/1)%10)*8),6,8);
        ulDrawImageXY(bHUD.b_numbers,bHUD.b_battleTab->x+12,167+11);
        mPPTics++;
        //cout << mTics <<endl;
        if (maplePP[0] < maplePP[1]) {
            //cout<<(mapleHP[1]/10)%10<<endl;
            //ulSetImageTileSize(bHUD.b_numbers,0,(((mapleHP[1]/10)%10)*8),6,8);
            if (mPPTics % 6==1) {
                //cout << mapleHP[1] << "\n";
                maplePP[1]--;
            }
        }
        else if (maplePP[0]>maplePP[1]) {
            if (mPPTics % 4==1) {
                //cout << mapleHP[1] << "\n";
                maplePP[1]++;
            }
        }
        else {
            mPPTics=0;
        }
    }
    else if (member=='a') {
        cout << "ehhh whatever";
    }
    return 0;
}

void hudRender(char hudType) {
    switch (hudType) { //i love switch cases
        case 'b':
            //NFdrawWindow(64,64,32,32,"popup");
            //NFdrawWindow(32,32,32,32,"popup");
            ulSetImageTileSize(bHUD.b_iconsFight,0,16,16,16);
            ulSetImageTileSize(bHUD.b_iconsItem,16,16,16,16);
            ulSetImageTileSize(bHUD.b_iconsSkill,32,16,16,16);
            ulSetImageTileSize(bHUD.b_iconsDefend,48,16,16,16);

            switch (partyMemberCount) {
                case 1:
                    ulSetTextColor(RGB15(0,0,0));
                    ulDrawImageXY(bHUD.b_battleTab,128,192-32);
                    ulDrawString(112,154,"Maple");
                    hpHandler('m');
                    ppHandler('m');
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
                        menuInput(true,false);
                        //ulDrawString(160,hudPos+6,"Fight");
                        if (ul_keys.pressed.left) {
                            cout<<"oh god it hurts";
                            mmEffect(SFX_HSELECT);
                            hSelected=3;
                        }
                        else if (ul_keys.pressed.A) {
                            mmEffect(SFX_SELECT);
                            battleMenuState=1; //fight
                            cout << "fight selected\n";
                            break;
                        }
                        drawWindow("Fight",156,18,128,4,"popup");
                        windowDisplayText("Fight");
                        break;
                    case 1:
                        ulSetImageTileSize(bHUD.b_iconsItem,16,0,16,16); //focus
                        ulDrawImageXY(bHUD.b_iconsItem,32,hudPos);
                        menuInput(false,false);
                        //ulDrawString(160,hudPos+6,"Item");
                        drawWindow("Goods",156,18,128,4,"popup");
                        windowDisplayText("Goods");
                        if (ul_keys.pressed.A) {
                            mmEffect(SFX_SELECT);

                        }
                        break;
                    case 2: //skill/psi
                        ulSetImageTileSize(bHUD.b_iconsSkill,32,0,16,16); //focus
                        ulDrawImageXY(bHUD.b_iconsSkill,48,hudPos);
                        menuInput(false,false);
                        //ulDrawString(160,hudPos+6,"Skill");
                        drawWindow("Skill",156,18,128,4,"popup");
                        windowDisplayText("PSI");
                        break;
                    case 3: //guard/defend
                        ulSetImageTileSize(bHUD.b_iconsDefend,48,0,16,16);
                        ulDrawImageXY(bHUD.b_iconsDefend,64,hudPos);
                        menuInput(false,true);
                        //ulDrawString(160,hudPos+6,"Defend");
                        if (ul_keys.pressed.right) {
                            mmEffect(SFX_HSELECT);
                            hSelected=0;
                        }
                        drawWindow("Guard",156,18,128,4,"popup");
                        windowDisplayText("Guard");
                        break;
                    default:
                        hSelected=0;
                        cout << "fallback \n";
                }
            }
            else if (battleMenuState==1) {
                if (ul_keys.pressed.B) {
                    mmEffect(SFX_DESELECT);
                    battleMenuState=0;
                }
                else if (ul_keys.pressed.A) {
                    mmEffect(SFX_SELECT);
                    mapleAction[0]="maple";
                    mapleAction[1]="enemy0";
                    mapleAction[2]="bash";
                    battlePhase=1;
                    battleMenuState=3;
                }
            }
            else if (battleMenuState==2) {
                drawWindow("enemyAppear",8,8,252-32,16,"normal");
                windowDisplayText("The Cheesy Rat appeared!","This is a test line! Buenos dias!");
                 if (doWindowDrawing==false) {
                     battleMenuState=0;
                     doWindowDrawing=true;
                }
            }
    }
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