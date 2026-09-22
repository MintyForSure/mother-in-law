//
// Created by tailofhell on 8/28/26.
//

#ifndef MDELTA_BATTLE_H
#define MDELTA_BATTLE_H
#include <string>

void battleInit(const char *enemy0, const char *enemy1 = "empty", const char *enemy2 = "empty");
void battleProcess();

extern int battlePhase;
extern std::string selectingPartyMember;
extern std::string mapleAction[];
extern std::string aaronAction[];

#endif //MDELTA_BATTLE_H
