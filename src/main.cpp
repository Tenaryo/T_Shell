#include <filesystem>
#include <functional>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <unordered_map>

void cmd_pwd();
void cmd_type();
void cmd_echo();
void cmd_exit();

static const std::unordered_map<std::string, std::function<void()>> commands{
  {"pwd", cmd_pwd},
  {"type", cmd_type},
  {"echo", cmd_echo},
  {"exit", cmd_exit}
};

std::optional<std::filesystem::path> search_path(std::string command) {
  static const char *cpath = getenv("PATH");
  std::stringstream ss(cpath);
  std::string dir;
  while (getline(ss, dir, ':')) {
    std::filesystem::path full = std::filesystem::path(dir) / command;
    if (std::filesystem::exists(full) && access(full.c_str(), X_OK) == 0) {
      return full;
    }
  }
  return std::nullopt;
}

void cmd_pwd() {
  try {
    std::filesystem::path cwd = std::filesystem::current_path();
    std::cout << cwd.string() << '\n';
  } catch (const std::filesystem::filesystem_error &e) {
    perror("Failed to get cwd");
  }
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

void cmd_exit() { exit(0); }

void exec_external(const std::string &full, const std::string &orig_args) {
  std::stringstream ss(orig_args);
  std::string token;
  std::vector<std::string> args;

  while (ss >> token)
    args.push_back(token);
  if (args.empty())
    return;

  std::vector<char *> argv;
  argv.push_back(const_cast<char *>(full.data()));
  for (auto &s : args)
    argv.push_back(s.data());
  argv.push_back(nullptr);

  pid_t pid = fork();
  if (pid == 0) {
    execvp(argv[0], argv.data());
    perror("execvp failed");
    exit(1);
  } else if (pid > 0) {
    int status;
    waitpid(pid, &status, 0);
  } else {
    perror("fork failed");
  }
}

void exec_shell() {
  std::cout << "$ ";
  std::string command;
  std::cin >> command;

  auto it = commands.find(command);
  if (it != commands.end()) {
    it->second();
  } else if (auto full = search_path(command)) {
    std::string args;
    std::getline(std::cin >> std::ws, args);
    exec_external(command, args);
  } else {
    std::cerr << command << ": command not found\n";
  }
}

int main() {
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  while (true) {
    exec_shell();
  }

  return 0;
}
