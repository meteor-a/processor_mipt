#pragma once

/*--------------------------------------------------------------------------*/

#include <cstddef>

/*--------------------------------------------------------------------------*/

#include "../commands/assembler_commands.h"
#include "../base_lib/default_lib.h"
#include "stack_lib/stack_lib.h"
#include "../logger/logger.h"
#include "graphics/graphics.h"

/*--------------------------------------------------------------------------*/

struct REGISTER {
    int registr[4] = {};
};

const int RAM_BYTE       = 1024;
const int RAM_VIDEO_BYTE = HEIGHT_WINDOW * WIDTH_WINDOW;

struct RAM {
    size_t size_ram                    = RAM_BYTE;
    size_t size_video_ram              = RAM_VIDEO_BYTE;
    int ram[RAM_BYTE + RAM_VIDEO_BYTE] = {};
};

#ifdef __linux__
    #define LINUX_CODE(...) __VA_ARGS__
#elif _WIN32
    #define LINUX_CODE(...)
#endif

struct CPU {
    REGISTER reg        = {};
    RAM      ram_memory = {};
    size_t   ip         = 0;
    char*    code       = {};
    LINUX_CODE(sf::RenderWindow window;)
    Stack_t stack       = {};
    Stack_t stack_call  = {};
};

void   ExecuteCPU  (const char* filename_assembler);
size_t InitAsmCode (CPU* cpu, const char* filename_assembler);