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
int battleMenuState=0; //2: starting window 0:menupick 1:fightpick
s16 hudPos=8;
static s16 selectionOffsetX=76;
static s16 selectionOffsetY;
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


    //Create the sprites :]

    switch (partyMemberCount) { //shut up clangd
        case 1:
            NF_CreateSprite(0,1,1,1,(256/2)-32,192-64);
        default:
            cout<<"default"<<endl;
            NF_CreateSprite(0,1,1,1,(256/2)-32,192-64);
    }
    NF_CreateSprite(0,2,2,2,192,62);
    for (int x = 0; x < 256; x += 64) {
        const int id = 3 + x / 64;
        NF_CreateSprite(0, id, 3, 3, x, 0);       // top bar
        NF_CreateSprite(0, id + 4, 3, 3, x, 160); // bottom bar

        NF_SpriteLayer(0,id,1);
        NF_SpriteLayer(0,id+4,1);
        cout<<"id="<<id+4<<endl;
    }
    //NF_SpriteLayer(0,11,0);
    NF_CreateSprite(0,11,4,4,40+selectionOffsetX,32-8); //bash
    NF_CreateSprite(0,12,5,5,16+selectionOffsetX,40);    //
    NF_CreateSprite(0,13,6,6,36+selectionOffsetX,40+8);
    NF_CreateSprite(0,14,7,7,64+selectionOffsetX,39);
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
    NF_MoveSprite(0,11,40+selectionOffsetX,selectionOffsetY+(32-8));
    NF_MoveSprite(0,12,16+selectionOffsetX,selectionOffsetY+40);
    NF_MoveSprite(0,13,36+selectionOffsetX,selectionOffsetY+40+8);
    NF_MoveSprite(0,14,64+selectionOffsetX,selectionOffsetY+39);
    if (battleMenuState==0) {
        selectionOffsetY=lerp(selectionOffsetY,0,0.25f);
    }
    else {
        selectionOffsetY=lerp(selectionOffsetY,-70,0.25f);
    }
    switch (hudType) {
        case 'b':
            if (battleMenuState==0) {
                if (keysHeld() & KEY_UP) {
                    battleMenuState=1;
                }
            }
            else if (battleMenuState==1) {
                if (keysHeld() & KEY_B) {
                    battleMenuState=0;
                }
            }
        case 'o':

        default: ;
    }
}

void hudRenderSub(const string& hudType) {
    if (hudType=="battle") {
        NF_MoveSprite(0,9,0,16-8);
        NF_MoveSprite(0,10,0,64-8);
        NF_MoveSprite(0,11,0,112-8);
        NF_MoveSprite(0,12,0,160-8);

        NF_SpriteFrame(0,9,1);
        NF_SpriteFrame(0,10,1);
        NF_SpriteFrame(0,11,1);
        NF_SpriteFrame(0,12,1);

        NF_WriteText16(1,0,2,2,"Fight");
        switch (vSelected) {
            case 0:
                NF_SpriteFrame(0,9,0);
                NF_MoveSprite(0,9,64,16-8);
                break;
            case 1:
                NF_SpriteFrame(0,10,0);
                NF_MoveSprite(0,10,64,64-8);
                break;
        }
    }
}