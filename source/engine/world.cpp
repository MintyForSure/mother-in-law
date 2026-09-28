//
// Created by tailofhell on 9/10/26.
//

#include "world.h"

#include <iostream>
#include <ulib/ulib.h>
#include "window.h"
#include "game.h"
#include "player.h"
#include "window.h"
#include "entities.h"
#include "../data/maps/debugRoom.h"
#include "tiles/debugRoom_png.h"


static UL_MAP *map;

class worldObject{

};

class NPC {

};

void mapLoad(const std::string& mapName) {
    if (mapName=="debugRoom") {
        UL_IMAGE *mapImg=ulLoadImageFilePNG(debugRoom_png,debugRoom_png_size,UL_IN_VRAM,UL_PF_PAL4);
        map=ulCreateMap(mapImg,debugRoomMap,16,16,30,30,UL_MF_U16);
        instanceEntity(0,0,"testEnemy");
    }
}

void mapInit(const std::string& mapName) { //british people when i talk about the earth

}

void mapRender() {
    updateEntities();
    ulDrawMap(map);
    map->scrollX=leaderXOffset;
    map->scrollY=leaderYOffset;
    //std::cout<<"leaderXPos"<<leaderXOffset<<std::endl;

    //drawMenu(4,false,16,16,128,32);
}