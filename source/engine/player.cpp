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
int animTics=0;
struct partyMembers {
    UL_IMAGE *maple;
    UL_IMAGE *ashton;

    UL_IMAGE *crusher;
};

static struct partyMembers party;
void partyInit() { //british neighbor asking you about your house
    party.maple=ulLoadImageFilePNG(reinterpret_cast<const char *>(maple_png),int(maple_png_size),UL_IN_VRAM,UL_PF_PAL4);
    ulSetImageTileSize(party.maple,0,0,16,32);
    ulImageSetRotCenter(party.maple);
    party.maple->x=256/2;
    party.maple->y=192/2;
}

void handleAnims(const string& partyMember) {
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
    if (leaderXOffset<=0)
        leaderXOffset=0;
    if (leaderXOffset>30*16)
        leaderXOffset=30*16;
    if (animTics>59) {
        animTics=0;
    }
    if (partyMembers[0]=="maple") {
        ulDrawImage(party.maple);
    }
    if (ul_keys.held.up && inWindow==false) {
        leaderYOffset--;
    }
    else if (ul_keys.held.down && inWindow==false) {
        leaderYOffset++;
    }
    if (ul_keys.held.left && inWindow==false) {
        leaderXOffset--;
    }
    else if (ul_keys.held.right && inWindow==false) {
        leaderXOffset++;
    }
}
