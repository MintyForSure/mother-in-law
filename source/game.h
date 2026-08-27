//
// Created by tailofhell on 8/26/26.
//

#ifndef MSHARP_GAME_H
#define MSHARP_GAME_H
#include <stdbool.h>

extern int partyMembers;
extern char gameState;
extern bool debug;
void gameInit();
void gameLogic();
void battleLogic();
#endif //MSHARP_GAME_H
