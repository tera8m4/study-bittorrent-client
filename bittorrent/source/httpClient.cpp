#include "httpClient.h"
#include <cpr/cpr.h>

std::string bittorrent::HTTPClient::get(const std::string& url, const std::unordered_map<std::string, std::string>& params)
{
    cpr::Parameters cprParams;
    for (const auto& pair : params) {
        cprParams.Add({ pair.first, pair.second });
    }
    cpr::Response r = cpr::Get(cpr::Url { url }, cprParams);
    return r.text;
}
