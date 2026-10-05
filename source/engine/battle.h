//
// Created by tailofhell on 8/28/26.
//

#ifndef MDELTA_BATTLE_H
#define MDELTA_BATTLE_H
#include <string>
#include <vector>

void battleInit(std::string enemy0, const std::string& enemy1 = "empty", const std::string& enemy2 = "empty");
void battleProcess();

std::vector<int> pressTurnCount(bool player, bool refresh);

extern int battlePhase;
extern std::string selectingPartyMember;
extern std::string mapleAction[];
extern std::string aaronAction[];
extern std::string crusherAction[];
extern bool mapleDefending;
extern bool aaronDefending;
extern bool crusherDefending;
extern void passTurn(bool player=true, bool fullTurn=true);
extern void playerMove(std::string member);
extern void makeHalfTurn(int turn);
extern std::vector<std::string> enemyList;

#endif //MDELTA_BATTLE_H
