#include "cpu.h"

int CPUConstructor(CPU* cpu);
int CPUDestructor (CPU* cpu);
int KillCPU(const char* text_err, TypeLog type_log, const char* filename, size_t num_str);
                                                                                        
#define DEF_CMD(cmd_in, num_args, is_leftside_arg, ident_of_type, command_action)                         \
    else if (((int)cpu.code[cpu.ip] & MAKE_NULL_FLAGS_BYTE) == (int)PROCESSOR_COMMANDS::CMD_##cmd_in) {   \
        command_action                                                                                    \
    }

int ExecuteCPU(const char* filename_assembler) {
    CPU cpu = {};
    CPUConstructor(&cpu);

    size_t size_code_byte = InitAsmCode(&cpu, filename_assembler);

    for (; cpu.ip < size_code_byte;) {
        if (1 == 0) {

        }

        #include "../constants/cmd_def.h"

        else {
            KillCPU("Failed: Cant recognize command", TypeLog::ERROR_, LOCATION__(cpu));
            CPUDestructor(&cpu);
        }
    }

    return 0;
}

#undef DEF_CMD

int CPUConstructor(CPU* cpu) {
    StackConstructor((cpu->stack));
    StackConstructor((cpu->stack_call));
    cpu->code = nullptr;
    cpu->ip = 0;
    cpu->reg = {};
    cpu->ram_memory.ram = (CPU_ARG_INT_T*)calloc(cpu->ram_memory.size_ram + cpu->ram_memory.size_video_ram, sizeof(CPU_ARG_INT_T));
    if (_IsBadReadPtr(cpu->ram_memory.ram)) {
        KillCPU("Failed: Cant get memory for RAM", TypeLog::ERROR_, LOCATION__(cpu->ram_memory.ram));
    }

    return 0;
}

int CPUDestructor(CPU* cpu) {
    StackDestructor(&(cpu->stack));
    StackDestructor(&(cpu->stack_call));
    free(cpu->ram_memory.ram);
    free(cpu->code);

    return 0;
}

size_t InitAsmCode(CPU* cpu, const char* filename_assembler) {
    if (_IsBadReadPtr(cpu)) {
        KillCPU("Failed: Bad ptr", TypeLog::ERROR_, LOCATION__(cpu));
    }

    FILE* file_assembler = fopen(filename_assembler, "rb");
    if (_IsBadReadPtr(file_assembler)) {
        KillCPU("Failed: Cant open file to write disassembler code", TypeLog::ERROR_, LOCATION__(file_assembler));
    }
    
    HEADER_ASM_FILE header = {};
    fread(&header, sizeof(HEADER_ASM_FILE), 1, file_assembler);
    fseek(file_assembler, sizeof(HEADER_ASM_FILE), SEEK_SET);

    if (header.ver_asm != VER_ASSEMBLER_CMD) {
        KillCPU("Version of header assembler file do not equal current version. Please re-create assembler file.", TypeLog::WARNING_, LOCATION__(header));
    }

    cpu->code = (char*)calloc(header.size_in_byte, sizeof(char));
    if (_IsBadReadPtr(cpu->code)) {
        KillCPU("Failed: Cant calloc memory", TypeLog::ERROR_, LOCATION__(cpu->code));
    }
    size_t num_read_symb = fread(cpu->code, sizeof(char), header.size_in_byte, file_assembler);

    fclose(file_assembler);

    return header.size_in_byte;
}

int KillCPU(const char* text_err, TypeLog type_log, const char* filename, size_t num_str) {
    CreateLog(text_err, type_log, filename, num_str);
    return -1;
}
