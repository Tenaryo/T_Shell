#include <iostream>
#include <string>

void inline handle_not_found(string_view usrInput) {
  std::cerr << usrInput << ": command not found\n";
}

void handle_input() {
  std::string_view usrInput;
  std::cin >> usrInput;
  handle_not_found(usrInput);
}

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  while (true) {
    std::cout << "$ ";
    handle_input();
  }
}
