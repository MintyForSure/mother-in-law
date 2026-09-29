//
// Created by tailofhell on 9/29/26.
//

#ifndef MDELTA_ANIMS_H
#define MDELTA_ANIMS_H
#include <string>
#include <vector>
#include <ulib/ulib.h>

struct partyMembers {
    UL_IMAGE *maple;
    UL_IMAGE *ashton;

    UL_IMAGE *crusher;
};

inline partyMembers party;

class animationMaker {
private:
    int frameWidth{};
    int frameHeight{};
    int frameCount{};
    int delayBetweenFrames{};
public:
    void animConstructor(int width,int height, int count,int delay) {
        frameWidth=width;
        frameHeight=height;
        frameCount=count;
        delayBetweenFrames=delay;
    }; //Slightly confusing, sorry
};

std::vector<std::string> initAnimation(const std::string& group);
void playAnimation(std::string name);
#endif //MDELTA_ANIMS_H
