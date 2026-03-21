#include <execute.hpp>
#include <iostream>
#include <lexer.hpp>
#include <parse.hpp>
#include <reader.hpp>

static void handle_cmd(const std::string& line) {
    if (line.empty())
        return;

    auto tokens = shell::lexer(line);
    auto pipelines = shell::parse(tokens);

    shell::execute(pipelines);
}

int main() {
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    shell::LineReader reader;
    reader.set_prompt("$ ");

    while (true) {
        std::string line = reader.read_line();
        if (line.empty())
            continue;
        handle_cmd(std::move(line));
    }

    return 0;
}
