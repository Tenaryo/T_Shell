#include <commands.hpp>
#include <executable_cache.hpp>
#include <execute.hpp>
#include <filesystem>
#include <fstream>
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

namespace history_impl {

static int last_written = 0;

static void do_read(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "history: " << filename << ": cannot open file\n";
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            add_history(line.c_str());
        }
    }
}

static void do_write(const std::string& filename) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "history: " << filename << ": cannot open file\n";
        return;
    }
    HIST_ENTRY** entries = history_list();
    if (entries) {
        for (int i = 0; i < history_length; i++) {
            file << entries[i]->line << '\n';
        }
    }
}

static void do_append(const std::string& filename) {
    std::ofstream file(filename, std::ios::app);
    if (!file) {
        std::cerr << "history: " << filename << ": cannot open file\n";
        return;
    }
    HIST_ENTRY** entries = history_list();
    if (entries) {
        for (int i = last_written; i < history_length; i++) {
            file << entries[i]->line << '\n';
        }
        last_written = history_length;
    }
}

static void do_show(int count) {
    HIST_ENTRY** entries = history_list();
    if (!entries)
        return;

    int total = history_length;
    int start = std::max(total - count, 0);

    for (int i = start; i < total; i++) {
        std::cout << "   " << i + 1 << "  " << entries[i]->line << '\n';
    }
}

} // namespace history_impl

void history(const Command& cmd) {
    if (cmd.args.empty()) {
        history_impl::do_show(history_length);
        return;
    }

    const auto& opt = cmd.args[0];

    if (opt == "-r") {
        if (cmd.args.size() < 2) {
            std::cerr << "history: -r: missing filename\n";
            return;
        }
        history_impl::do_read(cmd.args[1]);
    } else if (opt == "-w") {
        if (cmd.args.size() < 2) {
            std::cerr << "history: -w: missing filename\n";
            return;
        }
        history_impl::do_write(cmd.args[1]);
    } else if (opt == "-a") {
        if (cmd.args.size() < 2) {
            std::cerr << "history: -a: missing filename\n";
            return;
        }
        history_impl::do_append(cmd.args[1]);
    } else {
        try {
            int count = std::stoi(opt);
            if (count <= 0)
                return;
            history_impl::do_show(count);
        } catch (...) {
            std::cerr << "history: " << opt << ": invalid number\n";
        }
    }
}

} // namespace Commands

} // namespace shell
