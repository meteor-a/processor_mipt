#pragma once

/*--------------------------------------------------------------------------*/

#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <cstddef>
#include <string.h>

/*--------------------------------------------------------------------------*/

#include "../../base_lib/default_lib.h"

/*--------------------------------------------------------------------------*/

enum class _ERRORS_TEXTLIB {
  SUCCESSFUL,
  FAILED_REPEAT_CONSTRUCTOR,
  FAILED_DESTRUCTOR,
  ERROR_CALLOC_FOR_BUFFER,
  ERROR_BAD_PTR
};

struct StringStruct {
  char* str = nullptr;
  size_t lenght = 0;
};

_ERRORS_TEXTLIB SwapStr(StringStruct* left, StringStruct* right);

/*--------------------------------------------------------------------------*/

struct TextStruct {
  StringStruct  text = {};
  StringStruct* strings_text = nullptr;
  size_t        num_strings = 0;
};

_ERRORS_TEXTLIB TextConstructor(TextStruct* text);
_ERRORS_TEXTLIB TextDestructor(TextStruct* text);

_ERRORS_TEXTLIB GetMemoryForTextBuf(TextStruct* programm_code_text, FILE* file_code_text);
long long       CountNumOfStrings(StringStruct* str_input);
_ERRORS_TEXTLIB EraseSpaceStartEndStr(StringStruct* str_input);