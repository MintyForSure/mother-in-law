//
// Created by tailofhell on 8/24/26.
//

#include <iostream>
#include <nds.h>
#include <nds/arm9/input.h>
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
int battleMenuState=0; //2: starting window 0:menupick 1:fightpick
s16 hudPos=8;
static s16 selectionOffsetX=76;
static s16 selectionOffsetY;
// static s16 turnHolderX=192;
// static s16 enemyTurnHolder
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

    //Initialize the HUD sprites
    //Sprite slot 0-8 belongs to the hud
    NF_LoadSpriteGfx("gfx/ui/numbers1",0,16,16);
    NF_LoadSpritePal("gfx/ui/numbers1",0);
    NF_LoadSpriteGfx("gfx/ui/battleTab",1,64,64);
    NF_LoadSpritePal("gfx/ui/battleTab",1);
    NF_LoadSpriteGfx("gfx/ui/turnHolder",2,64,64);
    NF_LoadSpritePal("gfx/ui/turnHolder",2);
    NF_LoadSpriteGfx("gfx/ui/blackbox", 3, 64, 32);
    NF_LoadSpritePal("gfx/ui/blackbox", 3);
    NF_LoadSpriteGfx("gfx/ui/bBash",4,32,32);
    NF_LoadSpritePal("gfx/ui/bBash",4);
    NF_LoadSpriteGfx("gfx/ui/bPSI",5,32,32);
    NF_LoadSpritePal("gfx/ui/bPSI",5);
    NF_LoadSpriteGfx("gfx/ui/bGuard",6,32,32);
    NF_LoadSpritePal("gfx/ui/bGuard",6);
    NF_LoadSpriteGfx("gfx/ui/bBag",7,32,32);
    NF_LoadSpritePal("gfx/ui/bBag",7);
    NF_LoadSpriteGfx("gfx/ui/turnHolderEnemy",8,64,64);
    NF_LoadSpritePal("gfx/ui/turnHolderEnemy",8);
    NF_LoadSpriteGfx("gfx/ui/featherCursor",9,16,16);
    NF_LoadSpritePal("gfx/ui/featherCursor",9);
    NF_LoadSpriteGfx("gfx/ui/numbers",10,8,8);
    NF_LoadSpritePal("gfx/ui/numbers",10);


    NF_VramSpriteGfx(0,0,0,true);
    NF_VramSpritePal(0,0,0);
    NF_VramSpriteGfx(0,1,1,false);
    NF_VramSpritePal(0,1,1);
    NF_VramSpriteGfx(0,2,2,false);
    NF_VramSpritePal(0,2,2);
    NF_VramSpriteGfx(0, 3, 3, false);
    NF_VramSpritePal(0, 3, 3);
    NF_VramSpriteGfx(0,4,4,false);
    NF_VramSpritePal(0,4,4);
    NF_VramSpriteGfx(0,5,5,false);
    NF_VramSpritePal(0,5,5);
    NF_VramSpriteGfx(0,6,6,false);
    NF_VramSpritePal(0,6,6);
    NF_VramSpriteGfx(0,7,7,false);
    NF_VramSpritePal(0,7,7);
    NF_VramSpriteGfx(0,8,8,false);
    NF_VramSpritePal(0,8,8);
    NF_VramSpriteGfx(0,9,9,false);
    NF_VramSpritePal(0,9,9);
    NF_VramSpriteGfx(0,10,10,false);
    NF_VramSpritePal(0,10,10);

    //Create the sprites :]
    NF_DefineTextColor(0,0,1,0,0,0);
    NF_SetTextColor(0,0,1);
    NF_ScrollBg(0,0,-4,-7);
    switch (partyMemberCount) { //shut up clangd
        case 1:
            NF_CreateSprite(0,1,1,1,(256/2)-32,192-64);
            NF_SpriteLayer(0,1,1);
            //NF_DefineTextColor(0,0,1,0,0,0);
            NF_WriteText16(0,0,4,8,"Maple");
            for (int i=0;i<3;i++) { //HP
                NF_CreateSprite(0, 18+i, 10, 10, 124 + i * 8, 167);
                //NF_SpriteLayer(0, 18+i, 0);
            }
            for (int i=0;i<3;i++) { //PP
                NF_CreateSprite(0, 18+3+i, 10, 10, 124 + i * 8, 178);
            }
            break;
        case 2:
            cout<<"case 2"<<endl;
            NF_CreateSprite(0,1,1,1,64,128);
            NF_SpriteLayer(0,1,1);
            NF_WriteText16(0,0,9,9,"Maple");
            for (int i=0;i<3;i++) { //HP
                NF_CreateSprite(0, 18+i, 10, 10, 92 + i * 8, 167);
                //NF_SpriteLayer(0, 18+i, 0);
            }
            for (int i=0;i<3;i++) { //PP
                NF_CreateSprite(0, 18+3+i, 10, 10, 92 + i * 8, 178);
            }
            NF_CreateSprite(0,25,1,1,128,128);
            NF_SpriteLayer(0,25,1);
            NF_WriteText16(0,0,16,9,"Crusher");
            for (int i=0;i<3;i++) { //HP
                NF_CreateSprite(0, 26+i, 10, 10, 156 + i * 8, 167);
                //NF_SpriteLayer(0, 18+i, 0);
            }
            for (int i=0;i<3;i++) { //PP
                NF_CreateSprite(0, 26+3+i, 10, 10, 156 + i * 8, 178);
            }
            break;
        default:
            cout<<"default"<<endl;
            NF_CreateSprite(0,1,1,1,(256/2)-32,192-64);
            break;
    }
    NF_CreateSprite(0,2,2,2,192,62);
    for (int x = 0; x < 256; x += 64) {
        const int id = 3 + x / 64;
        NF_CreateSprite(0,id,3,3,x,0);       // top bar
        NF_CreateSprite(0,id+4,3,3,x,160); // bottom bar

        NF_SpriteLayer(0,id,3);
        NF_SpriteLayer(0,id+4,3);
        cout<<"id="<<id+4<<endl;
    }
    //NF_SpriteLayer(0,11,0);
    NF_CreateSprite(0,11,4,4,40+selectionOffsetX,32-8); //bash
    NF_CreateSprite(0,12,5,5,16+selectionOffsetX,40);    //
    NF_CreateSprite(0,13,6,6,36+selectionOffsetX,40+8);
    NF_CreateSprite(0,14,7,7,64+selectionOffsetX,39);
    NF_CreateSprite(0,15,8,8,192+64,62);
    NF_CreateSprite(0,16,9,9,-16,-16);
    NF_CreateSprite(0,0,0,0,206,58);
    //Sprite slots 0-32 belong to the Battle HUD
}

