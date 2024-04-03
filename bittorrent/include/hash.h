#pragma once

#include <array>
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

private:
    std::array<uint8_t, N> data;
};
} // namespace bittorrent::hash
