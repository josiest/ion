#pragma once
#include <string_view>
#include <cstdint>

namespace ion
{
bool read_resolution(std::string_view src, std::uint32_t & width, std::uint32_t & height);
}
