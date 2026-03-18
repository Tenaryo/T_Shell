#include <commands.hpp>
#include <utils.hpp>

#include <iostream>
#include <sstream>
#include <filesystem>

void cmd::cd() {
    std::string dir;
    std::getline(std::cin >> std::ws, dir);

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

void cmd::pwd() {
    try {
        std::filesystem::path cwd = std::filesystem::current_path();
        std::cout << cwd.string() << '\n';
    } catch (const std::filesystem::filesystem_error &e) {
        perror("Failed to get cwd");
    }
}

void cmd::type() {
    std::string command;
    std::getline(std::cin >> std::ws, command);
    std::stringstream ss(command);

    while (ss >> command) {
        auto it = cmd::commands.find(command);
        if (it != cmd::commands.end()) {
        std::cout << it->first << " is a shell builtin\n";
        } else if (auto full = search_path(command)) {
        std::cout << command << " is " << full->string() << '\n';
        } else {
        std::cerr << command << ": not found\n";
        }
    }
}

void cmd::echo() {
    std::string s;
    std::getline(std::cin >> std::ws, s);
    std::cout << s << '\n';
}

void cmd::exit() {
    std::exit(0); 
}
