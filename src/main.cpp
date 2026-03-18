#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <functional>

void cmd_echo() {
  std::string s;
  std::getline(std::cin, s);
  std::cout << s << '\n';
}

void cmd_exit() {
  exit(0);
}

void handle_not_found(std::string_view command) {
  std::cerr << command << ": command not found\n";
}

void exec_shell() {
  static const std::unordered_map<std::string, std::function<void()>> commands {
    { "echo", cmd_echo },
    { "exit", cmd_exit }
  };
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
