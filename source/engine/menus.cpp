//
// Created by tailofhell on 8/24/26.
//
#include <maxmod9.h>
#include "menus.h"
#include "sound.h"
#include <ulib/ulib.h>
#include "soundbank.h"
#include "soundbank_bin.h"

void menuInput(const bool fallbackL, const bool fallbackR) {
    if (ul_keys.pressed.left && fallbackL == false) {
        mmEffect(SFX_HSELECT);
        hSelected--;
    }
    else if (ul_keys.pressed.right && fallbackR == false) {
        mmEffect(SFX_HSELECT);
        hSelected++;
    }
    if (ul_keys.pressed.up && fallbackL == false) {
        mmEffect(SFX_VSELECT);
        vSelected--;
    }
    else if (ul_keys.pressed.down && fallbackR == false) {
        mmEffect(SFX_VSELECT);
        vSelected++;
    }
}