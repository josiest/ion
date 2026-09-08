#pragma once

#include <optional>
#include <string_view>
#include <string>

namespace ion::internal
{
std::optional<std::string> read_file(std::string_view path);
}