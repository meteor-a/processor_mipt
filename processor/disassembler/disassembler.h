#pragma once

/*--------------------------------------------------------------------------*/

#include <malloc.h>

/*--------------------------------------------------------------------------*/

#include "../commands/assembler_commands.h"
#include "../base_lib/default_lib.h"

/*--------------------------------------------------------------------------*/

void CreateTextFromAssembler(const char* filename_assembler, const char* filename_output);
