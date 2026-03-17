#include <iostream>
#include <string>
#include <string_view>

void handle_not_found(std::string_view usrInput) {
  std::cerr << usrInput << ": command not found\n";
}

bool handle_input() {
  std::string usrInput;
  std::cin >> usrInput;
  if (usrInput == "exit") return true;
  handle_not_found(usrInput);
  return false;
}

int main() {
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  while (true) {
    std::cout << "$ ";
    if (handle_input()) break;
  }

  return 0;
}
