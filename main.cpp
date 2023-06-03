#include <vector>
#include <iostream>
#include <curl/curl.h>

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

class BinanceAPIWrapper {
public:
    BinanceAPIWrapper(const std::string& apiKey, const std::string& secretKey)
        : apiKey(apiKey), secretKey(secretKey) {}

    std::string getTickerPrice(const std::string& symbol) {
        std::string url = "https://api.binance.com/api/v3/ticker/price?symbol=" + symbol;
        std::string response;

        CURL* curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

            CURLcode res = curl_easy_perform(curl);
            if (res == CURLE_OK) {
                // Request succeeded, return the response
                curl_easy_cleanup(curl);
                return response;
            } else {
                // Request failed, handle error
                std::cout << "Request failed: " << curl_easy_strerror(res) << std::endl;
            }
        }

        curl_easy_cleanup(curl);
        return "";
    }

private:
    std::string apiKey;
    std::string secretKey;
};

int main() {
    BinanceAPIWrapper binance("YOUR_API_KEY", "YOUR_SECRET_KEY");
    std::string tickerPrice = binance.getTickerPrice("BTCUSDT");
    std::cout << "BTCUSDT Price: " << tickerPrice << std::endl;

    return 0;
}