#include <binacpp.h>

string BinaCPP::api_key = "";
string BinaCPP::secret_key = "";
CURL *BinaCPP::curl = NULL;

void
BinaCPP::init(string &api_key, string &secret_key) {
    curl_global_init(CURL_GLOBAL_DEFAULT);
    BinaCPP::api_key = api_key;
    BinaCPP::secret_key = secret_key;
    BinaCPP::curl = curl_easy_init();
}

void
BinaCPP::cleanup() {
    curl_easy_cleanup(BinaCPP::curl);
    curl_global_cleanup();
}

// -----
// GET 
//