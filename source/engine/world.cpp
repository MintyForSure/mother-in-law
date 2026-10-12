//
// Created by tailofhell on 9/10/26.
//

#include "world.h"

#include <iostream>
#include "window.h"
#include "game.h"
#include "player.h"
#include "window.h"
#include "entities.h"
#include "../data/maps/debugRoom.h"


//static UL_MAP *map;

class worldObject{

};

class NPC {

};

void mapLoad(const std::string& mapName) {
    if (mapName=="debugRoom") {
    }
}

void mapInit(const std::string& mapName) { //british people when i talk about the earth

}

void mapRender() {
    updateEntities();
    // ulDrawMap(map);
    // map->scrollX=leaderXOffset;
    // map->scrollY=leaderYOffset;
    //std::cout<<"leaderXPos"<<leaderXOffset<<std::endl;

    //drawMenu(4,false,16,16,128,32);
}