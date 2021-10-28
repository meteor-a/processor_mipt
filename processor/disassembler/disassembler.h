#ifndef DISASSEMBLER_H__
#define DISASSEMBLER_H__

/*----------------------------------------*/

#include <malloc.h>

/*----------------------------------------*/

#include "../baselib/base_lib.h"
#include "../constants/constants.h"
#include "../constants/commands.h"
#include "../logger/logger.h"

/*----------------------------------------*/

int CreateTextFromAssembler(const char* filename_assembler, const char* filename_output);

#endif