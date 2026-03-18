#pragma once

#include <optional>
#include <filesystem>
#include <string>

std::optional<std::filesystem::path> search_path(std::string command);
