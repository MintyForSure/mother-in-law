//
// Created by tailofhell on 8/26/26.
//
#include <stdbool.h>
#include <ulib/ulib.h>
#include <nf_lib.h>
#include "game.h"
#include "../data/enemyData.h"
#include "hud.h"
#include <iostream>
#include <ctime>
#include <nds/arm9/video.h>
#include <filesystem.h>
#include <vector>

#include "battle.h"
#include "player.h"
#include "window.h"
#include "world.h"
#include "../data/items.h"

using namespace std;

string partyMembers[]={"maple"};
string inventory[]={"coolFood","junkFood"};
char gameState;
bool debug;
bool inWindow;
int mapleLevel[]={1,0,5}; //level,xp,xp to next level
int mapleStats[]={25,12,5,2,4}; //maxHP,maxPP,atk,def,speed
int mapleHP[]={mapleStats[0],mapleStats[0],mapleStats[0]}; //Target HP, Current HP, Max HP
int maplePP[]={mapleStats[1],mapleStats[1],mapleStats[1]}; //Target PP, Current PP, Max PP

static bool initSwitch_b=false;
static bool initSwitch_m=false;
static bool initSwitch_x=false;
static int sel=0;


static int getPartyStats(const string& member,int stat=0) {
    constexpr int stats[]={};
    if (member=="maple") {
        if (mapleLevel[0]==0) { //Initialize level
            mapleLevel[0]=1;
            mapleLevel[1]=0;
            mapleLevel[2]=5; //Level 1, 0 current XP, 5 XP to next level
        }
    }
    return *stats;
}

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
    NF_SetRootFolder("NITROFS");
    nitroFSInit(NULL);
    NF_InitSpriteBuffers();
    NF_InitTiledBgBuffers();
    NF_InitTiledBgSys(1);
    NF_InitSpriteSys(1);
    // Initialization of µlibrary

    ulInit(UL_INIT_ALL);
    ulInitGfx();
    ulInitText();
    ulSetMainLcd(1);
    ulSetTransparentColor(RGB15(31,0,31));
}
void pauseMenu() {
    drawMenu(4,true,16,16,64,128);
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
            //NF_LoadTextFont16("fnt/font16", "down", 256, 256, 0);
            //NF_CreateTextLayer16(1, 0, 0, "down");
            initSwitch_x=true;
        }
        else {
            ulDrawString(16,16,"Mother Delta Game State Selector");

            ulDrawString(32,32+64,"overworld");
            ulDrawString(32,48+64,"battle");
            ulDrawString(16,192-16,"D-Pad Up/Down to move, A to select.");

            if (ul_keys.pressed.down) {sel++;}
            else if (ul_keys.pressed.up){sel--;}

            if (sel==0) {
                ulDrawString(16,96,">");
                if (ul_keys.pressed.A) {gameState='o';}
            }
            else {
                ulDrawString(16,48+64,">");
                if (ul_keys.pressed.A) {gameState='b';}
            }

            NF_UpdateTextLayers();
        }
    }
    else if (gameState=='b') {
        if (initSwitch_b==false){
            battleHudInit();
            battleInit("rat");
            initSwitch_b=true;
            //cout << "initSwitch_b=true" << endl;
        }
        else {
            battleProcess();
            hudRender('b');
            if (ul_keys.pressed.X) {
                cout << "Setting Maple's HP to 12." << endl;
                mapleHP[0]=12;
            }
            else if (ul_keys.pressed.Y) {
                cout << "Setting Maple's HP to max" << endl;
                cout << mapleHP[2] << endl;
                mapleHP[0]=mapleHP[2];
            }
        }
    }
    if (ul_keys.held.L && ul_keys.held.R) {
        debug=true;
        consoleDemoInit();
        cout << "Maple level:"<< mapleLevel[0] << endl;
        cout << "Maple HP: " << mapleHP[0] << endl;
        //printf("debug enabled, probably");
    }
    if (ul_keys.pressed.L) {
        mapleHP[2]=125;
    }
}
