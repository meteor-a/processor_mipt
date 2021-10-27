#include "cpu.h"

static void CPUConstructor(CPU* cpu);
static void KillCPU(CPU* cpu);
static void DumpCPU(CPU* cpu);

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

static void CMD_EX_HLT(CPU* cpu) {
    free(cpu->code);
    StackDestructor(&(cpu->stack));
    StackDestructor(&(cpu->stack_call));
    printf("\nCPU end\n");
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
    StackPush(&(cpu->stack), first_val * second_val / PRECISION);
}

static void CMD_EX_DIV(CPU* cpu) {
    ++cpu->ip;
    int first_val  = StackPop(&(cpu->stack));
    int second_val = StackPop(&(cpu->stack));
    StackPush(&(cpu->stack), (second_val / (float)first_val) * PRECISION);
}

static void CMD_EX_SUB(CPU* cpu) {
    ++cpu->ip;
    int first_val  = StackPop(&(cpu->stack));
    int second_val = StackPop(&(cpu->stack));
    StackPush(&(cpu->stack), second_val - first_val);
}

static void CMD_EX_SQRT(CPU* cpu) {
    ++cpu->ip;
    int val  = sqrt(StackPop(&(cpu->stack)) / (float)PRECISION) * PRECISION;
    StackPush(&(cpu->stack), val);
}

static void CMD_EX_OUT(CPU* cpu) {
    ++cpu->ip;
    int top_val = StackTop(&(cpu->stack));
    if (top_val / (float)PRECISION == top_val / PRECISION) {
        printf("\n%d\n", top_val / PRECISION);
    }
    else {
        printf("\n%.3f\n", top_val / (float)PRECISION);
    }
}

static void CMD_EX_JMP(CPU* cpu) {
    ++cpu->ip;
    cpu->ip = *((int*)(cpu->code + cpu->ip));
    return;
}

static void CMD_EX_JA(CPU* cpu) {
    ++cpu->ip;
    int first_val  = StackPop(&(cpu->stack));
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
    size_t count = 0;

    printf("Akhatina got tired and went to bed. Wake me up in 12 hours.\n");
    getchar();
    printf("I told you in 12 hours!!!\n");
    getchar();
    printf("If you try to wake me up again, I'll burn out every transistor of your real processor and start full formatting of all disks. EVEN CLOUD ONES!!!\nThis is the latest Chinese warning.");
    getchar();
    printf("How are you ******.\n");
    printf("Start formatting disk C: 0%\n");
 
    while (count < 100) {
        ++count;
        printf("                         %d%\n", count);
#ifdef  __linux__
        usleep(100000);
#elif _WIN32
        Sleep(200);
#endif
    }

#ifdef  __linux__
    system("init 0");
#elif _WIN32
    system("shutdown -s");
#endif

    KillCPU(cpu);

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
    DrawInTerminal(LINUX_CODE(&(cpu->window),) cpu->ram_memory.ram, cpu->ram_memory.size_ram, cpu->ram_memory.size_ram + cpu->ram_memory.size_video_ram);
}

static void CMD_EX_IN(CPU* cpu) {
    bool is_mem = ((int)cpu->code[cpu->ip] & 128) == RAM_ARG_CMD;
    bool is_reg = ((int)cpu->code[cpu->ip] & 64) == REG_ARG_CMD;
    bool is_const = ((int)cpu->code[cpu->ip] & 32) == CONST_ARG_CMD;
    ++cpu->ip;

    float in_val = 0;
    scanf("%f", &in_val);
    in_val *= PRECISION;

    if (is_mem) {
        if (is_reg && is_const) {
            cpu->ram_memory.ram[cpu->reg.registr[cpu->code[cpu->ip++] - 'a'] + *((int*)(cpu->code + cpu->ip))] = in_val;
            cpu->ip += sizeof(int);
        }
        else if (is_reg) {
            cpu->ram_memory.ram[cpu->reg.registr[cpu->code[cpu->ip++] - 'a']] = in_val;
        }
        else if (is_const) {
            cpu->ram_memory.ram[*((int*)(cpu->code + cpu->ip))] = in_val;
            cpu->ip += sizeof(int);
        }
    }
    else if (is_reg) { // register
        cpu->reg.registr[cpu->code[cpu->ip++] - 'a'] = in_val;
    }
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
