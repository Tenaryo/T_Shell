#pragma once
#include <types.hpp>
#include <unordered_map>
#include <string>
#include <functional>

namespace shell {

namespace Commands {

void cd(const Command& cmd);
void pwd(const Command& cmd);
void type(const Command& cmd);
void exit(const Command& cmd);
void echo(const Command& cmd);

using CmdFunction = std::function<void(const Command& cmd)>;

const std::unordered_map<std::string, CmdFunction> cmds = {
    { "cd", cd },
    { "pwd", pwd },
    { "type", type },
    { "echo", echo },
    { "exit", exit },
};

} // namespace Commands

} // namespace shell
