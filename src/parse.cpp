#include <parse.hpp>
#include <iostream>

namespace shell {

Pipeline parse(const std::vector<Token>& tokens) {
    Pipeline ret;
    for (int i = 0; i < tokens.size(); i++) {
        Command cmd;
        cmd.program = tokens[i++].value;
        while (i < tokens.size() && tokens[i].type == TokenType::Word) {
            cmd.args.push_back(tokens[i].value);
            i++;
        }
        if (i + 1 < tokens.size() && tokens[i].type == TokenType::Redirect) {
            if (i + 1 < tokens.size()) {
                if (tokens[i].value == "1>") cmd.redirect = { 1, tokens[++i].value, RedirectOp::Replace };
                else cmd.redirect = { 1, tokens[++i].value, RedirectOp::Replace };
            } else {
                // TODO: error handling
                std::cerr << "parse error near \\n\n";
                exit(1);
            }
        } else {
            cmd.redirect = std::nullopt;
        }
        ret.push_back(cmd);
    }
    return ret;
}

} // namespace shell
