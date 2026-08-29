//
// Created by tailofhell on 8/26/26.
//

#ifndef MDELTA_GAME_H
#define MDELTA_GAME_H
#include <stdbool.h>

extern int partyMembers;
extern char gameState;
extern bool debug;
extern int mapleHP[];
extern int ashtonHP[];
void gameInit();
void gameLogic();
void battleLogic();
#endif //MDELTA_GAME_H
