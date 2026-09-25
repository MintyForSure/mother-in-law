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

#include <cmath>

#include "menus.h"
#include "window.h"
#include "../data/enemyData.h"
#include "characters/maple_png.h"
#include "ui/pressTurnIcons_png.h"
#include "ui/turnHolder_png.h"

using namespace std;

int hSelected=0; //0:fight 1:item 2:skill 3:whatever
int vSelected=0;
int battleMenuState=2; //2: starting window 0:menupick 1:fightpick
s16 hudPos=8;
s16 mapleBustPos=192;
s16 crusherBustPos=192;
bool doDrawing;

struct battleElem{
    UL_IMAGE *b_iconsFight;
    UL_IMAGE *b_iconsSkill;
    UL_IMAGE *b_iconsItem;
    UL_IMAGE *b_iconsDefend;

    UL_IMAGE *b_turnIcons;
    UL_IMAGE *b_enemyTurnIcons;
    UL_IMAGE *b_turnHolder;

    UL_IMAGE *b_mapleBattleTab;
    UL_IMAGE *b_ashtonBattleTab;
    UL_IMAGE *b_crusherBattleTab;

    UL_IMAGE *b_numbers;
    UL_IMAGE *maple;
    UL_IMAGE *ashton;
    UL_IMAGE *crusher;
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

    bHUD.b_turnIcons=ulLoadImageFilePNG(reinterpret_cast<const char *>(pressTurnIcons_png),pressTurnIcons_png_size,UL_IN_VRAM,UL_PF_PAL4);
    bHUD.b_enemyTurnIcons=ulLoadImageFilePNG(reinterpret_cast<const char *>(pressTurnIcons_png),pressTurnIcons_png_size,UL_IN_VRAM,UL_PF_PAL4);
    bHUD.b_turnHolder=ulLoadImageFilePNG(reinterpret_cast<const char *>(turnHolder_png),turnHolder_png_size,UL_IN_VRAM,UL_PF_PAL4);

    bHUD.b_mapleBattleTab=ulLoadImageFilePNG(static_cast<const char *>((void*)battleTab_png),(int)battleTab_png_size,UL_IN_VRAM,UL_PF_PAL4);
    bHUD.b_crusherBattleTab=ulLoadImageFilePNG(static_cast<const char *>((void*)battleTab_png),(int)battleTab_png_size,UL_IN_VRAM,UL_PF_PAL4);
    bHUD.b_crusherBattleTab=ulLoadImageFilePNG(static_cast<const char *>((void*)battleTab_png),(int)battleTab_png_size,UL_IN_VRAM,UL_PF_PAL4);

    bHUD.b_numbers=ulLoadImageFilePNG(static_cast<const char *>((void*)numbers_png),numbers_png_size,UL_IN_VRAM,UL_PF_PAL4);

    bHUD.maple=ulLoadImageFilePNG(maple_png,static_cast<int>(maple_png_size),UL_IN_VRAM,UL_PF_PAL4);
    bHUD.crusher=ulLoadImageFilePNG(maple_png,static_cast<int>(maple_png_size),UL_IN_VRAM,UL_PF_PAL4);
    ulSetImageTileSize(bHUD.maple,16,0,16,32);
    ulImageSetRotCenter(bHUD.maple);

    ulSetImageTileSize(bHUD.b_iconsFight,0,16,16,16);
    ulSetImageTileSize(bHUD.b_iconsItem,16,16,16,16);
    ulSetImageTileSize(bHUD.b_iconsSkill,32,16,16,16);
    ulSetImageTileSize(bHUD.b_iconsDefend,48,16,16,16);

