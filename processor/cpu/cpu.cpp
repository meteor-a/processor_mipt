#include "cpu.h"

static void CPUConstructor(CPU* cpu);
static void KillCPU(CPU* cpu);

#define DEF_CMD(cmd_in, num_args, is_leftside_arg, required) \
    static void CMD_EX_##cmd_in(CPU* cpu);    

#include "../commands/def/cmd_def.h"

#undef DEF_CMD

#define DEF_CMD(cmd_in, num_args, is_leftside_arg, required)                                        \
    else if (((int)cpu.code[cpu.ip] & 31) == (int)ASSEMBLER_COMMANDS::CMD_##cmd_in) {     \
            CMD_EX_##cmd_in(&cpu);                                                        \
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
    printf("\nCPU dead\n");
    exit(-1);
}

static void CMD_EX_HLT(CPU* cpu) {
    free(cpu->code);
    StackDestructor(&(cpu->stack));
    printf("CPU end");
    exit(0);
}

static void CMD_EX_PUSH(CPU* cpu) {
    bool is_mem = ((int)cpu->code[cpu->ip] & 128) == RAM_ARG_CMD;
    bool is_reg = ((int)cpu->code[cpu->ip] & 64) == REG_ARG_CMD;
    bool is_const = ((int)cpu->code[cpu->ip] & 32) == CONST_ARG_CMD;
    ++cpu->ip;

    int reg_val = 0;
    int arg_const = 0;
    if (is_reg) {
        reg_val = cpu->reg.registr[cpu->code[cpu->ip] - 'a'];
        ++cpu->ip;
    }
    if (is_const) {
        arg_const = *((int*)(cpu->code + cpu->ip));
        cpu->ip += sizeof(int);
    }
    if (is_mem) {
        StackPush(&(cpu->stack), cpu->ram_memory.ram[is_reg ? (is_const ? (reg_val + arg_const) : reg_val) : arg_const]);
    }
    else {
        StackPush(&(cpu->stack), is_reg ? (is_const ? (reg_val + arg_const) : reg_val) : arg_const);
    }
}

static void CMD_EX_POP(CPU* cpu) {
    int pop_val = StackPop(&(cpu->stack));

    bool is_mem = ((int)cpu->code[cpu->ip] & 128) == RAM_ARG_CMD;
    bool is_reg = ((int)cpu->code[cpu->ip] & 64) == REG_ARG_CMD;
    bool is_const = ((int)cpu->code[cpu->ip] & 32) == CONST_ARG_CMD;
    ++cpu->ip;

    if (is_mem) {
        if (is_reg && is_const) {
            cpu->ram_memory.ram[cpu->reg.registr[cpu->code[cpu->ip++] - 'a'] + *((int*)(cpu->code + cpu->ip))] = pop_val;
            cpu->ip += sizeof(int);
        } else if (is_reg) {
            cpu->ram_memory.ram[cpu->reg.registr[cpu->code[cpu->ip++] - 'a']] = pop_val;
        } else if (is_const) {
            cpu->ram_memory.ram[*((int*)(cpu->code + cpu->ip))] = pop_val;
            cpu->ip += sizeof(int);
        }
    } else if (is_reg) { // register
        cpu->reg.registr[cpu->code[cpu->ip++] - 'a'] = pop_val;
    }
}

static void CMD_EX_ADD(CPU* cpu) {
    ++cpu->ip;
    int first_val  = StackPop(&(cpu->stack));
    int second_val = StackPop(&(cpu->stack));
    StackPush(&(cpu->stack), first_val + second_val);
}

static void CMD_EX_MUL(CPU* cpu) {
    ++cpu->ip;
    int first_val = StackPop(&(cpu->stack));
    int second_val = StackPop(&(cpu->stack));
    StackPush(&(cpu->stack), first_val * second_val);
}

