#pragma once

#include <optional>
#include <filesystem>
#include <string>
#include <vector>

std::optional<std::filesystem::path> search_path(std::string command);
std::vector<std::string> tokenize(std::string s);
