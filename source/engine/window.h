//
// Created by tailofhell on 8/31/26.
//

#ifndef MDELTA_WINDOW_H
#define MDELTA_WINDOW_H
#include <string>
void windowSysInit();
void drawWindow(std::string windowID,int X,int Y,int width,int height,std::string type);
void windowDisplayText(const std::string& line1="",const std::string& line2="");
void drawMenu();
extern bool doWindowDrawing;

#endif //MDELTA_WINDOW_H