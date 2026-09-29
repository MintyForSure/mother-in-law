//
// Created by tailofhell on 9/29/26.
//

#include "anims.h"

#include <iostream>
#include <string>
#include <vector>

#include "characters/maple_png.h"
int tics = 0;
using namespace std;

vector<string> initAnimation(const std::string& group) {
    vector<string> animationGroup;
    if (group=="maple") {
        party.maple=ulLoadImageFilePNG(reinterpret_cast<const char *>(maple_png),int(maple_png_size),UL_IN_VRAM,UL_PF_PAL4);
        ulSetImageTileSize(party.maple,0,0,16,32);
        ulImageSetRotCenter(party.maple);
        animationMaker mapleWalkingUp;
        animationMaker mapleWalkingDown;
        animationMaker mapleWalkingLeft;
        animationMaker mapleWalkingRight;
        mapleWalkingUp.animConstructor(16,32,3,30);
        mapleWalkingDown.animConstructor(16,32,3,30);
        mapleWalkingLeft.animConstructor(16,32,3,30);
        mapleWalkingRight.animConstructor(16,32,3,30);
    }
    cout<<"animationInitialized"<<endl;
    return animationGroup;
}

void playAnimation(std::string name) {
    if (tics<59) {
        tics++;
    }
    else {
        tics=0;
    }
    if (name=="mapleWalkingDown") {
        ulDrawImage(party.maple);
        if (tics==0) {
            ulSetImageTileSize(party.maple,0,0,16,32);
        }
        else if (tics==14) {
            ulSetImageTileSize(party.maple,16,0,16,32);
        }
        else if (tics==29){
            ulSetImageTileSize(party.maple,32,0,16,32);
        }
        else if (tics==29+15) {
            ulSetImageTileSize(party.maple,16,0,16,32);
        }
        cout<<tics<<endl;
    }
}
