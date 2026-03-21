#include <parse.hpp>
#include <iostream>

namespace shell {

static std::optional<Redirect> parse_redirect(const std::vector<Token>& tokens, size_t& i) {
    if (i >= tokens.size() || tokens[i].type != TokenType::Redirect) {
        return std::nullopt;
    }

    if (i + 1 >= tokens.size()) {
        std::cerr << "parse error near \\n\n";
        exit(1);
    }

    const auto& op = tokens[i].value;
    const auto& target = tokens[++i].value;

    if (op == "1>" || op == ">") {
        return Redirect{ 1, target, RedirectOp::Replace };
    } else if (op == "2>") {
        return Redirect{ 2, target, RedirectOp::Replace };
    } else if (op == "1>>" || op == ">>") {
        return Redirect{ 1, target, RedirectOp::Append };
    } else if (op == "2>>") {
        return Redirect{ 2, target, RedirectOp::Append };
    }
    return std::nullopt;
}

Pipeline parse(const std::vector<Token>& tokens) {
    Pipeline ret;
    size_t i = 0;
    while (i < tokens.size()) {
        Command cmd;
        cmd.program = tokens[i++].value;

        while (i < tokens.size() && tokens[i].type == TokenType::Word) {
            cmd.args.push_back(tokens[i].value);
            i++;
        }

        if (i < tokens.size() && tokens[i].type == TokenType::Redirect) {
            cmd.redirect = parse_redirect(tokens, i);
            i++;
        }

        ret.push_back(cmd);

        if (i < tokens.size() && tokens[i].type == TokenType::Pipe) {
            i++;
        }
    }
    return ret;
}

} // namespace shell
