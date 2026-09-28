//
// Created by tailofhell on 8/31/26.
//

#include "window.h"

#include <string>
#include <iostream>
#include "ui/window_png.h"
#include "ui/cursor_png.h"
#include <ulib/ulib.h>
#include <nf_lib.h>
#include <nds.h>
#include <nds/arm9/video.h>

#include "game.h"
#include "hud.h"

using namespace std;

struct windowElements {
    UL_IMAGE *window00;
    UL_IMAGE *window01;
    UL_IMAGE *window02;

    UL_IMAGE *window10;
    UL_IMAGE *window11;
    UL_IMAGE *window12;

    UL_IMAGE *window20;
    UL_IMAGE *window21;
    UL_IMAGE *window22;

    UL_IMAGE *indicator;
};

class Window {
    public:
    string windowID;
    int width{};
    int height{};
    int X{};
    int Y{};
    bool popup{};
    bool doDrawing{};
    string line1;
    string line2;
};

static struct windowElements window;
static string currentText;
static string windowMessages[]={};

static int windowWidth;
static int windowHeight;
static s8 tics=0;
int visibleCharactersLine1=0;
int visibleCharactersLine2=0;
bool doWindowDrawing=true;

// void flushMessages() {
//     visibleCharactersLine1=0;
//     visibleCharactersLine2=0;
//     windowMessages[0]={};
//     windowMessages[1]={};
//     windowMessages[2]={};
//     windowMessages[3]={};
//     windowMessages[4]={};
// }

void windowSysInit() {
    //ulib side
    window.window00=ulLoadImageFilePNG(reinterpret_cast<const char *>(window_png),int(window_png_size),UL_IN_VRAM,UL_PF_PAL4);
    window.window01=ulLoadImageFilePNG(reinterpret_cast<const char *>(window_png),int(window_png_size),UL_IN_VRAM,UL_PF_PAL4);
    window.window02=ulLoadImageFilePNG(reinterpret_cast<const char *>(window_png),int(window_png_size),UL_IN_VRAM,UL_PF_PAL4);

    window.window10=ulLoadImageFilePNG(reinterpret_cast<const char *>(window_png),int(window_png_size),UL_IN_VRAM,UL_PF_PAL4);
    window.window11=ulLoadImageFilePNG(reinterpret_cast<const char *>(window_png),int(window_png_size),UL_IN_VRAM,UL_PF_PAL4);
    window.window12=ulLoadImageFilePNG(reinterpret_cast<const char *>(window_png),int(window_png_size),UL_IN_VRAM,UL_PF_PAL4);

    window.window20=ulLoadImageFilePNG(reinterpret_cast<const char *>(window_png),int(window_png_size),UL_IN_VRAM,UL_PF_PAL4);
    window.window21=ulLoadImageFilePNG(reinterpret_cast<const char *>(window_png),int(window_png_size),UL_IN_VRAM,UL_PF_PAL4);
    window.window22=ulLoadImageFilePNG(reinterpret_cast<const char *>(window_png),int(window_png_size),UL_IN_VRAM,UL_PF_PAL4);

    window.indicator=ulLoadImageFilePNG(cursor_png,cursor_png_size,UL_IN_VRAM,UL_PF_PAL4);

    ulSetImageTileSize(window.window00,0,0,8,8);
    ulSetImageTileSize(window.window01,8,0,8,8);
    ulSetImageTileSize(window.window02,16,0,8,8);
    ulSetImageTileSize(window.window10,0,8,8,8);
    ulSetImageTileSize(window.window11,8,8,8,8);
    ulSetImageTileSize(window.window12,16,8,8,8);
    ulSetImageTileSize(window.window20,0,16,8,8);
    ulSetImageTileSize(window.window21,8,16,8,8);
    ulSetImageTileSize(window.window22,16,16,8,8);
    ulSetImageTileSize(window.indicator,0,0,8,8);

    //NFLib side
    NF_LoadSpriteGfx("gfx/window",0,8,8);
    NF_LoadSpritePal("gfx/window",0);

    NF_VramSpriteGfx(1,0,0,false);
    NF_VramSpritePal(1,0,0);


    for (int i=0;i<9;i++) {
        NF_CreateSprite(1,i,0,0,-32,0);
    }
    NF_SpriteFrame(1,0,0);
    NF_SpriteFrame(1,1,1);
    NF_SpriteFrame(1,2,2);
    NF_SpriteFrame(1,3,4);
    NF_SpriteFrame(1,4,5);
    NF_SpriteFrame(1,5,6);
    NF_SpriteFrame(1,6,8);
    NF_SpriteFrame(1,7,9);
    NF_SpriteFrame(1,8,10);

}

