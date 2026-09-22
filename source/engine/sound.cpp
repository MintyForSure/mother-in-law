//
// Created by tailofhell on 9/6/26.
//

#include "sound.h"

#include <iostream>
#include <maxmod9.h>
#include <nds.h>

#include "soundbank.h"
//#include "soundbank_bin.h"

void audioInit() {
    std::cout<<"mmInitDefault: "<<mmInitDefault("nitro:/soundbank.bin")<<std::endl;
    soundEnable();
    std::cout<<"mmGetModuleCount(): "<<mmGetModuleCount()<<std::endl;
    std::cout<<"mmGetSampleCount(): "<<mmGetSampleCount()<<std::endl;
    std::cout<<"mmLoadEffect(SFX_HSELECT): "<<mmLoadEffect(SFX_HSELECT)<<std::endl;
    mmLoadEffect(SFX_VSELECT);
    mmLoadEffect(SFX_SELECT);
    mmLoadEffect(SFX_DESELECT);
}