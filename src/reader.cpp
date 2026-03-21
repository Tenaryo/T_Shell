#include <reader.hpp>
#include <commands.hpp>
#include <executable_cache.hpp>
#include <cstring>
#include <algorithm>
#include <readline/readline.h>
#include <readline/history.h>

namespace shell {

static char* command_generator(const char* text, int state) {
    static const std::vector<std::string> all_cmds = [] {
        std::vector<std::string> ret;
        for (const auto& [k, _] : Commands::cmds) {
            ret.push_back(k);
        }
        for (const auto& name : ExecutableCache::instance().names()) {
            if (!Commands::cmds.contains(name)) {
                ret.push_back(name);
            }
        }
        std::sort(ret.begin(), ret.end());
        return ret;
    }();

    static unsigned int match_idx, text_len;
    if (!state) {
        match_idx = 0;
        text_len = strlen(text);
    }

    while (match_idx < all_cmds.size()) {
        const char* cmd = all_cmds[match_idx++].c_str();
        if (strncmp(cmd, text, text_len) == 0) return strdup(cmd);
    }
    return nullptr;
}

static char** command_completion(const char* text, int start, int /*end*/) {
    if (start != 0) return nullptr;

    return rl_completion_matches(text, command_generator);
}

LineReader::LineReader() {
    rl_attempted_completion_function = command_completion;
}

std::string LineReader::read_line() {
    char* raw = readline(prompt_.c_str());
    if (!raw) std::exit(0);

    std::string line(raw);
    if (!line.empty()) {
        add_history(raw);
    }
    free(raw);

    return line;
}

} // namespace shell
