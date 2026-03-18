#pragma once

#include <unordered_map>
#include <string>
#include <functional>
#include <vector>

namespace cmd {

void cd(std::vector<std::string>& tokens);
void pwd(std::vector<std::string>& tokens);
void type(std::vector<std::string>& tokens);
void echo(std::vector<std::string>& tokens);
void exit(std::vector<std::string>& tokens);

const std::unordered_map<std::string, std::function<void(std::vector<std::string>&)>> commands{
    { "cd"  , cd   },
    { "pwd" , pwd  },
    { "type", type },
    { "echo", echo },
    { "exit", exit }
};

}; // namespace
