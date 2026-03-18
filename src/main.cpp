#include <commands.hpp>
#include <utils.hpp>

#include <iostream>
#include <sys/wait.h>
#include <unistd.h>

void exec_external(const std::string &full, const std::string &orig_args) {
    std::stringstream ss(orig_args);
    std::string token;
    std::vector<std::string> args;

    while (ss >> token)
        args.push_back(token);
    if (args.empty())
        return;

    std::vector<char *> argv;
    argv.push_back(const_cast<char *>(full.data()));
    for (auto &s : args)
        argv.push_back(s.data());
    argv.push_back(nullptr);

    pid_t pid = fork();
    if (pid == 0) {
        execvp(argv[0], argv.data());
        perror("execvp failed");
        exit(1);
    } else if (pid > 0) {
        int status;
        waitpid(pid, &status, 0);
    } else {
        perror("fork failed");
    }
}

void exec_shell() {
    std::cout << "$ ";
    std::string command;
    std::cin >> command;

    auto it = cmd::commands.find(command);
    if (it != cmd::commands.end()) {
        it->second();
    } else if (auto full = search_path(command)) {
        std::string args;
        std::getline(std::cin >> std::ws, args);
        exec_external(command, args);
    } else {
        std::cerr << command << ": command not found\n";
    }
}

int main() {
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    while (true) {
        exec_shell();
    }

    return 0;
}
