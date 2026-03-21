#include <iostream>
#include <lexer.hpp>

namespace shell {

std::vector<Token> lexer(const std::string& s) {
    std::vector<Token> ret;
    bool is_in_single_quote{false};
    bool is_in_double_quote{false};
    std::string cur;
    for (size_t i = 0; i < s.size(); i++) {
        char c = s[i];
        if (std::isspace(c) && !is_in_single_quote && !is_in_double_quote) {
            if (!cur.empty()) {
                ret.emplace_back(TokenType::Word, cur);
                cur.clear();
            }
            continue;
        }

        if (c == '\'' && !is_in_double_quote) {
            is_in_single_quote = !is_in_single_quote;
        } else if (c == '\"' && !is_in_single_quote) {
            is_in_double_quote = !is_in_double_quote;
        } else if (c == '\\') {
            if (!is_in_single_quote && !is_in_double_quote) {
                if (i == s.size() - 1)
                    cur += s[i];
                else
                    cur += s[++i];
            } else if (is_in_double_quote) {
                if (i == s.size() - 1)
                    std::cerr << "unclosed double quote\n";
                else if (s[i + 1] == '\\' || s[i + 1] == '\"')
                    cur += s[++i];
            } else
                cur += c;
        } else if (c == '|' && !is_in_double_quote && !is_in_single_quote) {
            if (!cur.empty()) {
                ret.emplace_back(TokenType::Word, cur);
                cur.clear();
            }
            ret.emplace_back(TokenType::Pipe, "|");
        } else if (c == '>' && !is_in_double_quote && !is_in_single_quote) {
            std::string fd;
            if (!cur.empty() && (cur.back() == '1' || cur.back() == '2')) {
                fd += cur.back();
                cur.pop_back();
            }
            if (!cur.empty())
                ret.emplace_back(TokenType::Word, cur);
            if (i + 1 < s.size() && s[i + 1] == '>') {
                ret.emplace_back(TokenType::Redirect, fd + ">>");
                i++;
            } else {
                ret.emplace_back(TokenType::Redirect, fd + ">");
            }
            cur.clear();
        } else {
            cur += c;
        }
    }
    if (!cur.empty())
        ret.emplace_back(TokenType::Word, cur);
    return ret;
}

} // namespace shell
