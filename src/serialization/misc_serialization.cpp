#include "ion/serialization/misc_serialization.hpp"

#include <string_view>
#include <cstdint>

#include <regex>
#include <string>

bool ion::read_resolution(std::string_view src, std::uint32_t & width, std::uint32_t & height)
{
    constexpr int IDX_WIDTH = 1;
    constexpr int IDX_HEIGHT = 2;
    const std::regex resolution_pattern{ "(\\d+)[Xx](\\d+)" };
    const std::string source{ src };

    if (std::smatch dimension_match;
        std::regex_search(source, dimension_match, resolution_pattern) and dimension_match.size() == 3)
    {
        width = std::stoul(dimension_match[IDX_WIDTH]);
        height = std::stoul(dimension_match[IDX_HEIGHT]);
        return true;
    }
    return false;
}
