#include "logger.h"

void CreateLog(const char* text_log, TypeLog type_log) {
    FILE* file_log = fopen(LOG_FILE_NAME, "r"); 
    if (file_log == nullptr) {
        printf("%s %s\n", STR_TYPE_LOG[(int)type_log], text_log);
        return;
    }   
    setvbuf(file_log, (char*)NULL, _IONBF, 0);

    printf("%s %s\n", STR_TYPE_LOG[(int)type_log], text_log);
    fprintf(file_log, "%s %s\n", STR_TYPE_LOG[(int)type_log], text_log);

    fclose(file_log);

    return;
}
