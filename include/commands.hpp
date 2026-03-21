#pragma once
#include <functional>
#include <string>
#include <types.hpp>
#include <unordered_map>

namespace shell {

namespace Commands {

void cd(const Command& cmd);
void pwd(const Command& cmd);
void type(const Command& cmd);
void exit(const Command& cmd);
void echo(const Command& cmd);
void history(const Command& cmd);

using CmdFunction = std::function<void(const Command& cmd)>;

inline const std::unordered_map<std::string, CmdFunction> cmds = {
    {"cd", cd},
    {"pwd", pwd},
    {"type", type},
    {"echo", echo},
    {"history", history},
    {"exit", exit},
};

} // namespace Commands

} // namespace shell
