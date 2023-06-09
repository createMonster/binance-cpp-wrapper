#include "binacpp_logger.h"

int    BinaCPP_logger::debug_level   = 1;
string BinaCPP_logger::debug_log_file = "/tmp/binawatch.log";
int    BinaCPP_logger::debug_log_file_enable = 0;
FILE  *BinaCPP_logger::log_fp = NULL;

void
BinaCPP_logger::write_log(const char *fmt, ...) {
    if (debug_level == 0) {
        return;
    }
    if (debug_log_file_enable == 1) {
        open_logfp_if_not_opened();
    }

    va_list arg;
    
    char new_fmt[1024];
    struct timeval tv;
    gettimeofday(&tv, NULL);
}