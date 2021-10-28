#ifndef CPU_H__
#define CPU_H__

/*--------------------------------------------------------------------------*/

#include <cstddef>
#include <math.h>
/*--------------------------------------------------------------------------*/

#include "stack_lib/stack_lib.h"
#include "../constants/constants.h"
#include "../constants/commands.h"
#include "graphics/graphics.h"
#include "../logger/logger.h"

/*--------------------------------------------------------------------------*/


#ifdef __linux__
    #define LINUX_CODE(...) __VA_ARGS__
#elif _WIN32
    #define LINUX_CODE(...)
#endif

/*--------------------------------------------------------------------------*/

struct REGISTER {
    int registr[4] = {};
};

struct RAM {
    size_t size_ram = RAM_BYTE;
    size_t size_video_ram = RAM_VIDEO_BYTE;
    int ram[RAM_BYTE + RAM_VIDEO_BYTE] = {};
};

struct CPU {
    REGISTER reg = {};
    RAM      ram_memory = {};
    size_t   ip = 0;
    char* code = {};
//    LINUX_CODE(sf::RenderWindow window;)
    Stack_t stack = {};
    Stack_t stack_call = {};
};

/*--------------------------------------------------------------------------*/

int    ExecuteCPU  (const char* filename_assembler);
size_t InitAsmCode (CPU* cpu, const char* filename_assembler);

#endif
