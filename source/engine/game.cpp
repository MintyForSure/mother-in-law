//
// Created by tailofhell on 8/26/26.
//
#include <stdbool.h>
#include <nf_lib.h>
#include <nds.h>
#include "game.h"
#include "../data/enemyData.h"
#include "hud.h"
#include <iostream>
#include <ctime>
#include <nds/arm9/video.h>
#include <filesystem.h>
#include <vector>

#include "../battle/battle.h"
#include "player.h"
#include "window.h"
#include "world.h"
#include "../data/items.h"

using namespace std;

vector<string> partyMembers={"maple"};
string inventory[]={"coolFood","junkFood"};
char gameState;
bool debug;
bool inWindow;
int mapleLevel[]={1,0,5}; //level,xp,xp to next level
int mapleStats[]={25,12,5,2,4}; //maxHP,maxPP,atk,def,speed
int crusherLevel[]={1,0,5}; //level,xp,xp to next level
int crusherStats[]={22,20,4,3,4}; //maxHP,maxPP,atk,def,speed
int ashtonLevel[]={1,0,5}; //level,xp,xp to next level
int ashtonStats[]={24,0,6,3,4}; //maxHP,maxPP,atk,def,speed

int partyMemberCount=std::size(partyMembers);

int mapleHP[]={mapleStats[0],mapleStats[0],mapleStats[0]}; //Target HP, Current HP, Max HP
int maplePP[]={mapleStats[1],mapleStats[1],mapleStats[1]}; //Target PP, Current PP, Max PP

int crusherHP[]={crusherStats[0],crusherStats[0],crusherStats[0]};
int crusherPP[]={crusherStats[1],crusherStats[1],crusherStats[1]};

int ashtonHP[]={ashtonStats[0],ashtonStats[0],ashtonStats[0]};
int ashtonPP[]={ashtonStats[1],ashtonStats[1],ashtonStats[1]};
bool gamePaused;
static bool initSwitch_b=false;
static bool initSwitch_m=false;
static bool initSwitch_x=false;
static int sel=0;
static int hSel=0;


// static int getPartyStats(const string& member,int stat=0) {
//     constexpr int stats[]={};
//     if (member=="maple") {
//         if (mapleLevel[0]==0) { //Initialize level
//             mapleLevel[0]=1;
//             mapleLevel[1]=0;
//             mapleLevel[2]=5; //Level 1, 0 current XP, 5 XP to next level
//         }
//     }
//     return *stats;
// }

vector<string> mapleInv={"coolFood"};

void addItem(const string& member, const string& item) {
    if (member=="maple") {
        mapleInv.push_back(item);
    }
}
void manageInv() {

}

void gameInit() {
    srand(time(0));

    //Initialize NF_Lib
    NF_Set2D(1,0);
    NF_Set2D(0,0);
    //NF_Set3D(0,0);
    NF_InitSpriteBuffers();
    NF_InitTiledBgBuffers();
    NF_InitTiledBgSys(0);
    NF_InitSpriteSys(0);
    NF_InitTextSys(0);
    NF_InitTiledBgSys(1);
    NF_InitSpriteSys(1);
    NF_InitTextSys(1);
    // Initialization of µlibrary

    // ulInit(UL_INIT_ALL);
    // ulInitDualScreenMode();
    // ulInitGfx();
    // ulInitText();
    // ulSetMainLcd(1);
    // ulSetTransparentColor(RGB15(31,0,31));
    NF_UpdateTextLayers();
}
void pauseMenu() {
    drawWindow("pause",16,16,32,128,"menu");

}
void gameLogic() { //checks where the game is :)
    if (gameState=='o') {
        if (initSwitch_m==false) {
            mapLoad("debugRoom");
            partyInit();
            initSwitch_m=true;
        }
        else if (initSwitch_m==true) {
            mapRender();
            partyRender();
        }
    }
    else if (gameState=='x') {

        if (initSwitch_x==false) {
            NF_InitTextSys(1);
            NF_LoadTextFont("fnt/default", "down", 256, 256, 0);
            NF_CreateTextLayer(0, 0, 0, "down");
            NF_Sort3dSprites();
            initSwitch_x=true;
        }
        else {
            NF_WriteText(0,0,2,2,"Mother Game State Selector");
            // ulDrawString(16,16,"Mother 2000 Game State Selector");
            //
            // ulDrawString(32,32+64,"battle");
            NF_WriteText(0,0,2,8,"battle");
            NF_WriteText(0,0,2,9,"overworld");
            // ulDrawString(32,48+64,"overworld");
            NF_WriteText(0,0,2,16,"D-Pad Up/Down to move, \n  A to select.");


            if (keysHeld() & KEY_DOWN) {sel++;}
            else if (keysHeld() & KEY_UP){sel--;}
            if (sel==0) {
                NF_WriteText(0,0,1,8,">");
                if (keysHeld() & KEY_A) {gameState='b';}
            }
            else {
                NF_WriteText(0,0,1,9,">");
                if (keysHeld() & KEY_A) {gameState='o';}
            }
            // if (hSel==0) {
            //     ulDrawString(16,32,"Party Members: Maple");
            //     partyMembers={"maple"};
            // }
            // if (hSel==1) {
            //     ulDrawString(16,32,"Party Members: Maple, Crusher");
            //     partyMembers={"maple","crusher"};
            //  }

        }
    }
    else if (gameState=='b') {
        if (initSwitch_b==false){
            battleHudInit();
            battleInit("cheesyRat");
            initSwitch_b=true;
            //cout << "initSwitch_b=true" << endl;
        }
        else {
            battleProcess();
            hudRender('b');
        }
    }
}
