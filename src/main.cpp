#include <iostream>
#include <string>

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  std::cout << "$ ";
  std::string usrInput;
  std::cin >> usrInput;
  std::cerr << usrInput << ": command not found\n";
}
