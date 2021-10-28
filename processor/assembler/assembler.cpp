#include "assembler.h"

int   InputTextCode           (TextStruct* programm_code_text, FILE* file_code_text);
int   TextSeparate            (TextStruct* text/*!< - Pointer for text struct */);
int   SeparateTextByEndOfLine (TextStruct* text);
int   ChangeWordsToCodes      (TextStruct* programm_code_text, const char* filename_assembler_text, size_t* count_labels, LABEL_* labels_arr, int step);
char* ParseCommand            (int num_cmd, char* commands_arr, size_t* ip);
int   ParseArgument           (char* commands_arr, size_t* ip, char* pointer_to_command, char* args, bool is_leftside_arg, 
                               size_t* count_labels, LABEL_* labels_arr, int step);

int   FindPosLabel            (const char* label, size_t count_labels, LABEL_* labels_arr);
int   SetPosLabel             (const char* label, size_t ip, size_t* count_labels, LABEL_* labels_arr);
int   WriteAssemblerToFile    (char* code_arr, size_t size, const char* filename_assembler_text);
int   KillAsm                 (const char* text_err, TypeLog type_log, const char* filename, size_t num_str);


/*--------------------------------------------------------------------------*/


int CreateAssemblerFile(const char* filename_code, const char* filename_asm) {
    FILE* file_input = fopen(filename_code, "r");
    if (_IsBadReadPtr(file_input)) {
        KillAsm("Failed while open file", TypeLog::ERROR_, LOCATION__(file_input));
    }

    TextStruct code_programm = {};
    TextConstructor    (&code_programm);
    GetMemoryForTextBuf(&code_programm, file_input);

    InputTextCode(&code_programm, file_input);

    size_t count_labels = 0;
    LABEL_ labels_arr[MAX_LABELS_COUNT] = { 0 };

    ChangeWordsToCodes(&code_programm, filename_asm, &count_labels, labels_arr, 1);
    ChangeWordsToCodes(&code_programm, filename_asm, &count_labels, labels_arr, 2);

    fclose(file_input);

    TextDestructor(&code_programm);

    return 0;
}

int InputTextCode(TextStruct* code_programm, FILE* file_input) {
    if (_IsBadReadPtr(code_programm)) {
        KillAsm("Failed: Bad ptr", TypeLog::ERROR_, LOCATION__(code_programm));
    }
    if (_IsBadReadPtr(file_input)) {
        KillAsm("Failed: Bad ptr", TypeLog::ERROR_, LOCATION__(file_input));
    }

    fread(code_programm->text.str, sizeof(char), getFileSize(file_input) + 1, file_input);
    code_programm->text.lenght = strlen(code_programm->text.str);

    TextSeparate(code_programm);

    return 0;
}

int TextSeparate(TextStruct* text/*!< - Pointer for text struct */) {
    if (_IsBadReadPtr(text)) {
        KillAsm("Failed: Bad ptr", TypeLog::ERROR_, LOCATION__(file_input));
    }

    text->num_strings = CountNumOfStrings(&text->text);

    text->strings_text = (StringStruct*)calloc(text->num_strings + 1, sizeof(StringStruct));
    if (_IsBadReadPtr(text->strings_text)) {
        KillAsm("Failed: calloc memory", TypeLog::CRITICAL_ERROR_, LOCATION__(text->strings_text));
    }

    SeparateTextByEndOfLine(text);

    return 0;
}

