#include <map>
#include <vector>
#include <string>


#include "binacpp.h"
#include "binacpp_websocket.h"
#include <json/json.h>

#define API_KEY 		"api key"
#define SECRET_KEY		"user key"


// Some code to make terminal to have colors
#define KGRN "\033[0;32;32m"
#define KCYN "\033[0;36m"
#define KRED "\033[0;32;31m"
#define KYEL "\033[1;33m"
#define KBLU "\033[0;32;34m"
#define KCYN_L "\033[1;36m"
#define KBRN "\033[0;33m"
#define RESET "\033[0m"


using namespace std;

map < string, map <double,double> >  depthCache;
map < long,   map <string,double> >  klinesCache;
map < long,   map <string,double> >  aggTradeCache;
map < long,   map <string,double> >  userTradeCache;
map < string, map <string,double> >  userBalance;


int lastUpdateId;


int main() {
    string api_key = API_KEY;
    string secret_key = SECRET_KEY;
    BinaCPP::init(api_key, secret_key);

    Json::Value result;
    long recvWindow = 10000;

    BinaCPP::get_serverTime(result);
    cout << result << endl;

}