#include <utils.hpp>
#include <sstream>
#include <unistd.h>

std::optional<std::filesystem::path> search_path(std::string command) {
    static const char *cpath = getenv("PATH");
    std::stringstream ss(cpath);
    std::string dir;
    while (getline(ss, dir, ':')) {
        std::filesystem::path full = std::filesystem::path(dir) / command;
        if (std::filesystem::exists(full) && access(full.c_str(), X_OK) == 0) {
            return full;
        }
    }
    return std::nullopt;
}
