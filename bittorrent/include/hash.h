#pragma once

#include <array>
#include <cstdint>
#include <iomanip>
#include <sstream>

namespace bittorrent {

template <std::size_t N>
class HashView {
public:
    HashView()
    {
        data.fill(0);
    }

    uint8_t operator[](const std::size_t index) const { return data[index]; }
    uint8_t& operator[](const std::size_t index) { return data[index]; }
    std::size_t size() const { return N; }

    operator std::string() const
    {
        std::stringstream ss;
        for (const uint8_t el : data) {
            ss << std::hex << std::setfill('0') << std::setw(2) << (0xff & el);
        }

        return ss.str();
    }

    template <typename T>
    void insert(const std::size_t atIndex, const T& value)
    {
        const uint8_t ratio = sizeof(T) / sizeof(uint8_t);
        for (std::size_t i = atIndex; i < atIndex + ratio; ++i) {
            const uint8_t shift = (ratio - 1 - i) * 8;
            data[i] = (value >> shift) & 0xff;
        }
    }

    bool operator==(const std::string& inString) const
    {
        return static_cast<std::string>(*this) == inString;
    }

private:
    std::array<uint8_t, N> data;
};
} // namespace bittorrent::hash