static int mHPTics = 0;
static int mHPVis=mapleHP[1];
static int mPPTics = 0;

static int cHPTics=0;
static int cPPTics=0;

//static int mapleHPTicker[]={0,0,0};
int animSpeed=2;

static void hpHandler(char member) {
    switch (member) {
        case 'm': //18,19,20 Maple HP
            NF_SpriteFrame(0,18,mapleHP[1]/100%10);
            NF_SpriteFrame(0,19,mapleHP[1]/10%10);
            NF_SpriteFrame(0,20,mapleHP[1]/1%10);
            break;
        case 'c':
            NF_SpriteFrame(0,26,crusherHP[1]/100%10);
            NF_SpriteFrame(0,27,crusherHP[1]/10%10);
            NF_SpriteFrame(0,28,crusherHP[1]/1%10);
            break;
        default:
            cout<<"default, hphandler"<<endl;
    }
}

static void ppHandler(char member) { //please be mature about this
    switch (member) {
        case 'm': //18,19,20 Maple HP
            NF_SpriteFrame(0,21,maplePP[1]/100%10);
            NF_SpriteFrame(0,22,maplePP[1]/10%10);
            NF_SpriteFrame(0,23,maplePP[1]/1%10);
            break;
        case 'c':
            NF_SpriteFrame(0,29,crusherPP[1]/100%10);
            NF_SpriteFrame(0,30,crusherPP[1]/10%10);
            NF_SpriteFrame(0,31,crusherPP[1]/1%10);
            break;
        default:
            cout<<"default, hphandler"<<endl;
    }
}

void hudShake(std::string member, int intensity) {
    if (member=="maple") {

    }
}

void hudRender(char hudType) {
    switch (hudType) {
        case 'b':
            hpHandler('m');
            ppHandler('m');
            hpHandler('c');
            ppHandler('c');
            NF_MoveSprite(0,11,40+selectionOffsetX,selectionOffsetY+(32-8));
            NF_MoveSprite(0,12,16+selectionOffsetX,selectionOffsetY+40);
            NF_MoveSprite(0,13,36+selectionOffsetX,selectionOffsetY+40+8);
            NF_MoveSprite(0,14,64+selectionOffsetX,selectionOffsetY+39);
            if (battleMenuState==0) {
                selectionOffsetY=lerp(selectionOffsetY,-16,0.25f);
            }
            else {
                selectionOffsetY=lerp(selectionOffsetY,-70,0.25f);
            }
            if (battlePhase==0 or battlePhase==1) {
                enemyTurnHolderPos=192+64;
                turnHolderPos=lerp(turnHolderPos,192,0.25f);
                NF_MoveSprite(0,2,turnHolderPos,62);
            }
            else if (battlePhase==2) {
                turnHolderPos=192+64;
                enemyTurnHolderPos=lerp(enemyTurnHolderPos,192,0.25f);
                NF_MoveSprite(0,15,enemyTurnHolderPos,62);
            }
            NF_SpriteFrame(0,0,pressTurnCount(true,false)/2);
            switch (battleMenuState) {
                case 0: //action select
                    if (keysDown() & KEY_UP) {
                        mmEffect(SFX_SELECT);
                        battleMenuState=1;
                    }
                    NF_MoveSprite(0,16,-16,-16);
                    break;
                case 1: //fight
                    if (keysDown() & KEY_B) {
                        mmEffect(SFX_DESELECT);
                        battleMenuState=0;
                    }
                    if (keysDown() & KEY_A) {
                        cout<<battlePhase<<endl;
                        battlePhase=1;
                        playerMove(selectingPartyMember,selectedEnemy,"bash");
                    }
                    //if enemy1 isnt empty, allow it to be selected
                    //ditto, enemy2
                    NF_MoveSprite(0,16,enemy0pos[0],enemy0pos[1]);
                    break;
                default: ;
            }
            break;
        case 'o':
            break;
        default: ;
    }
}

void hudRenderSub(const string& hudType) {
}