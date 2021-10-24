#include "disassembler.h"

#define DEF_CMD(cmd_in, num_args, is_leftside_arg, required)                               \
    else if (((int)buffer[count_byte] & 31) == (int)ASSEMBLER_COMMANDS::CMD_##cmd_in) {    \
        fprintf(file_disassembler, #cmd_in);                                               \
        if (num_args > 0) {                                                                \
            is_mem = ((int)buffer[count_byte] & 128) == RAM_ARG_CMD;                       \
            is_reg = ((int)buffer[count_byte] & 64) == REG_ARG_CMD;                        \
            is_const = ((int)buffer[count_byte] & 32) == CONST_ARG_CMD;                    \
            ++count_byte;                                                                  \
            reg[1] = { 0 };                                                                \
            arg_const = 0;                                                                 \
            if (is_reg && is_const) {                                                      \
                reg[0] = buffer[count_byte++];                                             \
                arg_const = *((int*)(buffer + count_byte));                                \
                count_byte += sizeof(int);                                                 \
            } else if (is_const) {                                                         \
                arg_const = *((int*)(buffer + count_byte));                                \
                count_byte += sizeof(int);                                                 \
            }                                                                              \
            else if (is_reg) {                                                             \
                reg[0] = buffer[count_byte++];                                             \
            }                                                                              \
            if (is_mem) {                                                                  \
                fprintf(file_disassembler, " [");                                          \
                if (is_reg && is_const) {                                                  \
                    fprintf(file_disassembler, "%cx+%d", reg[0], arg_const);               \
                } else if (is_reg) {                                                       \
                    fprintf(file_disassembler, "%cx", reg[0]);                             \
                }                                                                          \
                else if (is_const) {                                                       \
                    fprintf(file_disassembler, "%d", arg_const);                           \
                }                                                                          \
                fprintf(file_disassembler, "]");                                           \
            } else {                                                                       \
                if (is_reg && is_const) {                                                  \
                    fprintf(file_disassembler, " %cx+%d", reg[0], arg_const);              \
                } else if (is_reg) {                                                       \
                    fprintf(file_disassembler, " %cx", reg[0]);                            \
                }                                                                          \
                else if (is_const) {                                                       \
                    fprintf(file_disassembler, " %d", arg_const);                          \
                }                                                                          \
            }                                                                              \
        } else {                                                                           \
            ++count_byte;                                                                  \
        }                                                                                  \
        fprintf(file_disassembler, "\n");                                                  \
    }
    

void CreateTextFromAssembler(const char* filename_assembler, const char* filename_output) {
    FILE* file_assembler = fopen(filename_assembler, "rb");

    HEADER_ASM_FILE header = {};
    fread(&header, sizeof(HEADER_ASM_FILE), 1, file_assembler);
    fseek(file_assembler, sizeof(HEADER_ASM_FILE), SEEK_SET);

    char* buffer = (char*)calloc(header.size_in_byte, sizeof(char));
    size_t num_read_symb = fread(buffer, sizeof(char), header.size_in_byte, file_assembler);

    fclose(file_assembler);

    FILE* file_disassembler = fopen(filename_output, "w");

    for (size_t count_byte = 0; count_byte < num_read_symb;) {
        bool is_mem = false;                  
        bool is_reg = false;                   
        bool is_const = false;               
        char reg[2] = { 0 };                                                           
        int arg_const = 0;

        if (1 == 0) {

        }

        #include "../../commands/def/cmd_def.h"

    }

    fclose(file_disassembler);
}

#undef DEF_CMD
