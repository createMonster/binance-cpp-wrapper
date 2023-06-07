#include "binacpp_logger.h"

int    BinaCPP_logger::debug_level   = 1;
string BinaCPP_logger::debug_log_file = "/tmp/binawatch.log";
int    BinaCPP_logger::debug_log_file_enable = 0;
FILE  *BinaCPP_logger::log_fp = NULL;

