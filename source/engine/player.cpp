//
// Created by tailofhell on 9/10/26.
//

#include "player.h"

#include <iostream>
#include <ostream>

#include "anims.h"
#include "ulib/ulib.h"
#include "game.h"
#include "characters/maple_png.h"
using namespace std;
int animTics=0;

void partyInit() { //british neighbor asking you about your house
    initAnimation("maple");
}

void partyRender() {

    ulDrawImage(party.maple);
    if (leaderXOffset<=0)
        leaderXOffset=0;
    if (leaderXOffset>30*16)
        leaderXOffset=30*16;
    if (partyMembers[0]=="maple") {
        party.maple->x=256/2;
        party.maple->y=192/2;
    }
    if (ul_keys.held.up && inWindow==false) {
        leaderYOffset--;
    }
    else if (ul_keys.held.down && inWindow==false) {
        playAnimation("mapleWalkingDown");
        leaderYOffset++;
    }
    if (ul_keys.held.left && inWindow==false) {
        leaderXOffset--;
    }
    else if (ul_keys.held.right && inWindow==false) {
        leaderXOffset++;
    }
}
