#include "assembler.h"

static void KillAsm();
static void InputTextCode(TextStruct* programm_code_text, FILE* file_code_text);
static void TextSeparate(TextStruct* text/*!< - Pointer for text struct */);
static void SeparateTextByEndOfLine(TextStruct* text);
static void ChangeWordsToCodes(TextStruct* programm_code_text, const char* filename_assembler_text, size_t* count_labels, LABEL_* labels_arr, int step);
static int  FindPosLabel(const char* label, size_t count_lasels, LABEL_* labels_arr);
static void SetPosLabel(const char* label, size_t ip, size_t* count_labels, LABEL_* labels_arr);
static void WriteAssemblerToFile(char* code_arr, size_t size, const char* filename_assembler_text);


void CreateAssemblerFile(const char* filename_code, const char* filename_output) {
    FILE* file_input = fopen(filename_code, "r");
    if (file_input == nullptr) {
        CreateLog("Cant open file with code programm", TypeLog::ERROR_);
        KillAsm();
    }

    TextStruct code_programm = {};
    TextConstructor(&code_programm);

    GetMemoryForTextBuf(&code_programm, file_input);

    InputTextCode(&code_programm, file_input);

    LABEL_ labels_arr[MAX_LABELS_COUNT] = { 0 };
    size_t count_labels = 0;

    ChangeWordsToCodes(&code_programm, filename_output, &count_labels, labels_arr, 1);
    ChangeWordsToCodes(&code_programm, filename_output, &count_labels, labels_arr, 2);

    fclose(file_input);

    TextDestructor(&code_programm);
}

static void InputTextCode(TextStruct* code_programm, FILE* file_input) {
    if (_IsBadReadPtr(code_programm)) {
        CreateLog("Bad ptr", TypeLog::ERROR_);
        KillAsm();
    }

    fread(code_programm->text.str, sizeof(char), getFileSize(file_input) + 1, file_input);
    code_programm->text.lenght = strlen(code_programm->text.str);

    TextSeparate(code_programm);

    return;
}

static void TextSeparate(TextStruct* text/*!< - Pointer for text struct */) {
    if (_IsBadReadPtr(text)) {
        CreateLog("Bad ptr", TypeLog::ERROR_);
        KillAsm();
    }

    text->num_strings = CountNumOfStrings(&text->text);

    text->strings_text = (StringStruct*)calloc(text->num_strings + 1, sizeof(StringStruct));
    if (text->strings_text == nullptr) {
        CreateLog("Cant calloc memory to separate code by strings", TypeLog::ERROR_);
        KillAsm();
    }

    SeparateTextByEndOfLine(text);

    return;
}

