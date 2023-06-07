#include <iostream>

using std::toupper;

void string_toupper(string &src) {
    for (int i=0; i < src.size(); i ++){
        src[i] = toupper(src[i]);
    }
}
