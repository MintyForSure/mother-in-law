//
// Created by tailofhell on 8/24/26.
//

#ifndef MDELTA_HUD_H
#define MDELTA_HUD_H
//extern char hudType;
void battleHudInit();
void hudRender(char hudType);
void hudRenderSub(const std::string& hudType);
void hudShake(std::string member,int intensity);
extern int battleMenuState;
#endif //MDELTA_HUD_H