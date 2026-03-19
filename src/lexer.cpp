#include <lexer.hpp>

namespace shell {

std::vector<Token> lexer(const std::string& line) {
    std::vector<Token> ret;
    Token t{ TokenType::Word, line };
    ret.push_back(t);
    return ret;
}

}
