/*
#include "assembler.h"
#include "../constant/constant_filename.h"

int main(int argc, char* argv[]) {
    const char* filename_code = DEFAULT_CODE_FILE_NAME;
    const char* filename_asm = DEFAULT_ASSEMBLER_FILE_NAME;

    if (argc == 2) {
        filename_code = argv[1];
    }
    else if (argc == 3) {
        filename_code = argv[1];
        filename_asm = argv[2];
    }

    CreateAssemblerFile(filename_code, filename_asm);
}
  */