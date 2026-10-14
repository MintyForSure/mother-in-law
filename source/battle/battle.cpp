//
// Created by tailofhell on 8/28/26.
//
#include "battle.h"
#include <iostream>
#include <nds/arm9/video.h>
#include "../data/enemyData.h"
#include "../engine/game.h"
#include "../engine/hud.h"
#include "../engine/window.h"
#include <maxmod9.h>
#include <thread>
#include <nf_lib.h>

#include "../engine/battlebacks.h"
#include "nitrofs/soundbank_info.h"

using namespace std;

static s8 tics=0;
bool allySfxPlayed=false;
bool enemySfxPlayed=false;
string mapleAction[3]={};
string aaronAction[3]={};
string crusherAction[3]={};
bool mapleDefending;
bool aaronDefending;
bool crusherDefending;
string selectingPartyMember;
int selectedEnemy=0;
int battlePhase=0; //0-select 1-start combat phase 2-player move phase 3-enemy move phase 4-ex turn
int turns={};

vector<string> enemyList={};
namespace {
    struct battleElements { //one magic variable wont hurt

    };
}

struct partyMemberActions {
    string *maple;
    string *crusher;
    string *ashton;
};
//static struct partyMemberActions actions;

namespace {
    class mapleBattleStats {
    public:
        //moves
        string action;
        string target;
        int hp{};
        int pp{};
        int atk{};
        int def{};
        int spd{};
    };
}

namespace {
    class enemyBattleStats {
    public:
        string action;
        string target;
        string name;
        int hp{};
        int pp{};
        int atk{};
        int def{};
        int spd{};
    };
}

static struct battleElements bElem;
static mapleBattleStats maple;
static enemyBattleStats enemy0stats;
static enemyBattleStats enemy1stats;
static enemyBattleStats enemy2stats;

void battleInit(std::string enemy0, const std::string& enemy1, const std::string& enemy2) {
    selectingPartyMember=partyMembers.at(0);
    cout<<"selectingPartyMember: "<<selectingPartyMember<<endl;
    pressTurnCount(true,true);
    if (enemy0=="cheesyRat") {
        battlebackInit("rat");
        NF_LoadSpriteGfx("gfx/characters/cheesyRat",12,64,64);
        NF_LoadSpritePal("gfx/characters/cheesyRat",12);
        NF_VramSpriteGfx(0,12,12,false);
        NF_VramSpritePal(0,12,12);

        NF_CreateSprite(0,33,12,12,(256/2)-32,(192/2)-32);
        cout<<"enemy0: rat"<<endl;
    }
    //std::string turnOrder[]={mapleStats[4]};
    //bElem.battleBack=ulLoadImageFilePNG(reinterpret_cast<const char *>(bg_png),(int)bg_png_size,UL_IN_VRAM,UL_PF_PAL4);
    // bElem.cursor=ulLoadImageFilePNG(reinterpret_cast<const char *>(cursor_png),(int)cursor_png_size,UL_IN_VRAM,UL_PF_PAL4);
    // bElem.numbers=ulLoadImageFilePNG(reinterpret_cast<const char *>(numbers1_png),(int)numbers1_png_size,UL_IN_VRAM,UL_PF_PAL4);

    // ulSetImageTileSize(bElem.cursor,0,0,8,8);
    // ulSetImageTileSize(bElem.numbers,0,0,9,9);

    enemyList={enemy0};
    if (enemy1!="empty") {
        enemyList.emplace_back(enemy1);
    }
    if (enemy2!="empty") {
        enemyList.emplace_back(enemy2);
    }

    //int enemy0stats[]={getEnemyData("cheesyRat")};
    //std::cout << enemies[0] << std::endl;
    maple.hp=mapleHP[1];
    maple.pp=maplePP[1];
    maple.atk=mapleStats[2];
    maple.def=mapleStats[3];
    maple.spd=mapleStats[4];
    if (enemyList[0] == "cheesyRat") {
        enemy0stats.hp=getEnemyStats("cheesyRat")[0];
        enemy0stats.name=getEnemyName("cheesyRat");
        //bElem.enemy0=ulLoadImageFilePNG(reinterpret_cast<const char *>(cheesyRat_png),(int)cheesyRat_png_size,UL_IN_VRAM,UL_PF_PAL4);
        battlebackInit("rat");
    }
    //bElem.battleBack = ulCreateMap(battleBG,bg_map);
}
int playerTakeDamage(const string& target) {
    int damage=0;
    if (target=="maple") {
        hudShake("maple",0);
        mapleHP[0]-=5;
    }

    return damage;
}

