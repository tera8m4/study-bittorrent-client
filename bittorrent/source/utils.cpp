#include "utils.h"
#include <filesystem>
#include <fstream>
#include <iostream>

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