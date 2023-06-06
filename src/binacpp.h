#ifndef BINACPP.H
#define BINACPP.H

#include <iostream>
#include <string>
#include <vector>
#include <curl/curl.h>
#include <json/json.h>

#define BINANCE_HOST "https://api.binance.com"

using std::string;


class BinaCPP {
    static string api_key;
    static string secret_key;
    static CURL *curl;

    public:
        
        static void curl_api(string &url, string &result_json);
        static void curl_api_with_header(string &url, string &result_json, vector<string> &extra_http_header, string &post_data, string &action);
        static size_t curl_cb(void *content, size_t size, size_t nmemb, string *buffer);
        static void init(string &api_key, string&secret_key);
        static void cleanup();
        // Public API
        static void get_exchangeInfo(Json::Value &json_result);
        static void get_serverTime(Json::Value &json_result);
        
        static void get_allPrices(Json::Value &json_result);
        static double get_price(const char *symbol);


};

#endif