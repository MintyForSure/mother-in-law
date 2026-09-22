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
#include "battlebacks.h"
#include "ui/numbers_png.h"
#include "characters/maple_png.h"
#include "soundbank.h"

using namespace std;

static s8 tics=0;
static int damagePosXY[]={252/2,192/2};
static s8 yPos;
string mapleAction[]={"null","null","null"};
string aaronAction[]={};
static string selectingPartyMember;
int selectedEnemy=0;
int battlePhase=0; //0-select 1-start combat phase 2-player move phase 3-enemy move phase 4-ex turn
namespace {
    struct battleElements { //one magic variable wont hurt
        UL_MAP *battleBack;
        UL_IMAGE *battleBG;
        UL_IMAGE *enemy0;
        UL_IMAGE *enemy1;
        UL_IMAGE *enemy2;
        UL_IMAGE *cursor;
        UL_IMAGE *numbers;
        UL_IMAGE *maple;
    };
}

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

static struct battleElements bElem;
static mapleBattleStats maple;

void battleInit(const char *enemy0, const char *enemy1, const char *enemy2) {
    //std::string turnOrder[]={mapleStats[4]};
    //bElem.battleBack=ulLoadImageFilePNG(reinterpret_cast<const char *>(bg_png),(int)bg_png_size,UL_IN_VRAM,UL_PF_PAL4);
    bElem.cursor=ulLoadImageFilePNG((cursor_png),(int)cursor_png_size,UL_IN_VRAM,UL_PF_PAL4);
    bElem.numbers=ulLoadImageFilePNG(numbers1_png,(int)numbers1_png_size,UL_IN_VRAM,UL_PF_PAL4);
    bElem.maple=ulLoadImageFilePNG(maple_png,static_cast<int>(maple_png_size),UL_IN_VRAM,UL_PF_PAL4);
    ulSetImageTileSize(bElem.cursor,0,0,8,8);
    ulSetImageTileSize(bElem.numbers,0,0,9,9);
    ulSetImageTileSize(bElem.maple,16,0,16,32);

    string enemies[]={enemy0,enemy1,enemy2};
    //std::cout << enemies[0] << std::endl;
    maple.hp=mapleHP[1];
    maple.pp=maplePP[1];
    maple.atk=mapleStats[2];
    maple.def=mapleStats[3];
    maple.spd=mapleStats[4];
    if (enemies[0] == "rat") {
        getEnemyData("cheesyRat");
        bElem.enemy0=ulLoadImageFilePNG(cheesyRat_png,(int)cheesyRat_png_size,UL_IN_VRAM,UL_PF_PAL4);
        battlebackInit("rat");
    }
    if (enemies[1]=="rat") {
        bElem.enemy1=ulLoadImageFilePNG(cheesyRat_png,(int)cheesyRat_png_size,UL_IN_VRAM,UL_PF_PAL4);
    }
    if (enemies[2]=="rat") {
        bElem.enemy2=ulLoadImageFilePNG(cheesyRat_png,(int)cheesyRat_png_size,UL_IN_VRAM,UL_PF_PAL4);
    }
    //bElem.battleBack = ulCreateMap(battleBG,bg_map);
}

// int *getTurnOrder() {
//
//     int turnOrder[]={maple.spd};
//     return turnOrder;
// }

int damageCalc(string user,string target,string action) {
    int damage=0;
    if (action=="bash") {
        if (user=="maple") {
            cout<<(maple.atk * 1.5)<<endl;
            damage=int(maple.atk * 1.5);
        }
    }
    cout<<damage<<endl;
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
void playerMove(string actor) {
    if (actor=="maple" && mapleAction[2]=="bash") {
        drawWindow("mapleAttack",8,8,252-32,16,"normal");
        windowDisplayText("Maple attacks!","");
        int damageOutput = damageCalc(mapleAction[0], mapleAction[1],mapleAction[2]);
        //cout<<"maple gives damage: "<<damageCalc(mapleAction[0], mapleAction[1],mapleAction[2])<<endl;
        //cout<<"maple atk: "<<mapleStats[2]<<endl;
        damageRender(damageOutput);
    }
    else if (actor=="maple"&& mapleAction[2]=="PSI") {

    }
}

void battleProcess() {
    //ulDrawGradientRect(0, 0, 256, 192, RGB15(24, 0, 28), RGB15(0, 0, 0),RGB15(0, 0, 0), RGB15(0, 0, 24));
    //ulDrawImage(bElem.battleBack);
    renderBattleback("rat");
    ulDrawFillRect(0,0,256,30,RGB15(0,0,0));
    ulDrawFillRect(0,162,256,192,RGB15(0,0,0)); //layering troubles, so im rendering it here.
    ulImageSetRotCenter(bElem.enemy0);
    ulImageSetRotCenter(bElem.maple);
    ulDrawImageXY(bElem.maple,128,192-32);
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
        //ulSetImageTint(bElem.enemy0,RGB15(31,31,31));
        cout<<"enemy2 nullptr "<<(bElem.enemy2==nullptr)<<endl;
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
        case 0:
            break;
        case 1:
            // getTurnOrder();
            playerMove("maple");
            if (doWindowDrawing==false) {
                battlePhase=0;
                doWindowDrawing=true;
            }
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        default:
            break;
    }
}