//
// Created by tailofhell on 9/1/26.
//
#include <iostream>
#include <string>
#include <nds/arm9/video.h>
#include <nds.h>
#include <ulib/ulib.h>
#include "cursor_png.h"
static bool doDrawing;
struct windowElements {
    UL_IMAGE *cursor;
};

static struct windowElements window;

void windowSysInit() { //run once only
    window.cursor=ulLoadImageFilePNG(reinterpret_cast<const char *>(cursor_png),int(cursor_png_size),UL_IN_VRAM,UL_PF_PAL4);
    ulSetImageTileSize(window.cursor,0,0,8,8);
}
void spawnWindow(std::string text,bool top=true) {
    if (top==true) {
        ulDrawFillRect(0,0,256,32,RGB15(0,0,0));
        ulDrawString(8,8,text.c_str());
        ulDrawImageXY(window.cursor,256-16,20);
        //std::cout << text << std::endl;
        if (ul_keys.pressed.A) {
            doDrawing=false;
            //std::cout<<"doDrawing:"<<doDrawing<<std::endl;
        }
    }
    else if (top==false&&doDrawing==true) {
        ulDrawFillRect(0,0,256,144,RGB15(0,0,0));
    }
}