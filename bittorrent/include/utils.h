#include <filesystem>
#include <string>

namespace bittorrent::utils {
std::string readFile(const std::filesystem::path& inPath);
std::string sha1(const std::string& inData);
}