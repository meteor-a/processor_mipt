
#include "cpu.h"

int main(int argc, char* argv[]) {
    const char* filename_asm = DEFAULT_ASSEMBLER_FILE_NAME;

    if (argc == 2) {
        filename_asm = argv[1];
    }
    ExecuteCPU(filename_asm);
}
     