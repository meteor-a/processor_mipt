#include "logger.h"

int CreateLog(const char* text_log, TypeLog type_log, const char* filename, int num_line) {
    printf(         "%s %s %s %s %s %d\n", STR_TYPE_LOG[(int)type_log], text_log, "FILENAME: ", filename, "LINE: ", num_line);
    fprintf(stderr, "%s %s %s %s %s %d\n", STR_TYPE_LOG[(int)type_log], text_log, "FILENAME: ", filename, "LINE: ", num_line);

    FILE* file_log = fopen(LOG_FILE_NAME, "r");
    if (file_log == nullptr) {
        return -1;
    }

    setvbuf(file_log, (char*)NULL, _IONBF, 0);
    fprintf(file_log, "%s %s %s %s %s %d\n", STR_TYPE_LOG[(int)type_log], text_log, "FILENAME: ", filename, "LINE: ", num_line);

    fclose(file_log);

    return 0;
}
