//
// Created by tailofhell on 8/28/26.
//
#include "battle.h"
#include <iostream>
#include <nds/arm9/video.h>
#include <ulib/ulib.h>
#include "../data/enemyData.h"
#include "ui/cursor_png.h"
#include "game.h"
#include "cheesyRat_png.h"
#include "ui/numbers1_png.h"
#include "hud.h"
#include "window.h"
#include <maxmod9.h>
#include <thread>

#include "battlebacks.h"
#include "ui/pressTurnIcons_png.h"
#include "soundbank.h"

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
        UL_MAP *battleBack;
        UL_IMAGE *battleBG;
        UL_IMAGE *enemy0;
        UL_IMAGE *enemy1;
        UL_IMAGE *enemy2;
        UL_IMAGE *cursor;
        UL_IMAGE *numbers;
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
    class enemy0BattleStats {
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

namespace {
    class enemy1BattleStats {
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

namespace {
    class enemy2BattleStats {
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
static enemy0BattleStats enemy0stats;
static enemy1BattleStats enemy1stats;
static enemy2BattleStats enemy2stats;

void battleInit(std::string enemy0, const std::string& enemy1, const std::string& enemy2) {
    selectingPartyMember=partyMembers.at(0);
    consoleDebugInit(DebugDevice_NOCASH);
    pressTurnCount(true,true);
    //std::string turnOrder[]={mapleStats[4]};
    //bElem.battleBack=ulLoadImageFilePNG(reinterpret_cast<const char *>(bg_png),(int)bg_png_size,UL_IN_VRAM,UL_PF_PAL4);
    bElem.cursor=ulLoadImageFilePNG(reinterpret_cast<const char *>(cursor_png),(int)cursor_png_size,UL_IN_VRAM,UL_PF_PAL4);
    bElem.numbers=ulLoadImageFilePNG(reinterpret_cast<const char *>(numbers1_png),(int)numbers1_png_size,UL_IN_VRAM,UL_PF_PAL4);

    ulSetImageTileSize(bElem.cursor,0,0,8,8);
    ulSetImageTileSize(bElem.numbers,0,0,9,9);

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
        bElem.enemy0=ulLoadImageFilePNG(reinterpret_cast<const char *>(cheesyRat_png),(int)cheesyRat_png_size,UL_IN_VRAM,UL_PF_PAL4);
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

void makeHalfTurn(int turn) {
    switch (turn) {
        case 0:

            break;
    }
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
    ulSetImageTileSize(bElem.numbers,0,((dmg/100)%10)*9,9,9);
    ulDrawImageXY(bElem.numbers,bElem.cursor->x,bElem.cursor->y);
    ulSetImageTileSize(bElem.numbers,0,((dmg/10)%10)*9,9,9);
    ulDrawImageXY(bElem.numbers,bElem.cursor->x+8,bElem.cursor->y);
    ulSetImageTileSize(bElem.numbers,0,((dmg/1)%10)*9,9,9);
    ulDrawImageXY(bElem.numbers,bElem.cursor->x+16,bElem.cursor->y);
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
    //cout<<"enemyList[1]: "<<enemyList[1]<<endl; dont try to observe the inside of the vector i guess.
    //ulDrawGradientRect(0, 0, 256, 192, RGB15(24, 0, 28), RGB15(0, 0, 0),RGB15(0, 0, 0), RGB15(0, 0, 24));
    //ulDrawImage(bElem.battleBack);
    renderBattleback("rat");
    // ulDrawString(8,64,(string("selectingPartyMember: ")+selectingPartyMember).c_str());
    ulDrawFillRect(0,0,256,30,RGB15(0,0,0));
    ulDrawFillRect(0,162,256,192,RGB15(0,0,0)); //layering troubles, so im rendering it here.
    ulImageSetRotCenter(bElem.enemy0);

    ulDrawImageXY(bElem.enemy0,256/2,192/2);
    //ulDrawImageXY(bElem.enemy1,128,64);
    //ulDrawImageXY(bElem.enemy2,64,192/2); //fuck it bro its your life


    if (battleMenuState==1) {
        tics++;
        if (tics>29) {
            ulSetImageTileSize(bElem.cursor,0,0,8,8);
        }
        else {
            ulSetImageTileSize(bElem.cursor,8,0,8,8);
        }
        if (tics==60) {
            tics=0; //reset
        }
        if (ul_keys.pressed.left) {
            if (bElem.enemy1==nullptr && bElem.enemy2==nullptr) {

            }
            else if (selectedEnemy!=0) {
                selectedEnemy--;
                mmEffect(SFX_HSELECT);
            }
            else if (selectedEnemy==0) {
                if (bElem.enemy2!=nullptr) {
                    selectedEnemy=2;
                    mmEffect(SFX_HSELECT);
                }
                else {
                    selectedEnemy=1;
                    mmEffect(SFX_HSELECT);
                }
            }

        }
        else if (ul_keys.pressed.right) {
            if (bElem.enemy1==nullptr && bElem.enemy2==nullptr) {

            }
            else if (selectedEnemy!=2) {
                selectedEnemy++;
                mmEffect(SFX_HSELECT);
            }
            else if (selectedEnemy==1) {
                if (bElem.enemy2==nullptr) {
                    selectedEnemy=0;
                    mmEffect(SFX_HSELECT);
                }
                else {
                    selectedEnemy=2;
                    mmEffect(SFX_HSELECT);
                }
            }
            else {
                selectedEnemy=0;
                mmEffect(SFX_HSELECT);
            }
        }

        if (selectedEnemy==0) {
            ulDrawImageXY(bElem.cursor,bElem.enemy0->x-24,bElem.enemy0->y-16);
        }
        else if (selectedEnemy==1) {
            ulDrawImageXY(bElem.cursor,bElem.enemy1->x,bElem.enemy1->y);
        }
        else if (selectedEnemy==2) {
            ulDrawImageXY(bElem.cursor,bElem.enemy2->x,bElem.enemy2->y);
        }
        else if (ul_keys.pressed.B) {
            ulSetImageTint(bElem.enemy0,RGB15(31,31,31));
            if (bElem.enemy1!=nullptr) {
                ulSetImageTint(bElem.enemy1,RGB15(31,31,31));
            }
            if (bElem.enemy2!=nullptr) {
                ulSetImageTint(bElem.enemy2,RGB15(31,31,31));
            }
        }
    }
    switch (battlePhase) {
        case 0: //player select action
            //pressTurnCount(true,true);
            break;
        case 1: //player action execution
            if (selectingPartyMember=="maple") {
                renderPlayerMove("maple");
            }
            else if (selectingPartyMember=="ashton") {
                renderPlayerMove("ashton");
            }
            else if (selectingPartyMember=="crusher") {
                renderPlayerMove("crusher");
            }
            if (ul_keys.pressed.A) {
                allySfxPlayed=false;
                cout<<"A pressed, passing turn"<<endl;
                passTurn(true,true);
                cout<<"oh god where am i"<<endl;
            }
            break;
        case 2: //enemy execution/action
            if (inWindow==false) {doWindowDrawing=true;}
            drawWindow("swag",8,8,252-32,16,"normal");
            windowDisplayText(enemy0stats.name+" attacks!");
            if (enemySfxPlayed==false) {
                mmEffect(SFX_ENEMYATTACK);
                enemySfxPlayed=true;
            }
            //mmEffect(SFX_ENEMYATTACK);
            if (ul_keys.pressed.A) {
                playerTakeDamage("maple");
                enemySfxPlayed=false;
                passTurn(false,true);
            }
            break;
        case 3:
            break;
        case 4:
            break;
        default:
            break;
    }
}