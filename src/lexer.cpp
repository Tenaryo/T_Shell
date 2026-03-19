#include <lexer.hpp>
#include <iostream>

namespace shell {

std::vector<Token> lexer(const std::string& s) {
    std::vector<Token> ret;
    bool is_in_single_quote{false};
    bool is_in_double_quote{false};
    std::string cur;
    for (int i = 0; i < s.size(); i++) {
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
                if (i == s.size() - 1) cur += s[i];
                else cur += s[++i];
            } else if (is_in_double_quote) {
                if (i == s.size() - 1) std::cerr << "unclosed double quote\n";
                else if (s[i + 1] == '\\' || s[i + 1] == '\"') cur += s[++i];
            } else cur += c;
        } else if (c == '>' && !is_in_double_quote && !is_in_single_quote) {
            if (!cur.empty() && cur.back() == '1') {
                cur.pop_back();
            }
            if (!cur.empty()) ret.emplace_back(TokenType::Word, cur);
            ret.emplace_back(TokenType::Redirect, "1>");
            cur.clear();
        } else {
            cur += c;
        }
    }
    if (!cur.empty()) ret.emplace_back(TokenType::Word, cur);
    return ret;
}

}
