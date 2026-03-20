#include <reader.hpp>
#include <commands.hpp>
#include <cstring>
#include <cctype>
#include <readline/readline.h>

namespace shell {

static char* builtin_generator(const char* text, int state) {
    static const std::vector<std::string> builtin_cmds = [] {
        std::vector<std::string> ret;
        ret.reserve(Commands::cmds.size());
        for (const auto& [k, _] : Commands::cmds) {
            ret.push_back(k);
        }
        return ret;
    }();
    static unsigned int match_idx, text_len;
    if (!state) {
        match_idx = 0;
        text_len = strlen(text);
    }

    while (match_idx < builtin_cmds.size()) {
        const char* cmd = builtin_cmds[match_idx++].c_str();
        if (strncmp(cmd, text, text_len) == 0) return strdup(cmd);
    }
    return nullptr;
}

static char** builtin_completion(const char* text, int start, int /*end*/) {
    if (start != 0) return nullptr;

    return rl_completion_matches(text, builtin_generator);
}

LineReader::LineReader() {
    rl_attempted_completion_function = builtin_completion;
}

std::string LineReader::read_line() {
    char* raw = readline(prompt_.c_str());
    if (!raw) std::exit(0);

    std::string line(raw);
    free(raw);

    return line;
}

} // namespace shell
