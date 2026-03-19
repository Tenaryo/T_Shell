#include <execute.hpp>
#include <types.hpp>
#include <iostream>

namespace shell {

void execute(Pipeline pipe) {
    for (auto& cmd : pipe) {
        std::cerr << cmd.program << ": command not found\n";
    }
}

} // namespace shell
