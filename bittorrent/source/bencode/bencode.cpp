#include "bencode/bencode.h"
#include <string>
#include <string_view>

namespace bittorrent {
nlohmann::json Bencode::decode(const std::string_view& inData)
{
    return nlohmann::json(std::string { inData });
}
} // namespace bittorrent
