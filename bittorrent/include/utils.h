#include <filesystem>
#include <string>

namespace bittorrent::utils {
std::string readFile(const std::filesystem::path& inPath);
}