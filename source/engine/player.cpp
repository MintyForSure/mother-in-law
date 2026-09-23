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
int animTics=0;
struct partyMembers {
    UL_IMAGE *maple;
    UL_IMAGE *ashton;

    UL_IMAGE *crusher;
};

static struct partyMembers party;
void partyInit() { //british neighbor asking you about your house
    party.maple=ulLoadImageFilePNG(maple_png,int(maple_png_size),UL_IN_VRAM,UL_PF_PAL4);
    ulSetImageTileSize(party.maple,0,0,16,32);
    ulImageSetRotCenter(party.maple);
}

void handleAnims(string partyMember) {
    if (partyMember=="maple") {
        if (animTics>29) {
            ulSetImageTileSize(party.maple,0,0,16,32);
        }
        else {
            ulSetImageTileSize(party.maple,32,0,16,32);
        }
    }
};

void partyRender() {
    party.maple->x=xPos;
    party.maple->y=yPos;
    if (xPos<0)
        xPos=0;
    if (xPos>30*16)
        xPos=30*16;
    if (animTics>59) {
        animTics=0;
    }
    if (partyMembers[0]=="maple") {
        ulDrawImage(party.maple);
    }
    if (ul_keys.held.up && inWindow==false) {
        ulSetImageTileSize(party.maple,32,0,16,32);
        animTics++;
        yPos--;
    }
    else if (ul_keys.held.down && inWindow==false) {
        handleAnims("maple");
        yPos++;
    }
    if (ul_keys.held.left && inWindow==false) {
        ulSetImageTileSize(party.maple,16,0,16,32);
        animTics++;
        xPos--;
    }
    else if (ul_keys.held.right && inWindow==false) {
        ulSetImageTileSize(party.maple,48,0,16,32);
        animTics++;
        xPos++;
    }
}
