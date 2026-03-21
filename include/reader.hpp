#pragma once
#include <string>

namespace shell {

class LineReader {
  public:
    LineReader();
    ~LineReader();
    void set_prompt(std::string prompt) { prompt_ = std::move(prompt); }
    std::string read_line();
  private:
    std::string prompt_;
    std::string histfile_;
};

} // namespace shell
