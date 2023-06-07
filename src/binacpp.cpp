#include "binacpp.h"
#include "binacpp_logger.h"

using std::atof;

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
void
BinaCPP::get_exchangeInfo(Json::Value &json_result) {
    BinaCPP_logger::write_log("<BinaCPP::get_exchangeInfo>");
    
    string url(BINANCE_HOST);
    url += "/api/v1/exchangeInfo";

    string str_result;
    curl_api(url, str_result);
    if (str_result.size() > 0) {
        try {
            Json::Reader reader;
            json_result.clear();
            reader.parse(str_result, json_result);
        }
        catch (exception &e){
            BinaCPP_logger::write_log("<BinaCPP::get_exchangeInfo> Error! %s", e.what());
        }
        BinaCPP_logger::write_log("<BinaCPP::get_exchangeInfo> Done.");
    }
    else {
        BinaCPP_logger::write_log("<BinaCPP::get_exchangeInfo> failed to get anything");
    }
}

void
BinaCPP::get_serverTime(Json::Value &json_result) {
    BinaCPP_logger::write_log("<BinaCPP::get_exchangeInfo>");
    
    string url(BINANCE_HOST);
    url += "/api/v1/time";
    string str_result;
    curl_api(url, str_result);

    if (str_result.size() > 0) {
        try {
            Json::Reader reader;
            json_result.clear();
            reader.parse(str_result, json_result);
        }
        catch (exception &e) {
            BinaCPP_logger::write_log("<BinaCPP::get_serverTime> Error! %s", e.what());
        }
        BinaCPP_logger::write_log("<BinaCPP::get_serverTime> Done");
    }
    else {
        BinaCPP_logger::write_log("<BinaCPP::get_serverTime> Failed to get anything");
    }
}

void
BinaCPP::get_allPrices(Json::Value json_result) {
    BinaCPP_logger::write_log("<BinaCPP::get_allPrices>");

    string url(BINANCE_HOST);
    url += "/api/v1/ticker/allPrices";
    string str_result;
    curl_api(url, str_result);

    if (str_result.size() > 0) {
        try {
            Json::Reader reader;
            json_result.clear();
            reader.parse(str_result, json_result);
        }
        catch (exception &e) {
            BinaCPP_logger::write_log("<BinaCPP::get_allPrices> Error! %s", e.what());
        }
        BinaCPP_logger::write_log("<BinaCPP::get_allPrices> Done");
    }
    BinaCPP_logger::write_log("<BinaCPP::get_allPrices> Failed to get anything.");
}

double
BinaCPP::get_price(const char *symbol) {
    BinaCPP_logger::write_log("<BinaCPP::get_price>");
    
    double ret = 0.0;
    Json::Value allTickers;
    string str_symbol = string_toupper(symbol);
    get_allPrices(allTickers);

    for (int i = 0; i < allTickers.size(); i++) {
        if (allTickers[i]["symbol"].asString() == str_symbol) {
            ret = atof(allTickers[i]["price"].asString().c_str());
            break;
        }
    }
    return ret;
}


void
BinaCPP::curl_api(string &url, string &result_json) {
    vector <string> v;
    string action = "GET";
    string post_data = "";
    curl_api_with_header(url, result_json, v, post_data, action);
}

void
BinaCPP::curl_api_with_header(string &url, string &result_json, vector<string> &extra_http_header, string &post_data, string &action) {
    BinaCPP_logger::write_log("<BinaCPP::curl_api>");

    CURLcode res;
    if (BinaCPP::curl) {
        curl_easy_setopt(BinaCPP::curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(BinaCPP::curl, CURLOPT_WRITEFUNCTION, BinaCPP::curl_cb);
        curl_easy_setopt(BinaCPP::curl, CURLOPT_WRITEDATA, &result_json);
        curl_easy_setopt(BinaCPP::curl, CURLOPT_SSL_VERIFYPEER, false);
        curl_easy_setopt(BinaCPP::curl, CURLOPT_ENCODING, "gzip");

        if (extra_http_header.size() > 0) {
            struct curl_slist *chunk = NULL;
            for (int i = 0; i < extra_http_header.size(); i++) {
                chunk = curl_slist_append(chunk, extra_http_header[i].c_str());
            }
            curl_easy_setopt(BinaCPP::curl, CURLOPT_HTTPHEADER, chunk);
        }

        if (post_data.size() > 0 || action == "POST" || action == "PUT" || action == "DELETE") {
            if (action == "DELETE" || action == "PUT") {
                curl_easy_setopt(BinaCPP::curl, CURLOPT_CUSTOMREQUEST, action.c_str());
            }
            curl_easy_setopt(BinaCPP::curl, CURLOPT_POSTFIELDS, post_data.c_str());
        }

        res = curl_easy_perform(BinaCPP:curl);
        
        //Check for errors
        if (res != CURLE_OK) {
            BinaCPP_logger::write_log(" <BinaCPP::curl_api> curl_easy_perform() failed: %s", curl_easy_strerror(res));
        }
    }
    BinaCPP_logger::write_log("<BinaCPP::curl_api> done");
}
