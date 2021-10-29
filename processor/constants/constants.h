#ifndef CONSTANTS_H__
#define CONSTANTS_H__

/*------ASSEMBLER------*/

typedef long long CPU_ARG_INT_T;
typedef double    CPU_ARG_REAL_T;

static const char* DEFAULT_FILENAME_CODE       = "code.txt";
static const char* DEFAULT_ASSEMBLER_FILE_NAME = "assembler.txt";

const int MAX_CODE_LENGTH    = 1024;
const int MAX_COMMAND_LENGTH = 30;
const int MAX_STRING_LENGTH  = 30;
const int MAX_LABELS_COUNT   = 100;
const int MAX_LABEL_LENGTH   = 30;

const int PRECISION = 1000;

const int SIGNATURE_ASM     = 48613;
const int VER_ASSEMBLER_CMD = 1;

const int CONST_ARG_CMD = 32;
const int REG_ARG_CMD   = 64;
const int RAM_ARG_CMD   = 128;

struct HEADER_ASM_FILE {
    int signature = SIGNATURE_ASM;
    int ver_asm = VER_ASSEMBLER_CMD;
    size_t size_in_byte = 0;
};

#define HEADER_INIT(header, size)            \
    header.signature    = SIGNATURE_ASM;     \
    header.ver_asm      = VER_ASSEMBLER_CMD; \
    header.size_in_byte = size;

/*------DISASSEMBLER------*/

static const char* DEFAULT_DISASSEMBLER_FILE_NAME = "disassembler.txt";

const int MAKE_NULL_FLAGS_BYTE = 31;

/*------CPU------*/

const int HEIGHT_WINDOW = 300;
const int WIDTH_WINDOW  = 300;

const int RAM_BYTE = 1024;
const int RAM_VIDEO_BYTE = HEIGHT_WINDOW * WIDTH_WINDOW;

#endif