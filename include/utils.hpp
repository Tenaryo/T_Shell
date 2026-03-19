#pragma once
#include <string>
#include <optional>

namespace shell {

std::optional<std::string> search_path(const std::string& program);

} // namespace shell
