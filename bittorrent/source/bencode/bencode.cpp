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

long long stringViewToNumber(const std::string_view& inData)
{
    long long num;
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

    const int stringSize = stringViewToNumber(inData.substr(0, numberEnd));
    outBytesRead = numberEnd + stringSize + 1;
    const std::string_view& substr = inData.substr(numberEnd + 1, stringSize);
    return nlohmann::json(std::string { substr });
}

Bencode::data_t decodeInteger(std::string_view inData, std::size_t& outBytesRead)
{
    std::size_t endPos = inData.find_first_of('e');
    if (endPos == std::string_view::npos) {
        throw std::runtime_error("invalid number format");
    }
    const long long decodedNumber = stringViewToNumber(inData.substr(1, endPos));
    outBytesRead = endPos + 1;
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

    outBytesRead = bytesRead + 1;
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

void encodeHelper(const Bencode::data_t& inData, std::stringstream& outStringStream)
{
    if (inData.is_string()) {
        const std::string str = inData.get<std::string>();
        outStringStream << str.size() << ':' << str;
        return;
    } else if (inData.is_number()) {
        outStringStream << 'i' << inData.get<long long>() << 'e';
        return;
    } else if (inData.is_array()) {
        outStringStream << 'l';
        for (const auto& el : inData) {
            encodeHelper(el, outStringStream);
        }
        outStringStream << "e";
        return;
    } else if (inData.is_object()) {
        outStringStream << "d";
        for (const auto& pair : inData.items()) {
            encodeHelper(pair.key(), outStringStream);
            encodeHelper(pair.value(), outStringStream);
        }
        outStringStream << "e";
        return;
    }

    throw std::runtime_error("unsupported object type");
}

} // namespace

Bencode::data_t Bencode::decode(const std::string_view& inData)
{
    std::size_t tmp = 0;
    return decodeHelper(inData, tmp);
}

std::string Bencode::encode(const data_t& inObject)
{
    std::stringstream ss;
    encodeHelper(inObject, ss);

    return ss.str();
}