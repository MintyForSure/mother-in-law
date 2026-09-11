//
// Created by tailofhell on 8/26/26.
//
#include <stdbool.h>
#include <ulib/ulib.h>
#include "game.h"
#include "../data/enemyData.h"
#include "hud.h"
#include <iostream>
#include <ctime>
#include <nds/arm9/video.h>

#include "windowAlt.h"

#include "battle.h"
#include "world.h"

using namespace std;

string partyMembers[]={"maple","ashton"};
char gameState;
bool debug;
bool inWindow;
int mapleLevel[]={1,0,5}; //level,xp,xp to next level
int mapleStats[]={25,12,5,2,4}; //maxHP,maxPP,atk,def,speed
int mapleHP[]={mapleStats[0],mapleStats[0],mapleStats[0]}; //Target HP, Current HP, Max HP
int maplePP[]={mapleStats[1],mapleStats[1],mapleStats[1]}; //Target PP, Current PP, Max PP


bool initSwitch_b=false;
bool initSwitch_m=false;

static int getPartyStats(const string& member,int stat=0) {
    int stats[]={};
    if (member=="maple") {
        if (mapleLevel[0]==0) { //Initialize level
            mapleLevel[0]=1;
            mapleLevel[1]=0;
            mapleLevel[2]=5; //Level 1, 0 current XP, 5 XP to next level
        }
    }
    return *stats;
}

void gameInit() {
    srand(time(0));
    //getPartyStats("maple");
    // Initialization of µlibrary
    ulInit(UL_INIT_ALL);
    ulInitGfx();
    ulInitText();
    ulSetMainLcd(1);
    ulSetTransparentColor(RGB15(31,0,31));
}

void gameLogic() { //checks where the game is :)
    if (gameState=='o') {
        if (initSwitch_m==false) {
            mapLoad("debugRoom");
            initSwitch_m=true;
        }
        else if (initSwitch_m) {
            mapRender();
        }
    }
    else if (gameState=='b') {
        if (initSwitch_b==false){
            battleHudInit();
            battleInit("rat");
            initSwitch_b=true;
            cout << "initSwitch_b=true" << endl;
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