int SeparateTextByEndOfLine(TextStruct* text) {
    if (_IsBadReadPtr(text)) {
        KillAsm("Failed: Bad ptr", TypeLog::ERROR_, LOCATION__(text));
    }

    size_t cur_size_str           = text->text.lenght;
    size_t count_symb_in_cur_line = 0;
    size_t n_line                 = 0;
    for (size_t num_symb = 0; num_symb < cur_size_str + 1; ++num_symb) {
        if (text->text.str[num_symb] == '\n' || text->text.str[num_symb] == '\r' || text->text.str[num_symb] == '\0') {
            text->text.str[num_symb] = '\0';

            text->strings_text[n_line].str = (text->text.str + num_symb - count_symb_in_cur_line);

            if (strlen(text->strings_text[n_line].str) == 0) {
                count_symb_in_cur_line = 0;
                --text->num_strings;
                continue;
            }

            text->strings_text[n_line].lenght = strlen(text->strings_text[n_line].str);

            count_symb_in_cur_line = 0;
            ++n_line;
        }
        else if (text->text.str[num_symb] == ';') {
            while (text->text.str[num_symb] != '\n' && text->text.str[num_symb] != '\r' && text->text.str[num_symb] != '\0') {
                text->text.str[num_symb] = '\0';
                ++count_symb_in_cur_line;
            }
        }
        else {
            ++count_symb_in_cur_line;
        }
    }

    return 0;
}

char* ParseCommand(int num_cmd, char* commands_arr, size_t* ip, bool* is_was_hlt) {
    if (_IsBadReadPtr(commands_arr)) {
        KillAsm("Failed: Bad ptr", TypeLog::ERROR_, LOCATION__(commands_arr));
    }
    if (_IsBadReadPtr(ip)) {
        KillAsm("Failed: Bad ptr", TypeLog::ERROR_, LOCATION__(ip));
    }

    char* ip_command = commands_arr + (*ip);

    commands_arr[(*ip)] = (char)(num_cmd);
    if (num_cmd == (int)PROCESSOR_COMMANDS::CMD_HLT) {
        (*is_was_hlt) = true;
    }

    ++(*ip);

    return ip_command;
}

