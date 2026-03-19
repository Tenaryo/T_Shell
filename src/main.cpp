#include <lexer.hpp>
#include <parse.hpp>
#include <execute.hpp>
#include <iostream>

static void handle_cmd(const std::string& line) {
    if (line.empty()) return;

    auto tokens = shell::lexer(line);
    auto cmd = shell::parse(tokens);

    shell::execute(cmd);
}

int main() {
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    while (true) {
        std::cout << "$ ";
        std::string line;
        std::getline(std::cin >> std::ws, line);
        handle_cmd(std::move(line));
    }

    return 0;
}
