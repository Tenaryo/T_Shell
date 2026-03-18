#pragma once

#include <unordered_map>
#include <string>
#include <functional>

namespace cmd {

void cd();
void pwd();
void type();
void echo();
void exit();

const std::unordered_map<std::string, std::function<void()>> commands{
    { "cd"  , cd   },
    { "pwd" , pwd  },
    { "type", type },
    { "echo", echo },
    { "exit", exit }
};

}; // namespace
