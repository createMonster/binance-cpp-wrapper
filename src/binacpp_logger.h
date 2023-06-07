
#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <fstream>

using std::string;

class BinaCPP_logger {
    static int debug_level;
    static string debug_log_file;
    static int ;
    static FILE *log_fp;

    static void open_logfp_if_not_opened();

    public:
        static void write_log(const char *fmt, ...);
        static void write_log_clean(const char *fmt, ...);
        static void set_debug_level(int level);
        static void set_debug_logfile(string &pDebug_log_file);
        static void enable_logfile(int pDebug_log_file_enable);
}