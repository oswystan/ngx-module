/*
 **************************************************************************************
 *       Filename:  log.cc
 *    Description:   source file
 *
 *        Version:  1.0
 *        Created:  2018-04-27 17:31:30
 *
 *       Revision:  initial draft;
 **************************************************************************************
 */

#include <stdio.h>
#include <stdarg.h>

static FILE* log_fp = NULL;

int log_init(const char* logfile) {
    if (log_fp) {
        return 0;
    }
    log_fp = fopen(logfile, "a");
    if (log_fp) {
        return 0;
    } else {
        return -1;
    }
}

void write_log(const char* fmt, ...) {
    va_list args;
    char* log = NULL;
    va_start(args, fmt);
    vasprintf(&log, fmt, args);
    va_end(args);
    fprintf(log_fp, "%s", log);
    fflush(log_fp);
}


/********************************** END **********************************************/

