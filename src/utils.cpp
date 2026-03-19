#include <utils.hpp>
#include <sstream>
#include <filesystem>
#include <unistd.h>

namespace shell {

std::optional<std::string> search_path(const std::string& program) {
    static const char* cpath = getenv("PATH");
    std::stringstream ss(cpath);
    std::string dir;
    while (std::getline(ss, dir, ':')) {
        std::filesystem::path full = std::filesystem::path(dir) / program;
        if (std::filesystem::exists(full) && access(full.c_str(), X_OK) == 0) {
            return full.string();
        }
    }
    return std::nullopt;
}

} // namespace shell