    ulSetImageTileSize(bHUD.b_turnIcons,0,0,16,16);
    ulSetImageTileSize(bHUD.b_enemyTurnIcons,16,0,16,16);

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
    ulImageSetRotCenter(bHUD.b_mapleBattleTab);
    ulImageSetRotCenter(bHUD.b_crusherBattleTab);
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
            ulSetImageTileSize(bHUD.b_numbers,0,(((mapleHP[1]/100)%10)*8),6,8);
        }
        else {
            ulSetImageTileSize(bHUD.b_numbers,0,(((mapleHP[1]/100)%10)*8),6,8);
            ulDrawImageXY(bHUD.b_numbers,bHUD.b_mapleBattleTab->x-4,167);
        }
        ulSetImageTileSize(bHUD.b_numbers,0,(((mapleHP[1]/10)%10)*8),6,8); //Max HP will never be in the single digits, lol.
        ulDrawImageXY(bHUD.b_numbers,bHUD.b_mapleBattleTab->x+4,167);
        //ulSetImageTileSize(bHUD.b_numbers,0,lerp(mapleHP[1],mapleHP[1]*5,0.1f),6,8);
        ulSetImageTileSize(bHUD.b_numbers,0,(((mapleHP[1]/1)%10)*8),6,8);
        ulDrawImageXY(bHUD.b_numbers,bHUD.b_mapleBattleTab->x+12,167);
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
    else if (member=='c') {
        if (((crusherHP[2]/100)%10)*8==0) {
            ulSetImageTileSize(bHUD.b_numbers,0,(((crusherHP[1]/100)%10)*8),6,8);
        }
        else {
            ulSetImageTileSize(bHUD.b_numbers,0,(((crusherHP[1]/100)%10)*8),6,8);
            ulDrawImageXY(bHUD.b_numbers,bHUD.b_crusherBattleTab->x-4,167);
        }
        ulSetImageTileSize(bHUD.b_numbers,0,(((crusherHP[1]/10)%10)*8),6,8); //Max HP will never be in the single digits, lol.
        ulDrawImageXY(bHUD.b_numbers,bHUD.b_crusherBattleTab->x+4,167);
        //ulSetImageTileSize(bHUD.b_numbers,0,lerp(mapleHP[1],mapleHP[1]*5,0.1f),6,8);
        ulSetImageTileSize(bHUD.b_numbers,0,(((crusherHP[1]/1)%10)*8),6,8);
        ulDrawImageXY(bHUD.b_numbers,bHUD.b_crusherBattleTab->x+12,167);
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
    //cout<<(mapleHP[1]/10)%10<<endl;
    if (member=='m') {
        if (((maplePP[2]/100)%10)*8==0) {
            ulSetImageTileSize(bHUD.b_numbers,0,(((maplePP[1]/100)%10)*8),6,8);
        }
        else {
            ulSetImageTileSize(bHUD.b_numbers,0,(((maplePP[1]/100)%10)*8),6,8);
            ulDrawImageXY(bHUD.b_numbers,bHUD.b_mapleBattleTab->x-4,167+11);
        }
        ulSetImageTileSize(bHUD.b_numbers,0,(((maplePP[1]/10)%10)*8),6,8); //ditto
        ulDrawImageXY(bHUD.b_numbers,bHUD.b_mapleBattleTab->x+4,167+11);
        ulSetImageTileSize(bHUD.b_numbers,0,(((maplePP[1]/1)%10)*8),6,8);
        ulDrawImageXY(bHUD.b_numbers,bHUD.b_mapleBattleTab->x+12,167+11);
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
    else if (member=='c') {

        if (((crusherPP[2]/100)%10)*8==0) {
            ulSetImageTileSize(bHUD.b_numbers,0,(((crusherPP[1]/100)%10)*8),6,8);
        }
        else {
            ulSetImageTileSize(bHUD.b_numbers,0,(((crusherPP[1]/100)%10)*8),6,8);
            ulDrawImageXY(bHUD.b_numbers,bHUD.b_crusherBattleTab->x-4,167+11);
        }
        ulSetImageTileSize(bHUD.b_numbers,0,(((crusherPP[1]/10)%10)*8),6,8); //Max HP will never be in the single digits, lol.
        ulDrawImageXY(bHUD.b_numbers,bHUD.b_crusherBattleTab->x+4,167+11);
        //ulSetImageTileSize(bHUD.b_numbers,0,lerp(mapleHP[1],mapleHP[1]*5,0.1f),6,8);
        ulSetImageTileSize(bHUD.b_numbers,0,(((crusherPP[1]/1)%10)*8),6,8);
        ulDrawImageXY(bHUD.b_numbers,bHUD.b_crusherBattleTab->x+12,167+11);
        cHPTics++;
        //cout << mTics <<endl;
        if (crusherPP[0] < crusherPP[1]) {
            if (cPPTics % 6==1) {
                //cout << mapleHP[1] << "\n";
                mapleHP[1]--;
            }
        }
        else if (crusherPP[0]>crusherPP[1]) {
            if (cHPTics % 4==1) {
                //cout << mapleHP[1] << "\n";
                crusherPP[1]++;
            }
        }
        else {
            cPPTics=0;
        }
        if (cPPTics==80) {
            cPPTics=0;
        }
    }
    return 0;
}

void hudRender(char hudType) {
    switch (hudType) { //i love switch cases
        case 'b':
            ulDrawImageXY(bHUD.b_turnHolder,179,62);
            if (battlePhase==0 or battlePhase==1) {
                for (int i=0; i<pressTurnCount(true); i++) {
                    ulDrawImageXY(bHUD.b_turnIcons,188+(i*16),62-16);
                }
            }
            else if (battlePhase==2) {
                for (int i=0; i<pressTurnCount(false); i++) {
                    ulDrawImageXY(bHUD.b_enemyTurnIcons,188+(i*16),62-16);
                }
            }
            ulSetImageTileSize(bHUD.b_iconsFight,0,16,16,16);
            ulSetImageTileSize(bHUD.b_iconsItem,16,16,16,16);
            ulSetImageTileSize(bHUD.b_iconsSkill,32,16,16,16);
            ulSetImageTileSize(bHUD.b_iconsDefend,48,16,16,16);

            switch (partyMemberCount) {
                case 1:
                    ulSetTextColor(RGB15(0,0,0));
                    ulDrawImageXY(bHUD.maple,bHUD.b_mapleBattleTab->x,mapleBustPos);
                    ulDrawImageXY(bHUD.b_mapleBattleTab,128,192-32);
                    ulDrawString(112,154,"Maple");
                    hpHandler('m');
                    ppHandler('m');
                    //ulSetImageTileSize(bHUD.b_numbers,0,0,6,8);
                    //ulDrawString(124,167,reinterpret_cast<const char *>(mapleHP[1]));
                    break;
                case 2:
                    ulSetTextColor(RGB15(0,0,0));
                    ulDrawImageXY(bHUD.maple,bHUD.b_mapleBattleTab->x,mapleBustPos);
                    ulDrawImageXY(bHUD.crusher,bHUD.b_crusherBattleTab->x,crusherBustPos);
                    ulDrawImageXY(bHUD.b_mapleBattleTab,67+(58/2),192-32);
                    ulDrawImageXY(bHUD.b_crusherBattleTab,131+(58/2),192-32);

                    if (partyMembers[0]=="maple") {
                        ulDrawString(bHUD.b_mapleBattleTab->x-16,bHUD.b_mapleBattleTab->y-6,"Maple");
                        hpHandler('m');
                        ppHandler('m');
                    }
                    if (partyMembers[1]=="crusher") {
                        ulDrawString(bHUD.b_crusherBattleTab->x-20,bHUD.b_crusherBattleTab->y-6,"Crusher");
                        hpHandler('c');
                        ppHandler('c');
                    }
                    break;
                default:
                    //cout<<partyMemberCount<<endl;
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
                if (selectingPartyMember=="maple") {
                    mapleBustPos=lerp(mapleBustPos,192-(64-16),0.5f);
                }
                else {
                    mapleBustPos=lerp(192-(64-16),mapleBustPos,-1.0f);
                }
                switch (hSelected) {
                    case 0:
                        ulSetImageTileSize(bHUD.b_iconsFight,0,0,16,16); //focus
                        ulDrawImageXY(bHUD.b_iconsFight,16,hudPos);
                        menuInput(true,false);
                        //ulDrawString(160,hudPos+6,"Fight");
                        if (ul_keys.pressed.left) {
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
                    if (selectingPartyMember=="maple") {
                        mapleAction[0]="maple";
                        mapleAction[1]="enemy0";
                        mapleAction[2]="bash";
                        battlePhase=1;
                        playerMove("maple");
                        battlePhase=1;
                    }
                    if (selectingPartyMember=="crusher") {
                        playerMove("crusher");
                        battlePhase=1;
                    }
                    battleMenuState=3;
                }
            }
            else if (battleMenuState==2) {
                drawWindow("enemyAppear",8,8,252-32,16,"normal");
                windowDisplayText("The "+getEnemyName(enemyList[0])+" appeared!","This is a test line!");
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