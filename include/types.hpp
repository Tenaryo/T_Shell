#pragma once
#include <optional>
#include <string>
#include <vector>

namespace shell {

enum class TokenType {
    Word,
    Redirect,
    Pipe,
};

struct Token {
    TokenType type;
    std::string value;

    Token(TokenType t, std::string v) : type(t), value(std::move(v)) {};
};

enum class RedirectOp {
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
