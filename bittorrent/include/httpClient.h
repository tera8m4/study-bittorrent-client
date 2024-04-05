#pragma once

#include <string>
#include <unordered_map>
namespace bittorrent {
class HTTPClient {
public:
    using Params = std::unordered_map<std::string, std::string>;
    std::string get(const std::string& url, const Params& params);
};
} // namespace bittorrent
