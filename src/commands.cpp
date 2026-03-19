#include <commands.hpp>
#include <execute.hpp>
#include <utils.hpp>
#include <filesystem>
#include <iostream>

namespace shell {

namespace Commands {

void cd(const Command& cmd) {
    if (cmd.args.size() > 1) {
        std::cerr << "cd: too many arguments\n";
        return;
    }

    std::string dir;
    if (cmd.args.size() == 0) dir = "~";
    else dir = cmd.args[0];

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

void pwd(const Command& cmd) {
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
            if (auto full = search_path(arg)) {
                std::cout << arg << " is " << full.value() << '\n';
            } else {
                std::cout << arg << ": not found\n";
            }
        }
    }
}

void echo(const Command& cmd) {
    if (cmd.args.empty()) return;
    std::cout << cmd.args[0];
    for (int i = 1; i < cmd.args.size(); i++) {
        std::cout << ' ' << cmd.args[i];
    }
    std::cout << '\n';
}

void exit(const Command& cmd) {
    std::exit(0);
}

} // namespace Commands

} // namespace shell
