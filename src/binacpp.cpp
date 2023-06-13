#include "binacpp.h"
#include "binacpp_logger.h"
#include "binacpp_utils.h"

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
BinaCPP::get_depth(
        const char *symbol,
        int limit,
        Json::Value &json_result
) {
    BinaCPP_logger::write_log("<BinaCPP::get_depth>");
    string url(BINANCE_HOST);
    url += "/api/v1/depth?";
    
    string querystring("symbol=");
	querystring.append( symbol );
	querystring.append("&limit=");
	querystring.append( to_string( limit ) );

    url.append(querystring);
    BinaCPP_logger::write_log( "<BinaCPP::get_depth> url = |%s|" , url.c_str() ) ;
    
    string str_result;
    curl_api(url, str_result);
    
    if (str_result.size() > 0) {
        try {
            Json::Reader reader;
            json_result.clear()
            reader.parse(str_reslt, json_result);
        }
        catch (exception &e) {
            BinaCPP_logger::write_log("<BinaCPP::get_depth> Error! %s", e.what());
        }
        BinaCPP_logger::write_log("<BinaCPP::get_depth> Done.");
    }
    else {
        BinaCPP_logger::write_log("<BinaCPP::get_depth> Failed to get anything.");
    }
}

//--------------------
// Get Aggregated Trades list
/*

GET /api/v1/aggTrades

Name		Type	Mandatory	Description
symbol		STRING	YES	
fromId		LONG	NO		ID to get aggregate trades from INCLUSIVE.
startTime	LONG	NO		Timestamp in ms to get aggregate trades from INCLUSIVE.
endTime		LONG	NO		Timestamp in ms to get aggregate trades until INCLUSIVE.
limit		INT	NO		Default 500; max 500.
*/

void 
BinaCPP::get_aggTrades(
        const char *symbol,
        int fromId,
        time_t startTime,
        time_t endTime,
        int limit
) {
    BinaCPP_logger::write_log("<BinaCPP::get_aggTrades>");
    string url(BINANCE_HOST);
    url += "/api/v1/aggTrades?";

	string querystring("symbol=");
	querystring.append( symbol );
    
    if (startTime != 0 && endTime != 0) {
        querystring.append("&startTime=");
		querystring.append( to_string( startTime ) );

		querystring.append("&endTime=");
		querystring.append( to_string( endTime ) );
    }
    else {
        querystring.append("&fromId=");
		querystring.append( to_string( fromId ) );

		querystring.append("&limit=");
		querystring.append( to_string( limit ) );
    }
    url.append(querystring);
    BinaCPP_logger::write_log( "<BinaCPP::get_aggTrades> url = |%s|" , url.c_str() ) ;
    
    string str_result;
    curl_api(url, str_result);
    
    if (str_result.size() > 0) {
        try {
            Json::Reader reader;
            json_result.clear()
            reader.parse(str_reslt, json_result);
        }
        catch (exception &e) {
            BinaCPP_logger::write_log("<BinaCPP::get_aggTrades> Error! %s", e.what());
        }
        BinaCPP_logger::write_log("<BinaCPP::get_aggTrades> Done.");
    }
    else {
        BinaCPP_logger::write_log("<BinaCPP::get_aggTrades> Failed to get anything.");
    }
}

//--------------------
// Get 24hr ticker price change statistics
/*
Name	Type	Mandatory	Description
symbol	STRING	YES	
*/
void
BinaCPP::get_24hr(const char *symbol, Json::Value &json_result) {
    BinaCPP_logger.write_log("<BinaCPP::get_24hr>");
    string url(BINANCE_HOST);
    url += "/api/v1/ticker/24hr?";

	string querystring("symbol=");
	querystring.append( symbol );
    url.append(querystring);
    BinaCPP_logger.write_log("<BinaCPP::get_24hr> url = |%s|", url.c_str());
    
    string str_reuslt;
    curl_api(str_reuslt);
    if ( str_result.size() > 0 ) {
		
		try {
            Json::Reader reader;
            json_result.clear();	
            reader.parse( str_result , json_result );
	    		
		} catch ( exception &e ) {
		 	BinaCPP_logger::write_log( "<BinaCPP::get_24hr> Error ! %s", e.what() ); 
		}   
		BinaCPP_logger::write_log( "<BinaCPP::get_24hr> Done." ) ;
	
	} else {
		BinaCPP_logger::write_log( "<BinaCPP::get_24hr> Failed to get anything." ) ;
	}

}

