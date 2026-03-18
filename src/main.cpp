#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <functional>

void cmd_echo() {
  std::string s;
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  std::getline(std::cin, s);
  std::cout << s << '\n';
}

void cmd_exit() {
  exit(0);
}

void handle_not_found(std::string_view command) {
  std::cerr << command << ": command not found\n";
}

void handle_input() {
  static const std::unordered_map<std::string, std::function<void()>> commands {
    { "echo", cmd_echo },
    { "exit", cmd_exit }
  };
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
    std::cout << "$ ";
    handle_input();
  }

  return 0;
}
