#pragma once
#include "../assembler/asm_const.h"
#include <cstddef>

struct HEADER_ASM_FILE {
    int signature = SIGNATURE_ASM;
    int ver_asm = VER_ASSEMBLER_CMD;
    size_t size_in_byte = 0;
};

struct ARG_TYPE {
    int size_byte = 0;
    char reg = 0;
    bool is_reg = false;
    int const_arg = 0;
    bool is_const = false;
    bool is_mem = false;
};

struct LABEL_ {
    char name_label[MAX_LABEL_LENGTH] = { 0 };
    int  pos = 0;
};

#define HEADER_INIT(header, size)           \
    header.signature    = SIGNATURE_ASM;    \
    header.ver_asm      = VER_ASSEMBLER_CMD;\
    header.size_in_byte = size;

#define DEF_CMD(cmd, rabbish_num_args, rabbish_is_leftside_arg, required) \
    CMD_##cmd,

enum class ASSEMBLER_COMMANDS {
    CMD_ZERO,
    #include "def/cmd_def.h"
    CMD_MAX
};

#undef DEF_CMD