int ParseArgument(char* commands_arr, size_t* ip, char* pointer_to_command, char* args, bool is_leftside_arg, size_t* count_labels, LABEL_* labels_arr, int step) {
    if (_IsBadReadPtr(commands_arr)) {
        KillAsm("Failed: Bad ptr", TypeLog::ERROR_, LOCATION__(commands_arr));
    }
    if (_IsBadReadPtr(ip)) {
        KillAsm("Failed: Bad ptr", TypeLog::ERROR_, LOCATION__(ip));
    }
    if (_IsBadReadPtr(pointer_to_command)) {
        KillAsm("Failed: Bad ptr", TypeLog::ERROR_, LOCATION__(pointer_to_command));
    }
    if (_IsBadReadPtr(args)) {
        KillAsm("Failed: Bad ptr", TypeLog::ERROR_, LOCATION__(args));
    }

    bool  is_const = false;
    bool  is_reg   = false;
    bool  is_mem   = false;

    char  arg_label[MAX_LABEL_LENGTH] = { 0 };
    int   tmp_const  = 0;
    char  reg[2]     = { 0 };
    float arg_const  = 0;
    int   pos        = 0;
    int   count_symb_to_end = 0;
    int   length_arg = strlen(args);

    if ((sscanf(args, "[%1[abcd]x+%d]%n", &reg, &arg_const, &count_symb_to_end) == 2 ||
         sscanf(args, "[%d+%1[abcd]x]%n", &arg_const, &reg, &count_symb_to_end) == 2) &&
         count_symb_to_end == length_arg) {

        is_const = true;
        is_reg   = true;
        is_mem   = true;
    }
    else if (sscanf(args, " [%1[abcd]x]%n", &reg, &count_symb_to_end) == 1 &&
             count_symb_to_end == length_arg) {

        is_reg = true;
        is_mem = true;
    }
    else if (sscanf(args, " [%f]%n", &arg_const, &count_symb_to_end) == 1 &&
             sscanf(args, " [%d]%n", &tmp_const, &count_symb_to_end) == 1 &&
             arg_const == tmp_const && count_symb_to_end == length_arg) {

        is_const = true;
        is_mem   = true;
    }
    else if ((sscanf(args, " %1[abcd]x+%f%n", &reg, &arg_const, &count_symb_to_end) == 2  ||
              sscanf(args, " %f+%1[abcd]x%n", &arg_const, &reg, &count_symb_to_end) == 2) &&
              !is_leftside_arg && count_symb_to_end == length_arg) {

        is_const = true;
        is_reg   = true;
    }
    else if (sscanf(args, " %1[abcd]x%n", &reg, &count_symb_to_end) == 1 &&
             count_symb_to_end == length_arg) {

        is_reg = true;
    }
    else if (sscanf(args, " %f%n", &arg_const, &count_symb_to_end) == 1 && !is_leftside_arg && count_symb_to_end == length_arg) {
        is_const = true;
    }
    else if (sscanf(args, " %s", arg_label) == 1 &&
             arg_label[0] != '[' && arg_label[strlen(arg_label) - 1] != ']' && !is_leftside_arg) {

        pos = FindPosLabel(arg_label, *count_labels, labels_arr);
        if (pos != -1) {
            (*pointer_to_command) += (char)(CONST_ARG_CMD);
            *((int*)(commands_arr + *ip)) = pos;
            (*ip) += sizeof(arg_const);
        }
        else if (step == 1) {
            strcpy(labels_arr[*count_labels].name_label, arg_label);
            labels_arr[*count_labels].pos = -1;
            ++(*count_labels);
            (*pointer_to_command) += (char)(CONST_ARG_CMD);
            *((int*)(commands_arr + *ip)) = 0;
            (*ip) += sizeof(arg_const);
        }
        else {
            KillAsm("Failed: Cannot find label", TypeLog::ERROR_, LOCATION__(programm_code_text));
        }

        return 0;
    }
    else {
        KillAsm("Failed: Cannot find argument to commands", TypeLog::ERROR_, LOCATION__(programm_code_text));
    }

    if (is_const) (*pointer_to_command) += (char)(CONST_ARG_CMD);
    if (is_reg)   (*pointer_to_command) += (char)(REG_ARG_CMD);
    if (is_mem)   (*pointer_to_command) += (char)(RAM_ARG_CMD);

    if (is_reg) {
        commands_arr[(*ip)++] = reg[0];
    }
    if (is_const) {
        *((int*)(commands_arr + (*ip))) = (int)(arg_const * PRECISION);
        (*ip) += sizeof(arg_const);
    }

    return 0;
}

int FindPosLabel(const char* label, size_t count_labels, LABEL_* labels_arr) {
    if (_IsBadReadPtr(labels_arr)) {
        KillAsm("Failed: Bad ptr", TypeLog::ERROR_, LOCATION__(labels_arr));
    }

    for (size_t cur_label = 0; cur_label < count_labels; ++cur_label) {
        if (strcmp(labels_arr[cur_label].name_label, label) == 0) {
            return labels_arr[cur_label].pos;
        }
    }
    return -1;
}

int SetPosLabel(const char* label, size_t ip, size_t* count_labels, LABEL_* labels_arr) {
    if (_IsBadReadPtr(count_labels)) {
        KillAsm("Failed: Bad ptr", TypeLog::ERROR_, LOCATION__(count_labels));
    }
    if (_IsBadReadPtr(labels_arr)) {
        KillAsm("Failed: Bad ptr", TypeLog::ERROR_, LOCATION__(labels_arr));
    }

    for (size_t cur_label = 0; cur_label < *count_labels; ++cur_label) {
        if (strcmp(labels_arr[cur_label].name_label, label) == 0) {
            if (labels_arr[cur_label].pos != -1) {
                KillAsm("Failed: Repeat label", TypeLog::ERROR_, LOCATION__(programm_code_text));
            }
            labels_arr[cur_label].pos = ip;
            return 0;
        }
    }
    strcpy(labels_arr[*count_labels].name_label, label);
    labels_arr[*count_labels].pos = ip;
    ++(*count_labels);

    return 0;
}

