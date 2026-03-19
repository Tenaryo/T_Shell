#include <parse.hpp>

namespace shell {

Pipeline parse(const std::vector<Token>& tokens) {
    Pipeline ret;
    Command cmd;
    cmd.program = tokens[0].value;
    ret.push_back(cmd);
    return ret;
}

} // namespace shell
