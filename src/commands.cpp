#include <commands.hpp>
#include <executable_cache.hpp>
#include <execute.hpp>
#include <filesystem>
#include <iostream>
#include <readline/history.h>

namespace shell {

namespace Commands {

void cd(const Command& cmd) {
    if (cmd.args.size() > 1) {
        std::cerr << "cd: too many arguments\n";
        return;
    }

    std::string dir;
    if (cmd.args.empty())
        dir = "~";
    else
        dir = cmd.args[0];

    if (dir[0] == '~') {
        const char* home = getenv("HOME");
        if (!home) {
            std::cerr << "cd: HOME not set\n";
            return;
        }
        dir = home + dir.substr(1);
    }

    try {
        std::filesystem::current_path(dir);
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "cd: " << dir << ": No such file or directory\n";
    }
}

void pwd(const Command& /*cmd*/) {
    try {
        auto path = std::filesystem::current_path();
        std::cout << path.string() << '\n';
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "get current path failed\n";
    }
}

void type(const Command& cmd) {
    for (auto& arg : cmd.args) {
        auto it = (cmds.find(arg));
        if (it != cmds.end()) {
            std::cout << arg << " is a shell builtin\n";
        } else {
            if (auto full = ExecutableCache::instance().get_path(arg)) {
                std::cout << arg << " is " << full.value() << '\n';
            } else {
                std::cout << arg << ": not found\n";
            }
        }
    }
}

void echo(const Command& cmd) {
    if (cmd.args.empty())
        return;
    std::cout << cmd.args[0];
    for (size_t i = 1; i < cmd.args.size(); i++) {
        std::cout << ' ' << cmd.args[i];
    }
    std::cout << '\n';
}

void exit(const Command& /*cmd*/) { std::exit(0); }

void history(const Command& cmd) {
    HIST_ENTRY** entries = history_list();
    if (!entries)
        return;

    int total = history_length;
    int count = total;

    if (!cmd.args.empty()) {
        try {
            count = std::stoi(cmd.args[0]);
            if (count <= 0)
                return;
        } catch (...) {
            std::cerr << "history: " << cmd.args[0] << ": invalid number\n";
            return;
        }
    }

    int start = std::max(total - count, 0);

    for (int i = start; i < total; i++) {
        std::cout << "   " << i + 1 << "  " << entries[i]->line << '\n';
    }
}

} // namespace Commands

} // namespace shell
