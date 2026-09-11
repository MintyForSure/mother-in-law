cmake_minimum_required(VERSION 3.16)

project(mdelta LANGUAGES CXX)

set(BLOCKSDS $ENV{BLOCKSDS})
set(BLOCKSDSEXT $ENV{BLOCKSDSEXT})

set(MDELTA_SRC source/main.cpp)
include_directories(${BLOCKSDS}/libs/libnds/include)
include_directories(${BLOCKSDS}/libs/maxmod/include)
include_directories(${BLOCKSDSEXT}/ulibrary/include)

add_executable(mdelta ${MDELTA_SRC})

if(NINTENDO_DS)
    if(NDS_DSI_EXCLUSIVE)
        set(SUBTITLE "Twilight Edition")
    else()
        set(SUBTITLE "For NDS")
    endif()

    nds_create_rom(portable_demo
            NAME "Mother: Delta"
            SUBTITLE "${SUBTITLE}"
            AUTHOR "mintyforsure"
            ICON "${CMAKE_CURRENT_SOURCE_DIR}/icon.bmp"
            ARM7 "${CMAKE_CURRENT_SOURCE_DIR}/arm9.elf"
    )
endif()
