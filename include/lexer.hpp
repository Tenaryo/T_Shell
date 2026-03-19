#pragma once
#include <types.hpp>
#include <vector>

namespace shell {

std::vector<Token> lexer(const std::string& line);

} // namespace shell
