#include "bencode/bencode.h"
#include <string>
#include <string_view>

#include "bencode/bencode.h"

#include <cctype>
#include <charconv>
#include <cstddef>
#include <stdexcept>
#include <string_view>

#include "nlohmann/json_fwd.hpp"

using namespace bittorrent;

namespace {
Bencode::data_t decodeHelper(std::string_view inData, std::size_t& outBytesRead);

int decodeNumber(const std::string_view& inData)
{
    int num;
    auto result = std::from_chars(inData.data(), inData.data() + inData.size(), num);
    if (result.ec == std::errc::invalid_argument) {
        throw std::runtime_error("failed to parse number");
    }
    return num;
}

Bencode::data_t decodeString(std::string_view inData, std::size_t& outBytesRead)
{
    const std::size_t numberEnd = inData.find_first_of(':');
    if (numberEnd == std::string_view::npos) {
        throw std::runtime_error("failed to parse string");
    }

    const int stringSize = decodeNumber(inData.substr(0, numberEnd));
    outBytesRead = numberEnd + stringSize + 1;

    return nlohmann::json(std::string { inData.substr(numberEnd + 1, numberEnd + stringSize - 1) });
}

Bencode::data_t decodeInteger(std::string_view inData, std::size_t& outBytesRead)
{
    std::size_t endPos = inData.find_first_of('e');
    if (endPos == std::string_view::npos) {
        throw std::runtime_error("invalid number format");
    }
    const int decodedNumber = decodeNumber(inData.substr(1, endPos));
    outBytesRead = endPos;
    return nlohmann::json(decodedNumber);
}

Bencode::data_t decodeArray(std::string_view inData, std::size_t& outBytesRead)
{
    std::size_t bytesRead = 1;
    nlohmann::json json = nlohmann::json::array();

    while (bytesRead < inData.size() && inData[bytesRead] != 'e') {
        std::size_t itemRead = 0;
        json.emplace_back(decodeHelper(inData.substr(bytesRead, inData.size() - 1), itemRead));

        bytesRead += itemRead;
    }

    outBytesRead = bytesRead;
    return json;
}

Bencode::data_t decodeDictionary(std::string_view inData, std::size_t& outBytesRead)
{
    std::size_t bytesRead = 1;
    nlohmann::json json = nlohmann::json::object();

    while (bytesRead < inData.size() && inData[bytesRead] != 'e') {
        std::size_t itemRead = 0;
        const auto& key = decodeHelper(inData.substr(bytesRead), itemRead);
        bytesRead += itemRead;
        const auto& value = decodeHelper(inData.substr(bytesRead), itemRead);
        bytesRead += itemRead;

        json.emplace(key, value);
    }

    outBytesRead = bytesRead;
    return json;
}

Bencode::data_t decodeHelper(std::string_view inData, std::size_t& outBytesRead)
{
    if (std::isdigit(inData[0])) {
        return decodeString(inData, outBytesRead);
    } else if (inData[0] == 'i') {
        return decodeInteger(inData, outBytesRead);
    } else if (inData[0] == 'l') {
        return decodeArray(inData, outBytesRead);
    } else if (inData[0] == 'd') {
        return decodeDictionary(inData, outBytesRead);
    }
    throw std::runtime_error("undefined data type");
}
} // namespace

Bencode::data_t Bencode::decode(const std::string_view& inData)
{
    std::size_t tmp = 0;
    return decodeHelper(inData, tmp);
}