//
// Created by tailofhell on 8/24/26.
//

#include "menus.h"
#include <ulib/ulib.h>

void menuInput(const bool fallbackL, const bool fallbackR) {
    if (ul_keys.pressed.left && fallbackL == false) {
        hSelected--;
    }
    else if (ul_keys.pressed.right && fallbackR == false) {
        hSelected++;
    }
    if (ul_keys.pressed.up && fallbackL == false) {
        vSelected--;
    }
    else if (ul_keys.pressed.down && fallbackR == false) {
        vSelected++;
    }
}