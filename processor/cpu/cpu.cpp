#include "cpu.h"

static void CPUConstructor(CPU* cpu);
static void KillCPU(CPU* cpu);
static void DumpCPU(CPU* cpu);

#define DEF_CMD(cmd_in, num_args, is_leftside_arg, required, command_action)              \
    else if (((int)cpu.code[cpu.ip] & 31) == (int)ASSEMBLER_COMMANDS::CMD_##cmd_in) {     \
            command_action                                                                \
    }  

void ExecuteCPU(const char* filename_assembler) {
    CPU cpu = {};
    CPUConstructor(&cpu);

    size_t size_code_byte = InitAsmCode(&cpu, filename_assembler);

    for (; cpu.ip < size_code_byte;) {
        if (1 == 0) {

        }
        #include "../commands/def/cmd_def.h"
        else {
            CreateLog("CPU dead", TypeLog::ERROR_);
            KillCPU(&cpu);
        }
    }
}

#undef DEF_CMD

size_t InitAsmCode(CPU* cpu, const char* filename_assembler) {
    FILE* file_assembler = fopen(filename_assembler, "rb");

    HEADER_ASM_FILE header = {};
    fread(&header, sizeof(HEADER_ASM_FILE), 1, file_assembler);
    fseek(file_assembler, sizeof(HEADER_ASM_FILE), SEEK_SET);

    if (header.ver_asm != VER_ASSEMBLER_CMD) {
        CreateLog("Version of header assembler file do not equal current version. Please re-create assembler file.", TypeLog::WARNING_);
    }

    cpu->code = (char*)calloc(header.size_in_byte, sizeof(char));
    size_t num_read_symb = fread(cpu->code, sizeof(char), header.size_in_byte, file_assembler);

    fclose(file_assembler);

    return header.size_in_byte;
}

static void CPUConstructor(CPU* cpu) {
    StackConstructor((cpu->stack));
    StackConstructor((cpu->stack_call));
    cpu->code = nullptr;
    cpu->ip = 0;
    cpu->reg = {};
    cpu->ram_memory = {};
}

void KillCPU(CPU* cpu) {
    free(cpu->code);
    StackDestructor(&(cpu->stack));
    StackDestructor(&(cpu->stack_call));
    printf("\nCPU dead\n");
    exit(-1);
}

void DumpCPU(CPU* cpu) {
    
}