static void SeparateTextByEndOfLine(TextStruct* text) {
    if (_IsBadReadPtr(text)) {
        CreateLog("Bad ptr", TypeLog::ERROR_);
        KillAsm();
    }

    size_t cur_size_str = text->text.lenght;
    size_t count_symb_in_cur_line = 0;
    size_t n_line = 0;
    for (size_t num_symb = 0; num_symb < cur_size_str + 1; ++num_symb) {
        if (text->text.str[num_symb] == '\n' || text->text.str[num_symb] == '\r' || text->text.str[num_symb] == '\0') {
            text->text.str[num_symb] = '\0';

            text->strings_text[n_line].str = (text->text.str + num_symb - count_symb_in_cur_line);

            if (strlen(text->strings_text[n_line].str) == 0) {
                count_symb_in_cur_line = 0;
                --text->num_strings;
                continue;
            }
            //EraseSpaceStartEndStr(&(text->strings_text[n_line]));

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

    return;
}

bool ParseCommand (int num_cmd, char* commands_arr, size_t* ip, char* full_cmd, char* cmd, int num_args, int required, int is_leftside_arg, int symb_from_start_cmd, 
                   size_t* count_labels, LABEL_* labels_arr, int step)  {
    bool is_was_hlt = false;
    if (required == 1) {                                                                                                                                                  
            is_was_hlt = true;                                                                                                                                                
    }

    bool  is_arg_const  = false;
    bool  is_reg        = false;
    float arg_const     = 0;
    int   tmp_arg_const = 0;
    int   cmd_int       = 0; 
    char  reg[2]        = { 0 };
    char  arg_label[MAX_LABEL_LENGTH] = { 0 };
    int   pos           = 0;
    if (num_args > 0) {
        if (sscanf(full_cmd + symb_from_start_cmd, " [%1[abcd]x+%d]", reg, &arg_const) == 2 ||
            sscanf(full_cmd + symb_from_start_cmd, " [%d+%1[abcd]x]", &arg_const, reg) == 2) {
                cmd_int = num_cmd + RAM_ARG_CMD + REG_ARG_CMD + CONST_ARG_CMD;                                                                  
                is_arg_const = true;                                                                                                                                          
                is_reg = true;                                                                                                                                                
        }
        else if (sscanf(full_cmd + symb_from_start_cmd, " [%1[abcd]x]", reg) == 1) {
                cmd_int = num_cmd + RAM_ARG_CMD + REG_ARG_CMD;                                                                                  
                is_reg = true;                                                                                                                                                
        }
        else if (sscanf(full_cmd + symb_from_start_cmd, " [%f]", &arg_const)     == 1 &&
                     sscanf(full_cmd + symb_from_start_cmd, " [%d]", &tmp_arg_const) == 1 && arg_const == tmp_arg_const) {
                                                                                                                                                                              
                arg_const = tmp_arg_const;                                                                                                                                    
                cmd_int = num_cmd + RAM_ARG_CMD + CONST_ARG_CMD;                                                                                
                is_arg_const = true;                                                                                                                                          
        }
        else if ((sscanf(full_cmd + symb_from_start_cmd, " %1[abcd]x+%f", reg, &arg_const) == 2 || 
                  sscanf(full_cmd + symb_from_start_cmd, " %f+%1[abcd]x", &arg_const, reg) == 2) && is_leftside_arg == 1) {
                cmd_int = num_cmd + REG_ARG_CMD + CONST_ARG_CMD;                                                                                
                is_arg_const = true;                                                                                                                                          
                is_reg = true;                                                                                                                                                
        }
        else if (sscanf(full_cmd + symb_from_start_cmd, " %1[abcd]x", reg) == 1) {
                cmd_int = num_cmd + REG_ARG_CMD;                                                                                                
                is_reg = true;                                                                                                                                                
        }                                                                                                                                                                 
        else if (sscanf(full_cmd + symb_from_start_cmd, " %f", &arg_const) == 1 && is_leftside_arg == 1) {
            cmd_int = num_cmd + CONST_ARG_CMD;                                                                                              
            is_arg_const = true;                                                                                                                                          
        }
        else if (sscanf(full_cmd + symb_from_start_cmd, " %s", arg_label) == 1 &&
                 arg_label[0] != '[' && arg_label[strlen(arg_label) - 1] != ']' && is_leftside_arg == 1) { 

            pos = FindPosLabel(arg_label, *count_labels, labels_arr);                                                                                                     
            if (pos != -1) {                                                                                                                                              
                commands_arr[(*ip)++] = (char)(num_cmd + CONST_ARG_CMD);                                                                       
                *((int*)(commands_arr + *ip)) = pos;                                                                                                                      
                (*ip) += sizeof(int);                                                                                                                                        
            }                                                                                                                                                             
            else if (step == 1) {                                                                                                                                         
                strcpy(labels_arr[*count_labels].name_label, arg_label);                                                                                                  
                labels_arr[*count_labels].pos = -1;                                                                                                                       
                ++(*count_labels);                                                                                                                                        
                commands_arr[(*ip)++] = (char)(num_cmd + CONST_ARG_CMD);                                                                       
                *((int*)(commands_arr + *ip)) = 0;                                                                                                                        
                (*ip) += sizeof(int);                                                                                                                                        
            } else {                                                                                                                                                        
                    CreateLog("Cant find label", TypeLog::ERROR_);                                                                                                            
                    KillAsm();                                                                                                                                                
            }                                                                                                                                                             

            return is_was_hlt;                                                                                                                                                     
        }                                                                                                                                                                                                                                                                                                                                     
        else {                                                                                                                                                                
            CreateLog("Cant find argument", TypeLog::ERROR_);                                                                                                             
            KillAsm();                                                                                                                                                    
        }
    } else {                                                                                                                                                               
        cmd_int = num_cmd;     
    }

    commands_arr[(*ip)++] = (char)cmd_int;

    if (is_reg) {                                                                                                                                                         
        commands_arr[(*ip)++] = reg[0];                                                                                                                                      
    }      

    if (is_arg_const) {                                                                                                                                                   
        *((int*)(commands_arr + (*ip))) = (int)(arg_const * PRECISION);                                                                                                      
        (*ip) += sizeof(int);                                                                                                                                                
    }

    return is_was_hlt;
}                                                                                                                                                                
      
#define DEF_CMD(cmd_in, num_args, is_leftside_arg, required)         \
    else if (strcmp(cmd, #cmd_in) == 0) {                            \
        is_was_hlt |= ParseCommand((int)ASSEMBLER_COMMANDS::CMD_##cmd_in, commands_arr, &ip, programm_code_text->strings_text[cur_line].str, cmd, num_args, required, is_leftside_arg,   \
                     symb_to_end_cmd, count_labels, labels_arr, step);                                                               \
    }   

static void ChangeWordsToCodes(TextStruct* programm_code_text, const char* filename_assembler_text, size_t* count_labels, LABEL_* labels_arr, int step) {
    if (_IsBadReadPtr(programm_code_text)) {
        CreateLog("Bad ptr", TypeLog::ERROR_);
        KillAsm();
    }

    char commands_arr[MAX_CODE_LENGTH] = { 0 };
    char cmd[MAX_COMMAND_LENGTH]       = { 0 };
    char args[MAX_COMMAND_LENGTH]      = { 0 };

    size_t ip              = 0;
    size_t symb_to_end_cmd = 0;
    bool is_was_hlt        = false;

    for (size_t cur_line = 0; cur_line < programm_code_text->num_strings; ++cur_line) {
        int   count_read_cmd = sscanf(programm_code_text->strings_text[cur_line].str, "%s%n", cmd, &symb_to_end_cmd);

        if (1 == 0) {
        }

        #include "../commands/def/cmd_def.h"

        else if (cmd[strlen(cmd) - 1] == ':' && strlen(cmd) >= 2) {
            if (step == 1) {
                cmd[strlen(cmd) - 1] = '\0';
                SetPosLabel(cmd, ip, count_labels, labels_arr);
            }
        }
        else {
            CreateLog("Cant recognize symb", TypeLog::ERROR_);                                                                                                        
            KillAsm();
        }
    }

    if (is_was_hlt == false) {
        CreateLog("Cant find command ", TypeLog::ERROR_);
        KillAsm();
    }

    if (step == 2) {
        WriteAssemblerToFile(commands_arr, ip, filename_assembler_text);
    }
}

#undef DEF_CMD

static int FindPosLabel(const char* label, size_t count_labels, LABEL_* labels_arr) {
    for (size_t cur_label = 0; cur_label < count_labels; ++cur_label) {
        if (strcmp(labels_arr[cur_label].name_label, label) == 0) {
            return labels_arr[cur_label].pos;
        }
    }
    return -1;
}

static void SetPosLabel(const char* label, size_t ip, size_t* count_labels, LABEL_* labels_arr) {
    for (size_t cur_label = 0; cur_label < *count_labels; ++cur_label) {
        if (strcmp(labels_arr[cur_label].name_label, label) == 0) {
            if (labels_arr[cur_label].pos != -1) {
                CreateLog("Repeat label", TypeLog::ERROR_);
                KillAsm();
            }
            labels_arr[cur_label].pos = ip;
            return;
        }
    }
    strcpy(labels_arr[*count_labels].name_label, label);
    labels_arr[*count_labels].pos = ip;                                                                                                                       
    ++(*count_labels);
}

static void WriteAssemblerToFile(char* code_arr, size_t size, const char* filename_assembler_text) {
    FILE* file_assembler_text = fopen(filename_assembler_text, "wb");
    if (file_assembler_text == nullptr) {
        CreateLog("Cant open file to write binary code programm", TypeLog::ERROR_);
        KillAsm();
    }

    HEADER_ASM_FILE header = {};
    HEADER_INIT(header, size);

    fwrite(&header, sizeof(header), 1, file_assembler_text);

    fwrite(code_arr, sizeof(char), size, file_assembler_text);

    fclose(file_assembler_text);

    return;
}

void KillAsm() {
    CreateLog("Program has criticaly finished", TypeLog::ERROR_);
    abort();
}