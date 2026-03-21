#pragma once
#include <optional>
#include <types.hpp>
#include <vector>

namespace shell {

Pipeline parse(const std::vector<Token>& tokens);

} // namespace shell
