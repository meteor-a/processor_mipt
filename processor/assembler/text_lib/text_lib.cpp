#include "text_lib.h"

/*!
    * Function for swap two StringStruct elems
*/
_ERRORS_TEXTLIB SwapStr(StringStruct* left, StringStruct* right) {
  if (_IsBadReadPtr(left)) {
    return _ERRORS_TEXTLIB::ERROR_BAD_PTR;
  }
  if (_IsBadReadPtr(right)) {
    return _ERRORS_TEXTLIB::ERROR_BAD_PTR;
  }

  StringStruct tmp = *left;
  *left = *right;
  *right = tmp;

  return _ERRORS_TEXTLIB::SUCCESSFUL;
}

/*--------------------------------------------------------------------------*/
/**
    * Function to count strings in buffer of text code
*/
long long CountNumOfStrings(StringStruct* str_input) {
  if (_IsBadReadPtr(str_input)) {
    return -1;
  }

  long long num_of_strings = 0;
  for (size_t cur_symb = 0; cur_symb < strlen(str_input->str); ++cur_symb) {
    if (str_input->str[cur_symb] == '\n' || str_input->str[cur_symb] == '\r') {
      ++num_of_strings;
    }
  }

  return num_of_strings + 1;
}

/**
    * Constructor default
*/
_ERRORS_TEXTLIB TextConstructor(TextStruct* text) {
  if (_IsBadReadPtr(text)) {
    return _ERRORS_TEXTLIB::ERROR_BAD_PTR;
  }

  if (!(text->num_strings == 0 &&
    text->strings_text == nullptr)) {

    return _ERRORS_TEXTLIB::FAILED_REPEAT_CONSTRUCTOR;
  }

  text->num_strings = 0;
  text->strings_text = nullptr;

  return _ERRORS_TEXTLIB::SUCCESSFUL;
}

/**
    * Destructor
*/
_ERRORS_TEXTLIB TextDestructor(TextStruct* text) {
  if (_IsBadReadPtr(text)) {
    return _ERRORS_TEXTLIB::ERROR_BAD_PTR;
  }

  if (_IsBadReadPtr(text->strings_text)) {
    return _ERRORS_TEXTLIB::ERROR_BAD_PTR;
  }

  if (_IsBadReadPtr(text->text.str)) {
    return _ERRORS_TEXTLIB::ERROR_BAD_PTR;
  }

  free(text->strings_text);
  free(text->text.str);

  return _ERRORS_TEXTLIB::SUCCESSFUL;
}

_ERRORS_TEXTLIB EraseSpaceStartEndStr(StringStruct* str_input) {
    if (_IsBadReadPtr(str_input)) {
        return _ERRORS_TEXTLIB::ERROR_BAD_PTR;
    }

    for (size_t start_symb = 0, end_symb = str_input->lenght - 1; start_symb < end_symb;) {
        if (str_input->str[start_symb] == ' ') {
            str_input->str[start_symb] = '\0';
            ++str_input->str;
            --str_input->lenght;
            ++start_symb;
            continue;
        }
        if (str_input->str[end_symb] == ' ') {
            str_input->str[end_symb] = '\0';
            --str_input->lenght;
            --end_symb;
            continue;
        }

        break;
    }

    return _ERRORS_TEXTLIB::SUCCESSFUL;
}

/**
    * Function for calloc memory for buffer
*/
_ERRORS_TEXTLIB GetMemoryForTextBuf(TextStruct* programm_code_text, FILE* file_code_text) {
  programm_code_text->text.str = (char*)calloc(getFileSize(file_code_text) + 1, sizeof(char));
  assert(programm_code_text->text.str != nullptr);

  if (programm_code_text->text.str == nullptr) {
    return _ERRORS_TEXTLIB::ERROR_CALLOC_FOR_BUFFER;
  }

  return _ERRORS_TEXTLIB::SUCCESSFUL;
}