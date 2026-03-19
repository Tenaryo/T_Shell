#pragma once
#include <string>
#include <vector>
#include <optional>

namespace shell {

enum class TokenType {
    Word,
    Redict,
};

struct Token {
    TokenType type;
    std::string value;
};

enum class RedirectOp{
    Replace,
    Append,
};

struct Redirect {
    int fd;
    std::string target;
    RedirectOp op;
};

struct Command {
    std::string program;
    std::vector<std::string> args;
    std::optional<Redirect> redirect;
};

using Pipeline = std::vector<Command>;

} // namespace shell
