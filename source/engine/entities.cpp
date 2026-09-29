//
// Created by tailofhell on 9/26/26.
//

#include "player.h"
#include "entities.h"

#include <iostream>
#include <ulib/ulib.h>
#include <string>
#include "characters/enemy_png.h"

using namespace std;

struct overworldEntity {
    UL_IMAGE *image;
    const char *name;
    int *entityXPos;
    int *entityYPos;
};

static struct overworldEntity entity;
int instanceEntity(int X, int Y, std::string name) {
    if (name=="testEnemy") {
        entity.name="testEnemy";
        entity.image=ulLoadImageFilePNG(reinterpret_cast<const char *>(enemy_png),int(enemy_png_size),UL_IN_VRAM,UL_PF_PAL4);
        entity.entityXPos=&X;
        entity.entityYPos=&Y;
        cout<<entity.name<<endl;
    }
    return 0;
}

void updateEntities() {
    ulDrawImage(entity.image);
    cout<<"entity.image->x: "<<entity.image->x<<endl;
    entity.image->x=leaderXOffset+8;
    entity.image->y=leaderYOffset+8;
}