static void CMD_EX_DIV(CPU* cpu) {
    ++cpu->ip;
    int first_val = StackPop(&(cpu->stack));
    int second_val = StackPop(&(cpu->stack));
    StackPush(&(cpu->stack), first_val / second_val);
}

static void CMD_EX_SUB(CPU* cpu) {
    ++cpu->ip;
    int first_val = StackPop(&(cpu->stack));
    int second_val = StackPop(&(cpu->stack));
    StackPush(&(cpu->stack), second_val - first_val);
}

static void CMD_EX_OUT(CPU* cpu) {
    ++cpu->ip;
    printf("%d", StackTop(&(cpu->stack)));
}

static void CMD_EX_JMP(CPU* cpu) {
    ++cpu->ip;
    cpu->ip = *((int*)(cpu->code + cpu->ip));
    return;
}

static void CMD_EX_JA(CPU* cpu) {
    ++cpu->ip;
    int first_val = StackPop(&(cpu->stack));
    int second_val = StackPop(&(cpu->stack));
    if (second_val > first_val) {
        cpu->ip = *((int*)(cpu->code + cpu->ip));
        return;
    }
    cpu->ip += sizeof(int);
}

static void CMD_EX_JAE(CPU* cpu) {
    ++cpu->ip;
    int first_val = StackPop(&(cpu->stack));
    int second_val = StackPop(&(cpu->stack));
    if (second_val >= first_val) {
        cpu->ip = *((int*)(cpu->code + cpu->ip));
        return;
    }
    cpu->ip += sizeof(int);
}

static void CMD_EX_JB(CPU* cpu) {
    ++cpu->ip;
    int first_val = StackPop(&(cpu->stack));
    int second_val = StackPop(&(cpu->stack));
    if (second_val < first_val) {
        cpu->ip = *((int*)(cpu->code + cpu->ip));
        return;
    }
    cpu->ip += sizeof(int);
}

static void CMD_EX_JBE(CPU* cpu) {
    ++cpu->ip;
    int first_val = StackPop(&(cpu->stack));
    int second_val = StackPop(&(cpu->stack));
    if (second_val <= first_val) {
        cpu->ip = *((int*)(cpu->code + cpu->ip));
        return;
    }
    cpu->ip += sizeof(int);
}

static void CMD_EX_JE(CPU* cpu) {
    ++cpu->ip;
    int first_val = StackPop(&(cpu->stack));
    int second_val = StackPop(&(cpu->stack));
    if (second_val == first_val) {
        cpu->ip = *((int*)(cpu->code + cpu->ip));
        return;
    }
    cpu->ip += sizeof(int);
}

static void CMD_EX_JNE(CPU* cpu) {
    ++cpu->ip;
    int first_val = StackPop(&(cpu->stack));
    int second_val = StackPop(&(cpu->stack));
    if (second_val != first_val) {
        cpu->ip = *((int*)(cpu->code + cpu->ip));
        return;
    }
    cpu->ip += sizeof(int);
}

static void CMD_EX_JH(CPU* cpu) {
    ++cpu->ip;
    int first_val = StackPop(&(cpu->stack));
    int second_val = StackPop(&(cpu->stack));
    cpu->ip = *((int*)(cpu->code + cpu->ip));
    return;
}

static void CMD_EX_CALL(CPU* cpu) {
    ++cpu->ip;
    StackPush(&(cpu->stack_call), cpu->ip + sizeof(int));
    cpu->ip = *((int*)(cpu->code + cpu->ip));
    return;
}

static void CMD_EX_RET(CPU* cpu) {
    cpu->ip = StackPop(&(cpu->stack_call));
}

static void CMD_EX_GRAPH(CPU* cpu) {
    ++cpu->ip;
    //printf("DRAW");
    DrawInTerminal(cpu->ram_memory.ram, cpu->ram_memory.size_ram, cpu->ram_memory.size_ram + cpu->ram_memory.size_video_ram);
}
