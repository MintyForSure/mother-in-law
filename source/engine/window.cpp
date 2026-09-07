//
// Created by tailofhell on 8/31/26.
//

#include "window.h"

#include <string>
#include <iostream>
#include "window_png.h"
#include <ulib/ulib.h>
#include <nds.h>

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
};

static struct windowElements window;

static int windowWidth;
static int windowHeight;

void windowInit(int width,int height,int X,int Y,bool quickWindow) {
    window.window00=ulLoadImageFilePNG(reinterpret_cast<const char *>(window_png),int(window_png_size),UL_IN_VRAM,UL_PF_PAL4);
    window.window01=ulLoadImageFilePNG(reinterpret_cast<const char *>(window_png),int(window_png_size),UL_IN_VRAM,UL_PF_PAL4);
    window.window02=ulLoadImageFilePNG(reinterpret_cast<const char *>(window_png),int(window_png_size),UL_IN_VRAM,UL_PF_PAL4);

    window.window10=ulLoadImageFilePNG(reinterpret_cast<const char *>(window_png),int(window_png_size),UL_IN_VRAM,UL_PF_PAL4);
    window.window11=ulLoadImageFilePNG(reinterpret_cast<const char *>(window_png),int(window_png_size),UL_IN_VRAM,UL_PF_PAL4);
    window.window12=ulLoadImageFilePNG(reinterpret_cast<const char *>(window_png),int(window_png_size),UL_IN_VRAM,UL_PF_PAL4);

    window.window20=ulLoadImageFilePNG(reinterpret_cast<const char *>(window_png),int(window_png_size),UL_IN_VRAM,UL_PF_PAL4);
    window.window21=ulLoadImageFilePNG(reinterpret_cast<const char *>(window_png),int(window_png_size),UL_IN_VRAM,UL_PF_PAL4);
    window.window22=ulLoadImageFilePNG(reinterpret_cast<const char *>(window_png),int(window_png_size),UL_IN_VRAM,UL_PF_PAL4);
    windowWidth=width;
    windowHeight=height;
    ulSetImageTileSize(window.window00,0,0,8,8);
    ulSetImageTileSize(window.window01,8,0,8,8);
    ulSetImageTileSize(window.window02,16,0,8,8);
    ulSetImageTileSize(window.window10,0,8,8,8);
    ulSetImageTileSize(window.window11,8,8,8,8);
    ulSetImageTileSize(window.window12,16,8,8,8);
    ulSetImageTileSize(window.window20,0,16,8,8);
    ulSetImageTileSize(window.window21,8,16,8,8);
    ulSetImageTileSize(window.window22,16,16,8,8);

    //window.window01->centerX=width;
    window.window01->stretchX=width;
    window.window10->stretchY=height;
    window.window11->stretchX=width;
    window.window11->stretchY=height;
    window.window12->stretchY=height;
    window.window21->stretchX=width;
}

void drawWindow(std::string text,int X,int Y) {
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

    ulDrawString(window.window00->x+4,window.window00->y+6,text.c_str());
}
