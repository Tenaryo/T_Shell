#pragma once
#include <types.hpp>
#include <vector>
#include <optional>

namespace shell {

Pipeline parse(const std::vector<Token>& tokens);

} // namespace shell
