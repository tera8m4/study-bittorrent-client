#include "utils.h"
#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

std::string bittorrent::utils::readFile(const std::filesystem::path& inPath)
{
    namespace fs = std::filesystem;
    // Open the stream to 'lock' the file.
    std::ifstream f(inPath, std::ios::in | std::ios::binary);

    // Obtain the size of the file.
    const auto sz = fs::file_size(inPath);

    // Create a buffer.
    std::string result(sz, '\0');

    // Read the whole file into the buffer.
    f.read(result.data(), sz);

    return result;
}

std::string bittorrent::utils::sha1(const std::string& inData)
{
    const uint64_t bitLength = inData.size() * 8;

    uint32_t h0 = 0x67452301;
    uint32_t h1 = 0xEFCDAB89;
    uint32_t h2 = 0x98BADCFE;
    uint32_t h3 = 0x10325476;
    uint32_t h4 = 0xC3D2E1F0;

    std::vector<uint8_t> data(inData.begin(), inData.end());
    data.push_back(0x80);

    while (data.size() % 64 != 56) {
        data.push_back(0);
    }

    for (int i = 7; i >= 0; --i) {
        data.push_back(bitLength >> (i * 8));
    }

    std::array<uint32_t, 80> w;
    for (std::size_t i = 0; i < data.size(); i += 64) {
        for (std::size_t j = 0; j < 16; ++j) {
            w[j] = (data[i + j * 4] << 24) | (data[i + j * 4 + 1] << 16) | (data[i + j * 4 + 2] << 8) | data[i + j * 4 + 3];
        }
        for (std::size_t j = 16; j < w.size(); ++j) {
            w[j] = (w[j - 3] ^ w[j - 8] ^ w[j - 14] ^ w[j - 16]);
            w[j] = (w[j] << 1) | (w[j] >> 31);
        }

        uint32_t a = h0;
        uint32_t b = h1;
        uint32_t c = h2;
        uint32_t d = h3;
        uint32_t e = h4;

        for (int j = 0; j < 80; ++j) {
            uint32_t f, k;

            if (j < 20) {
                f = (b & c) | ((~b) & d);
                k = 0x5A827999;
            } else if (j < 40) {
                f = b ^ c ^ d;
                k = 0x6ED9EBA1;
            } else if (j < 60) {
                f = (b & c) | (b & d) | (c & d);
                k = 0x8F1BBCDC;
            } else {
                f = b ^ c ^ d;
                k = 0xCA62C1D6;
            }

            uint32_t temp = ((a << 5) | (a >> 27)) + f + e + k + w[j];
            e = d;
            d = c;
            c = (b << 30) | (b >> 2);
            b = a;
            a = temp;
        }

        h0 = h0 + a;
        h1 = h1 + b;
        h2 = h2 + c;
        h3 = h3 + d;
        h4 = h4 + e;
    }

    std::stringstream ss;

    ss << std::hex
       << std::setfill('0')
       << std::setw(8) << h0
       << std::setw(8) << h1
       << std::setw(8) << h2
       << std::setw(8) << h3
       << std::setw(8) << h4;

    return ss.str();
}
