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

std::vector<std::string> tokenize(std::string s) {
    std::vector<std::string> ret;
    bool is_in_quote{false};
    std::string cur;
    for (char c : s) {
        if (std::isspace(c) && !is_in_quote) {
            if (!cur.empty()) {
                ret.push_back(cur);
                cur.clear();
            }
            continue;
        }

        if (c == '\'') is_in_quote = !is_in_quote;
        else cur += c;
    }
    if (!cur.empty()) ret.push_back(cur);
    return ret;
}
