#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <string_view>

namespace bittorrent {
class Bencode {
public:
    static nlohmann::json decode(const std::string_view& inData);
};
} // namespace bittorrent