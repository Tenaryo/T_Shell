#include <commands.hpp>
#include <utils.hpp>

#include <iostream>
#include <sys/wait.h>
#include <unistd.h>

void exec_external(std::vector<std::string>& tokens) {
    std::vector<char *> argv;
    for (auto &token : tokens)
        argv.push_back(const_cast<char *>(token.c_str()));
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
    std::string line;
    std::getline(std::cin, line);
    if (line.empty()) return;
    auto tokens = tokenize(line);

    auto it = cmd::commands.find(tokens[0]);
    if (it != cmd::commands.end()) {
        it->second(tokens);
    } else if (search_path(tokens[0]) != std::nullopt) {
        exec_external(tokens);
    } else {
        std::cerr << tokens[0] << ": command not found\n";
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
