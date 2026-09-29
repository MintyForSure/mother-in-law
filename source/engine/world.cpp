//
// Created by tailofhell on 9/10/26.
//

#include "world.h"
#include <ulib/ulib.h>
#include "window.h"
#include "game.h"
#include "window.h"
#include "../data/maps/debugRoom.h"
#include "tiles/debugRoom_png.h"


static UL_MAP *map;

void mapLoad(const std::string& mapName) {
    if (mapName=="debugRoom") {
        UL_IMAGE *mapImg=ulLoadImageFilePNG(reinterpret_cast<const char *>(debugRoom_png),debugRoom_png_size,UL_IN_VRAM,UL_PF_PAL4);
        map=ulCreateMap(mapImg,debugRoomMap,16,16,30,30,UL_MF_U16);
    }
}

void mapInit(const std::string& mapName) { //british people when i talk about the earth

}

void mapRender() {
    ulDrawMap(map);
    //drawMenu(4,false,16,16,128,32);
}