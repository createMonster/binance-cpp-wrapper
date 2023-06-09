#include "binacpp_utils.h"

using std::toupper;

void split_string(string &s, char delim, vector<string> &result) {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        result.push_back(item);
    }
}

int replace_string_once(string &str, const char *from, const char *to, int offset) {
    size_t start_pos = str.find(from, offset);
    if (start_pos == std::string::npos) {
        return 0;
    }
    str.replace(start_pos, strlen(from), to);
    return start_pos + strlen(to);
}


void string_toupper(string &src) {
    for (int i=0; i < src.size(); i ++){
        src[i] = toupper(src[i]);
    }
}
