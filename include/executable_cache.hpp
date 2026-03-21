#pragma once
#include <string>
#include <optional>
#include <vector>

namespace shell {

class ExecutableCache {
public:
    static ExecutableCache& instance() {
        static ExecutableCache cache;
        return cache;
    }

    bool contains(const std::string& name) const;
    std::optional<std::string> get_path(const std::string& name) const;
    const std::vector<std::string>& names() const { return names_; }
    void refresh();

private:
    ExecutableCache() { scan_path(); }
    void scan_path();

    std::vector<std::string> names_;
    std::vector<std::string> paths_;
};

} // namespace shell