//-----------------
/*

Get KLines( Candle stick / OHLC )
GET /api/v1/klines

Name		Type	Mandatory	Description
symbol		STRING	YES	
interval	ENUM	YES	
limit		INT		NO	Default 500; max 500.
startTime	LONG	NO	
endTime		LONG	NO	

*/

void
BinaCPP::get_klines(
        const char *symbol, 
        const char *interval,
        int limit,
        time_t startTime,
        time_t endTime
) {
    BinaCPP_logger::write_log("<BinaCPP::get_klines>");
    string url(BINANCE_HOST);
    url += "/api/v1/klines?";
    
    string querystring = "symbol=";
    querystring.append(symbol);
    querystring.append( "&interval=" );
	querystring.append( interval );

	if ( startTime > 0 && endTime > 0 ) {

		querystring.append("&startTime=");
		querystring.append( to_string( startTime ) );

		querystring.append("&endTime=");
		querystring.append( to_string( endTime ) );
	
	} else if ( limit > 0 ) {
		querystring.append("&limit=");
		querystring.append( to_string( limit ) );
	}

    url.append(querystring);
    BinaCPP_logger::write_log( "<BinaCPP::get_klines> url = |%s|" , url.c_str() ) ;
	
	string str_result;
	curl_api( url, str_result ) ;

	if ( str_result.size() > 0 ) {
		
		try {
			Json::Reader reader;
	    	json_result.clear();	
			reader.parse( str_result , json_result );
	    		
		} catch ( exception &e ) {
		 	BinaCPP_logger::write_log( "<BinaCPP::get_klines> Error ! %s", e.what() ); 
		}   
		BinaCPP_logger::write_log( "<BinaCPP::get_klines> Done." ) ;
	
	} 
    else {
		BinaCPP_logger::write_log( "<BinaCPP::get_klines> Failed to get anything." ) ;
	}
}

//--------------------
// Get current account information. (SIGNED)
/*
GET /api/v3/account

Parameters:
Name		Type	Mandatory	Description
recvWindow	LONG	NO	
timestamp	LONG	YES
*/
void
BinaCPP::get_account(long recvWindow, Json::Value &json_result) {
    
    BinaCPP_logger.write_log("<BinaCPP::get_account>");
    if (api_key.size() == 0 || secret_key.size() == 0) {
        BinaCPP_logger.write_log("<BinaCPP::get_account> API Key and Secret Key has not been set.");
        return;
    }
    string url(BINANCE_HOST);
	url += "/api/v3/account?";
	string action = "GET";

    string querystring("timestamp=");
    querystring.append(to_string(get_current_ms_epoch()));

    if (recvWindow > 0) {
        querystring.append("&recvWindow=");
        querystring.append(to_string(recvWindow));
    }
    
    string signature = hmac_sha256(secret_key.c_str(), api_key.cstr());
    querystring.append("&signature=");
    querystring.append(signature);
    url.append(querystring);

    vector<string> extra_http_header;
    string header_chunk("X-MBX-APIKEY: ");
    header_chunk.append(api_key);
    extra_http_header.push_back(header_chunk);

    BinaCPP_logger::write_log( "<BinaCPP::get_account> url = |%s|" , url.c_str() ) ;
    
    string post_data = "";
    string str_result;
    curl_api_with_header(url, str_result, extra_http_header, post_data, action);
    
    if (str_result.size() > 0) {
        try {
            Json::Reader reader;
            json_result.clear();
            reader.parse(str_result, json_result);
        }
        catch (exception &e) {
            BinaCPP_logger::write_log( "<BinaCPP::get_account> Error ! %s", e.what() ); 
        }
        BinaCPP_logger::write_log( "<BinaCPP::get_account> Done.\n" ) ;
    }
    else {
        BinaCPP_logger::write_log( "<BinaCPP::get_account> Failed to get anything." ) ;
    }
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
