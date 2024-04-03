#include "hash.h"
#include <filesystem>
#include <string>

namespace bittorrent::utils {
std::string readFile(const std::filesystem::path& inPath);

using Sha1Hash = HashView<20>;

Sha1Hash sha1(const std::string& inData);
}