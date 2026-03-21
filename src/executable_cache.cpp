#include <algorithm>
#include <executable_cache.hpp>
#include <filesystem>
#include <ranges>
#include <sstream>
#include <unistd.h>
#include <unordered_map>

namespace shell {

void ExecutableCache::scan_path() {
    names_.clear();
    paths_.clear();

    std::unordered_map<std::string, std::string> tmp;

    const char* cpath = getenv("PATH");
    if (!cpath)
        return;

    std::stringstream ss(cpath);
    std::string dir;

    while (std::getline(ss, dir, ':')) {
        try {
            for (const auto& entry : std::filesystem::directory_iterator(dir)) {
                if (!entry.is_regular_file())
                    continue;
                if (access(entry.path().c_str(), X_OK) != 0)
                    continue;

                std::string name = entry.path().filename().string();
                if (!tmp.contains(name)) {
                    tmp.emplace(std::move(name), entry.path().string());
                }
            }
        } catch (...) {
        }
    }

    names_.reserve(tmp.size());
    paths_.reserve(tmp.size());
    for (auto& [name, path] : tmp) {
        names_.push_back(std::move(name));
        paths_.push_back(std::move(path));
    }

    std::vector<size_t> idx(names_.size());
    for (size_t i = 0; i < idx.size(); ++i)
        idx[i] = i;
    std::ranges::sort(idx, [&](size_t a, size_t b) { return names_[a] < names_[b]; });

    std::vector<std::string> sorted_names(names_.size());
    std::vector<std::string> sorted_paths(paths_.size());
    for (size_t i = 0; i < idx.size(); ++i) {
        sorted_names[i] = std::move(names_[idx[i]]);
        sorted_paths[i] = std::move(paths_[idx[i]]);
    }
    names_ = std::move(sorted_names);
    paths_ = std::move(sorted_paths);
}

bool ExecutableCache::contains(const std::string& name) const {
    auto it = std::ranges::lower_bound(names_, name);
    return it != names_.end() && *it == name;
}

std::optional<std::string> ExecutableCache::get_path(const std::string& name) const {
    auto it = std::ranges::lower_bound(names_, name);
    if (it != names_.end() && *it == name) {
        return paths_[static_cast<size_t>(std::distance(names_.begin(), it))];
    }
    return std::nullopt;
}

void ExecutableCache::refresh() { scan_path(); }

} // namespace shell
