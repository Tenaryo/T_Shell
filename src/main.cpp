#include <iostream>
#include <string>
#include <string_view>
#include <sstream>
#include <unordered_map>
#include <functional>

void cmd_type();
void cmd_echo();
void cmd_exit();

static const std::unordered_map<std::string, std::function<void()>> commands {
  { "type", cmd_type },
  { "echo", cmd_echo },
  { "exit", cmd_exit }
};

void handle_not_found(std::string_view command) {
  std::cerr << command << ": command not found\n";
}

void cmd_type() {
  std::string command;
  std::getline(std::cin >> std::ws, command);
  std::stringstream ss(command);

  while (ss >> command) {
    auto it = commands.find(command);
    if (it != commands.end()) {
      std::cout << it->first << " is a shell builtin\n";
    } else {
      handle_not_found(command);
    }
  }
}

void cmd_echo() {
  std::string s;
  std::getline(std::cin >> std::ws, s);
  std::cout << s << '\n';
}

void cmd_exit() {
  exit(0);
}

void exec_shell() {
  std::cout << "$ ";
  std::string command;
  std::cin >> command;

  auto it = commands.find(command);
  if (it != commands.end()) it->second();
  else handle_not_found(command);
}

int main() {
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  while (true) {
    exec_shell();
  }

  return 0;
}
