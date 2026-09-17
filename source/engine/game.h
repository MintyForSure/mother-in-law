//
// Created by tailofhell on 8/26/26.
//

#ifndef MDELTA_GAME_H
#define MDELTA_GAME_H
#include <stdbool.h>
#include <string>

extern std::string partyMembers[];
extern int partyMemberCount;
extern char gameState;
extern bool debug;
extern bool inWindow;
extern int mapleHP[]; //Target HP, Current HP, Max HP
extern int maplePP[];
extern int mapleStats[]; //maxHP,maxPP,atk,def,speed
extern int ashtonHP[];
extern int ashtonPP[];
extern int ashtonStats[];
extern std::string inventory[];
void gameInit();
void gameLogic();
void battleLogic();
#endif //MDELTA_GAME_H
