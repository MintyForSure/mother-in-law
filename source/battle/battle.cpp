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
static string battleInitEnemy0;
static string battleInitEnemy1;
static string battleInitEnemy2;
string selectingPartyMember;
int battlePhase=0; //0-select 1-start combat phase 2-player move phase 3-enemy move phase 4-ex turn
int turns={};

s8 enemy0pos[2];
s8 enemy1pos[2];
s8 enemy2pos[2];

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

void battleInit(const std::string& enemy0, const std::string& enemy1, const std::string& enemy2) {
    battleInitEnemy0=enemy0;
    battleInitEnemy1=enemy1;
    battleInitEnemy2=enemy2;
    selectingPartyMember=partyMembers.at(0);
    cout<<"selectingPartyMember: "<<selectingPartyMember<<endl;
    pressTurnCount(true,true);
    cout<<"enemy1: "<<enemy1<<endl;
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
    setEnemyPositions();
}

void setEnemyPositions() {
    if (battleInitEnemy0!="empty" and battleInitEnemy1=="empty" and battleInitEnemy2=="empty") {
        enemy0pos[0]=(256/2)-32;
        enemy0pos[1]=(192/2)-32;
        cout<<enemy0pos[1]<<endl;
    }
    NF_MoveSprite(0,12,enemy0pos[0],enemy0pos[1]);
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

int damageCalc(string user,int target,string action) {
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

static int mHPTics = 0;
static int mPPTics = 0;

static int cHPTics = 0;
static int cPPTics = 0;

void updateHP(){
    if (mapleHP[0] < mapleHP[1]) {
        if (mapleDefending==true && mHPTics % 10==1) {
            mapleHP[1]--;
        }
        else if (mHPTics % 6==1) {
            mapleHP[1]--;
        }
    }
    else if (mapleHP[0]>mapleHP[1]) {
        if (mHPTics % 4==1) {
            mapleHP[1]++;
        }
    }
    else {
        mHPTics=0;
    }
    if (mHPTics==80) {
        mHPTics=0;
    }
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

void updatePP() {
    if (maplePP[0] < maplePP[1]) {
        if (mPPTics % 6==1) {
            maplePP[1]--;
        }
    }
    else if (maplePP[0]>maplePP[1]) {
        if (mPPTics % 4==1) {
            maplePP[1]++;
        }
    }
    else {
        mPPTics=0;
    }
    if (crusherPP[0] < crusherPP[1]) {
        if (cPPTics % 6==1) {
            //cout << mapleHP[1] << "\n";
            crusherPP[1]--;
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
}

void playerMove(string member,int target,string action) {
    int damageOutput = 0;
    if (action=="bash") {
        if (member=="maple") {
            damageOutput = damageCalc("maple",selectedEnemy,"bash");
        }
        else if (member=="crusher") {
            damageOutput = damageCalc("crusher",selectedEnemy,"bash");
        }
        passTurn(true,true);
    }
    cout<<damageOutput<<endl;
}

void battleProcess() {
    updateHP();
    updatePP();
    switch (battlePhase) {
        case 0:
            break;
        case 1:
            break;
        case 2: //windowwww
            break;
        default:
            cout<<"default, battlePhase"<<endl;
    }
}