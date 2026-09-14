//
// Created by tailofhell on 9/10/26.
//

#include "player.h"

#include <iostream>
#include <ostream>

#include "ulib/ulib.h"
#include "game.h"
#include "characters/maple_png.h"
using namespace std;
int xPos=64;
int yPos=64;
struct partyMembers {
    UL_IMAGE *maple;
};

static struct partyMembers party;
void partyInit() { //british guy asking you about your house
    party.maple=ulLoadImageFilePNG(maple_png,int(maple_png_size),UL_IN_VRAM,UL_PF_PAL4);
    ulSetImageTileSize(party.maple,0,0,16,28);
    ulImageSetRotCenter(party.maple);
}

void partyRender() {
    party.maple->x=xPos;
    party.maple->y=yPos;
    if (partyMembers[0]=="maple") {
        ulDrawImage(party.maple);
        cout<<01<<endl;
    }
    if (ul_keys.held.up) {
        ulSetImageTileSize(party.maple,32,0,16,28);
        yPos--;
    }
    else if (ul_keys.held.down) {
        ulSetImageTileSize(party.maple,0,0,16,28);
        yPos++;
    }
    if (ul_keys.held.left) {
        ulSetImageTileSize(party.maple,16,0,16,28);
        xPos--;
    }
    else if (ul_keys.held.right) {
        ulSetImageTileSize(party.maple,48,0,16,28);
        xPos++;
    }
}