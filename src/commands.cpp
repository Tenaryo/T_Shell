#include <commands.hpp>
#include <utils.hpp>

#include <iostream>
#include <sstream>
#include <filesystem>

void cmd::cd(std::vector<std::string>& tokens) {
    std::string& dir = tokens[1];

    if (!dir.empty() && dir[0] == '~') {
        const char* home = getenv("HOME");
        if (!home) {
            std::cerr << "cd: HOME not set\n";
            return;
        }
        dir = home + dir.substr(1);
    }

    try {
        std::filesystem::current_path(dir);
    } catch (const std::filesystem::filesystem_error &e) {
        std::cerr << "cd: " << dir << ": No such file or directory\n";
    }
}

void cmd::pwd(std::vector<std::string>& tokens) {
    try {
        std::filesystem::path cwd = std::filesystem::current_path();
        std::cout << cwd.string() << '\n';
    } catch (const std::filesystem::filesystem_error &e) {
        perror("Failed to get cwd");
    }
}

void cmd::type(std::vector<std::string>& tokens) {
    for (int i = 1; i < tokens.size(); i++) {
        auto it = cmd::commands.find(tokens[i]);
        if (it != cmd::commands.end()) {
        std::cout << it->first << " is a shell builtin\n";
        } else if (auto full = search_path(tokens[i])) {
        std::cout << tokens[i] << " is " << full->string() << '\n';
        } else {
        std::cerr << tokens[i] << ": not found\n";
        }
    }
}

void cmd::echo(std::vector<std::string>& tokens) {
    if (tokens.size() == 1) return;
    std::cout << tokens[1];
    for (int i = 2; i < tokens.size(); i++) {
        std::cout << ' ' << tokens[i];
    }
    std::cout << '\n';
}

void cmd::exit(std::vector<std::string>& tokens) {
    std::exit(0); 
}