#define DEF_CMD(cmd_name, num_args, is_leftside_arg, ...)                                                                 \
    else if (strcmp(cmd, #cmd_name) == 0) {                                                                               \
        pointer_to_command = ParseCommand((int)(PROCESSOR_COMMANDS::CMD_##cmd_name), commands_arr, &ip, &is_was_hlt);     \
        if (num_args > 0) {                                                                                               \
            ParseArgument(commands_arr, &ip, pointer_to_command, args, is_leftside_arg, count_labels, labels_arr, step);  \
        }                                                                                                                 \
    }                                    

int ChangeWordsToCodes(TextStruct* programm_code_text, const char* filename_assembler_text, size_t* count_labels, LABEL_* labels_arr, int step) {
    if (_IsBadReadPtr(programm_code_text)) {
        KillAsm("Failed: Bad ptr", TypeLog::ERROR_, LOCATION__(programm_code_text));
    }
    if (_IsBadReadPtr(count_labels)) {
        KillAsm("Failed: Bad ptr", TypeLog::ERROR_, LOCATION__(count_labels));
    }
    if (_IsBadReadPtr(labels_arr)) {
        KillAsm("Failed: Bad ptr", TypeLog::ERROR_, LOCATION__(labels_arr));
    }

    char    commands_arr[MAX_CODE_LENGTH] = { 0 };
    char    cmd[MAX_COMMAND_LENGTH]       = { 0 };
    char    args[MAX_COMMAND_LENGTH]      = { 0 };
    size_t  symb_to_end_cmd               = 0;
    size_t  ip                            = 0;
    char*   pointer_to_command            = commands_arr;
    bool    is_was_hlt                    = false;

    size_t num_strings_in_code   = programm_code_text->num_strings;
    for (size_t cur_line = 0; cur_line < num_strings_in_code; ++cur_line) {
        int count_read_cmd = sscanf(programm_code_text->strings_text[cur_line].str, "%s%n", cmd, &symb_to_end_cmd);
        int count_read_arg = sscanf(programm_code_text->strings_text[cur_line].str + symb_to_end_cmd, " %s", args);

        if (1 == 0) {

        }

        #include "../constants/cmd_def.h"

        else if (cmd[strlen(cmd) - 1] == ':' && strlen(cmd) >= 2) {
            if (step == 1) {
                cmd[strlen(cmd) - 1] = '\0';
                SetPosLabel(cmd, ip, count_labels, labels_arr);
            }
        }
        else {
            KillAsm("Failed: Cant recognize symbol", TypeLog::ERROR_, LOCATION__(labels_arr));
        }
    }

    if (!is_was_hlt) {
        KillAsm("Failed: Cant find end programm", TypeLog::ERROR_, LOCATION__(labels_arr));
    }

    if (step == 2) {
        WriteAssemblerToFile(commands_arr, ip, filename_assembler_text);
    }

    return 0;
}

#undef DEF_CMD

int WriteAssemblerToFile(char* code_arr, size_t size, const char* filename_assembler_text) {
    if (_IsBadReadPtr(code_arr)) {
        KillAsm("Failed: Cant open file to write binary code programm", TypeLog::ERROR_, LOCATION__(code_arr));
    }

    FILE* file_assembler_text = fopen(filename_assembler_text, "wb");
    if (_IsBadReadPtr(file_assembler_text)) {
        KillAsm("Failed: Cant open file to write binary code programm", TypeLog::ERROR_, LOCATION__(file_assembler_text));
    }

    HEADER_ASM_FILE header = {};
    HEADER_INIT(header, size);

    fwrite(&header, sizeof(header), 1, file_assembler_text);

    fwrite(code_arr, sizeof(char), size, file_assembler_text);

    fclose(file_assembler_text);

    return 0;
}


int KillAsm(const char* text_err, TypeLog type_log, const char* filename, size_t num_str) {
    CreateLog(text_err, type_log, filename, num_str);
    return -1;
}
