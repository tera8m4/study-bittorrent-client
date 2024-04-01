#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <string_view>

namespace bittorrent {
class Bencode {
public:
    using data_t = nlohmann::json;
    static data_t decode(const std::string_view& inData);
    static std::string encode(const data_t& inObject);
};
} // namespace bittorrent