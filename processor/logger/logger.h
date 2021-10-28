#ifndef LOGGER_H__
#define LOGGER_H__

/*--------------------------------------------------------------------------*/

#include <stdio.h>

/*--------------------------------------------------------------------------*/

enum class TypeLog {
    INFO_,
    WARNING_,
    ERROR_,
    CRITICAL_ERROR_
};

static const char* STR_TYPE_LOG[4] = { "INFO: ", "WARNING", "ERROR", "CRIT_ERROR" };

static const char* LOG_FILE_NAME   = "log.txt";

int CreateLog(const char* text_log, TypeLog type_log, const char* filename, int num_line);

#endif
