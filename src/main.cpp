#include <filesystem>
#include <iostream>
#include <optional>
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

std::optional<std::filesystem::path> search_path(std::string command) {
  static const char* cpath = getenv("PATH");
  std::stringstream ss(cpath);
  std::string dir;
  while (getline(ss, dir, ':')) {
    std::filesystem::path full = std::filesystem::path(dir) / command;
    if (std::filesystem::exists(full)) {
      return full;
    }
  }
  return std::nullopt;
}

void cmd_type() {
  std::string command;
  std::getline(std::cin >> std::ws, command);
  std::stringstream ss(command);

  while (ss >> command) {
    auto it = commands.find(command);
    if (it != commands.end()) {
      std::cout << it->first << " is a shell builtin\n";
    } else if (auto full = search_path(command)) {
      std::cout << command << " is " << full->string() << '\n';
    } else {
      std::cerr << command << ": not found\n";
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
  else std::cerr << command << ": command not found\n";
}

int main() {
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  while (true) {
    exec_shell();
  }

  return 0;
}
