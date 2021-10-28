#include "disassembler.h"

int WriteCommand  (const char* command, char* buffer, size_t* count_byte, int num_args, FILE* file_disassembler);
int WriteArgument (char* buffer, size_t* count_byte, FILE* file_disassembler);
int KillDisasm    (const char* text_err, TypeLog type_log, const char* filename, size_t num_str);

int WriteCommand(const char* command, char* buffer, size_t *count_byte, int num_args, FILE* file_disassembler) {
    if (_IsBadReadPtr(buffer)) {
        KillDisasm("Failed: Bad ptr", TypeLog::ERROR_, LOCATION__(buffer));
    }
    if (_IsBadReadPtr(count_byte)) {
        KillDisasm("Failed: Bad ptr", TypeLog::ERROR_, LOCATION__(count_byte));
    }
    if (_IsBadReadPtr(file_disassembler)) {
        KillDisasm("Failed: Cant open file to write disassembler code", TypeLog::ERROR_, LOCATION__(file_disassembler));
    }

    fprintf(file_disassembler, command);
    if (num_args > 0) {
        WriteArgument(buffer, count_byte, file_disassembler);
    }
    else {
        ++(*count_byte);
    }
    fprintf(file_disassembler, "\n");

    return 0;
}

int WriteArgument(char* buffer, size_t* count_byte, FILE* file_disassembler) {
    if (_IsBadReadPtr(buffer)) {
        KillDisasm("Failed: Bad ptr", TypeLog::ERROR_, LOCATION__(buffer));
    }
    if (_IsBadReadPtr(count_byte)) {
        KillDisasm("Failed: Bad ptr", TypeLog::ERROR_, LOCATION__(count_byte));
    }
    if (_IsBadReadPtr(file_disassembler)) {
        KillDisasm("Failed: Cant open file to write disassembler code", TypeLog::ERROR_, LOCATION__(file_disassembler));
    } 

    bool is_mem   = (int)buffer[*count_byte] & RAM_ARG_CMD;
    bool is_reg   = (int)buffer[*count_byte] & REG_ARG_CMD;
    bool is_const = (int)buffer[*count_byte] & CONST_ARG_CMD;

    ++(*count_byte);

    char  reg[2]    = { 0 };
    float arg_const = 0;

    if (is_reg) reg[0] = buffer[(*count_byte)++];
    if (is_const) {
        arg_const = (*((int*)(buffer + (*count_byte)))) / (float)PRECISION;
        (*count_byte) += sizeof(arg_const);
    }

    if (is_mem) {
        fprintf(file_disassembler, " [");
        if (is_reg && is_const) {
            fprintf(file_disassembler, "%cx+%d", reg[0], (int)arg_const);
        }
        else if (is_reg) {
            fprintf(file_disassembler, "%cx", reg[0]);
        }
        else if (is_const) {
            fprintf(file_disassembler, "%d", (int)arg_const);
        }
        fprintf(file_disassembler, "]");
    }
    else {
        if (is_reg && is_const) {
            fprintf(file_disassembler, " %cx+%.3f", reg[0], arg_const);
        }
        else if (is_reg) {
            fprintf(file_disassembler, " %cx", reg[0]);
        }
        else if (is_const) {
            fprintf(file_disassembler, " %.3f", arg_const);
        }
    }

    return 0;
}

#define DEF_CMD(cmd_name, num_args, ...)                                                                        \
    else if (((int)buffer[count_byte] & MAKE_NULL_FLAGS_BYTE) == (int)PROCESSOR_COMMANDS::CMD_##cmd_name) {    \
        WriteCommand(#cmd_name, buffer, &count_byte, num_args, file_disassembler);                              \
    }

int CreateTextFromAssembler(const char* filename_assembler, const char* filename_output) {
    FILE* file_assembler = fopen(filename_assembler, "rb");
    if (_IsBadReadPtr(file_assembler)) {
        KillDisasm("Failed: Cant open file with assembler", TypeLog::ERROR_, LOCATION__(file_assembler));
    }

    HEADER_ASM_FILE header = {};
    fread(&header, sizeof(HEADER_ASM_FILE), 1, file_assembler);
    fseek(file_assembler, sizeof(HEADER_ASM_FILE), SEEK_SET);

    if (header.ver_asm != VER_ASSEMBLER_CMD) {
        KillDisasm("Version of header assembler file do not equal current version. Please re-create assembler file.", TypeLog::WARNING_, LOCATION__(file_assembler));
    }

    char* buffer = (char*)calloc(header.size_in_byte, sizeof(char));
    size_t num_read_symb = fread(buffer, sizeof(char), header.size_in_byte, file_assembler);

    fclose(file_assembler);

    FILE* file_disassembler = fopen(filename_output, "w");
    if (_IsBadReadPtr(file_assembler)) {
        KillDisasm("Failed: Cant open file to write disassembler code", TypeLog::ERROR_, LOCATION__(file_assembler));
    }

    for (size_t count_byte = 0; count_byte < num_read_symb;) {
        if (1 == 0) {

        }

        #include "../constants/cmd_def.h"

        else {
            KillDisasm("Failed: Disassembler cant recognize symb", TypeLog::ERROR_, LOCATION__(file_assembler));
        }
    }

    fclose(file_disassembler);

    return 0;
}

#undef DEF_CMD

int KillDisasm(const char* text_err, TypeLog type_log, const char* filename, size_t num_str) {
    CreateLog(text_err, type_log, filename, num_str);
    return -1;
}
