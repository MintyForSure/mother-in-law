//
// Created by tailofhell on 9/6/26.
//

#include "sound.h"
#include <maxmod9.h>
#include <nds.h>

#include "soundbank.h"
#include "soundbank_bin.h"

void audioInit() {
    mmInitDefaultMem((mm_addr)soundbank_bin);
    soundEnable();

    mmLoadEffect(SFX_HSELECT);
    mmLoadEffect(SFX_VSELECT);
    mmLoadEffect(SFX_SELECT);
    mmLoadEffect(SFX_DESELECT);
}