void passTurn(bool player, bool fullTurn) {
    if (player==true) {
        if (fullTurn==true){
            turns-=2;
        }
        else {
            turns--;
        }
        if (partyMembers.size()!=1) {
            if (partyMembers.at(0)=="maple") {
                selectingPartyMember=partyMembers.at(1);
                cout<<selectingPartyMember<<endl;
            }
        } //check to see if its at the end of the party members vector
        if (turns<=0){
            cout<<"turns: "<<turns<<endl;
            pressTurnCount(false,true);
            battlePhase=2;
        }
        else {
            battlePhase=0;
        }
    }
    else if (player==false) {
        if (fullTurn==true) {
            turns-=2;
        }
        else {
            turns--;
        }
        if (turns<=0) {
            cout<<"enemy turns: "<<turns<<endl;
            pressTurnCount(true,true);
            battleMenuState=2;
            battlePhase=0;
        }
    }
}

int pressTurnCount(bool player, bool refresh) {
    if (player==true) {
        if (refresh==true){
            for (auto i:partyMembers) {
                turns+=2;
            }
        }
        return turns;
    }
    else {
        if (refresh==true) {
            for (auto i=0;i<enemyList.size();i++) {
                turns+=2;
            }
        }
        return turns;
    }
}

int damageCalc(string user,string target,string action) {
    int damage=0;
    if (action=="bash") {
        if (user=="maple") {
            damage=static_cast<int>(maple.atk * 1.5);
        }
    }
    return damage;
}

void damageRender(int dmg) {

}
void renderPlayerMove(string member) {
    if (allySfxPlayed==false) {
        mmEffect(SFX_ALLYATTACK);
        allySfxPlayed=true;
    }
    if (selectingPartyMember=="maple") {
        int damageOutput = damageCalc(mapleAction[0], mapleAction[1],mapleAction[2]);
        drawWindow("mapleAttack",8,8,252-32,16,"normal");
        windowDisplayText("Maple attacks!","");
        if (doWindowDrawing==true) {
            damageRender(damageOutput);
        }
    }
    else if (selectingPartyMember=="crusher") {
        int damageOutput = damageCalc(crusherAction[0], crusherAction[1],crusherAction[2]);
        drawWindow("crusherAttack",8,8,252-32,16,"normal");
        windowDisplayText("Crusher attacks!","");
        if (doWindowDrawing==true) {
            damageRender(damageOutput);
        }
    }
}
void playerMove(string member) {
    if (member=="maple") {
        if (mapleAction[2]=="bash") {
            const int damageOutput = damageCalc(mapleAction[0], mapleAction[1],mapleAction[2]);
            //cout<<"maple gives damage: "<<damageCalc(mapleAction[0], mapleAction[1],mapleAction[2])<<endl;
            cout<<"maple atk: "<<mapleStats[2]<<endl;
            //playerTurns--;
            enemy0stats.hp=enemy0stats.hp-damageOutput;
            passTurn();
        }
        if (mapleAction[2]=="defend") {
            passTurn(true,false);
            cout<<"Maple defending."<<endl;
            mapleDefending=true;
        }
    }
    if (member=="crusher") {
        if (crusherAction[2]=="bash") {
            const int damageOutput = damageCalc(crusherAction[0], crusherAction[1],crusherAction[2]);
            cout<<"crusher atk: "<<mapleStats[2]<<endl;
            enemy0stats.hp=enemy0stats.hp-damageOutput;
        }
        if (crusherAction[2]=="defend") {
            passTurn(true,false);
            crusherDefending=true;
        }
    }
}

void battleProcess() {
    switch (battlePhase) {
        case 0:
            break;
    }
}