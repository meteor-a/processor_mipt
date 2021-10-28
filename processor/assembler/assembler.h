#ifndef ASSEMBLER_H__
#define ASSEMBLER_H__

/*----------------------------------------*/

#include <stdlib.h>

/*----------------------------------------*/

#include "../constants/constants.h"
#include "../logger/logger.h"
#include "../baselib/base_lib.h"
#include "../text_lib/text_lib.h"
#include "../constants/commands.h"

/*----------------------------------------*/

struct LABEL_ {
    char name_label[MAX_LABEL_LENGTH] = { 0 };
    int  pos = 0;
};

/*----------------------------------------*/

int CreateAssemblerFile(const char* filename_code, const char* filename_asm);

#endif