void windowInstance(int X, int Y, int width, int height, bool popup=false) {

}

void windowDisplayText(const string& line1,const string& line2) {
    // for (int i=0;i<line1.length();i++) {
    //     visibleCharactersLine1++;
    //     ulDrawString(window.window00->x+4,window.window00->y+6,&line1[line1.length()-i]);
    //     cout<<line1[i]<<endl;
    //}
    if (doWindowDrawing==true) {
        ulSetTextColor(RGB15(31,31,31));
        ulDrawString(window.window00->x+4,window.window00->y+6,line1.c_str());
        ulDrawString(window.window00->x+4,window.window01->y+16,line2.c_str());
    }
}

void drawWindow(string windowID,int X,int Y,int width,int height,string type) {
    windowWidth=width;
    windowHeight=height;
    window.window01->stretchX=width;
    window.window10->stretchY=height;
    window.window11->stretchX=width;
    window.window11->stretchY=height;
    window.window12->stretchY=height;
    window.window21->stretchX=width;
    if (doWindowDrawing==true) {
        //std::cout<<window.window01->stretchX<<std::endl;
        ulDrawImageXY(window.window00,X,Y);
        ulDrawImageXY(window.window01,window.window00->x+8,Y);
        ulDrawImageXY(window.window02,window.window01->stretchX+window.window01->x,window.window01->y);

        ulDrawImageXY(window.window10,X,Y+8);
        ulDrawImageXY(window.window11,window.window01->x,Y+8);
        ulDrawImageXY(window.window12,window.window11->stretchX+window.window11->x,Y+8);

        ulDrawImageXY(window.window20,X,window.window10->stretchY+Y+8);
        ulDrawImageXY(window.window21,window.window20->x+8,window.window10->stretchY+Y+8);
        ulDrawImageXY(window.window22,window.window21->stretchX+window.window21->x,window.window10->stretchY+Y+8);
        if (type=="normal") {
            inWindow=true;
            ulDrawImageXY(window.indicator,window.window22->x-4,window.window22->y-4);
            if (ul_keys.pressed.A && gameState=='b') {
                doWindowDrawing=false;
                inWindow=false;
            }
            tics++;
            if (tics>29) {
                ulSetImageTileSize(window.indicator,0,0,8,8);
            }
            else {
                ulSetImageTileSize(window.indicator,8,0,8,8);
            }
            if (tics==60) {
                tics=0; //reset
            }
        }
        else if (type=="menu") {

        }
    }
}

void drawMenu(int entries,bool allowReturn, int X,int Y,int width,int height) {
    windowWidth=width;
    windowHeight=height;
    window.window01->stretchX=width;
    window.window10->stretchY=height;
    window.window11->stretchX=width;
    window.window11->stretchY=height;
    window.window12->stretchY=height;
    window.window21->stretchX=width;
    if (doWindowDrawing==true) {
        //std::cout<<window.window01->stretchX<<std::endl;
        ulDrawImageXY(window.window00,X,Y);
        ulDrawImageXY(window.window01,window.window00->x+8,Y);
        ulDrawImageXY(window.window02,window.window01->stretchX+window.window01->x,window.window01->y);

        ulDrawImageXY(window.window10,X,Y+8);
        ulDrawImageXY(window.window11,window.window01->x,Y+8);
        ulDrawImageXY(window.window12,window.window11->stretchX+window.window11->x,Y+8);

        ulDrawImageXY(window.window20,X,window.window10->stretchY+Y+8);
        ulDrawImageXY(window.window21,window.window20->x+8,window.window10->stretchY+Y+8);
        ulDrawImageXY(window.window22,window.window21->stretchX+window.window21->x,window.window10->stretchY+Y+8);
    }
    for (int i=0;i<entries;i++) {
        if (entries) {
            ulDrawString(window.window00->x+6,window.window00->y+6,mapleInv[0].c_str());
            ulDrawString(window.window00->x+6,window.window00->y+16,"entry");
        }
    }
}

void NFdrawWindow(int X,int Y,int width,int height,string type) {
    windowWidth=width;
    windowHeight=height;

    NF_MoveSprite(1,0,X,Y);
    NF_MoveSprite(1,1,X+8,Y);
    NF_MoveSprite(1,2,X+16,Y);

    NF_MoveSprite(1,3,X,Y+8);
    NF_MoveSprite(1,4,X+8,Y+8);
    NF_MoveSprite(1,5,X+16,Y+8);

    NF_MoveSprite(1,6,X,Y+16);
    NF_MoveSprite(1,7,X+8,Y+16);
    NF_MoveSprite(1,8,X+16,Y+16);

}