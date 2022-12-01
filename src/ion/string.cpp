#include "ion/string.hpp"
#include <algorithm>

namespace ion {
void strlower(std::string & str)
{
    std::transform(str.begin(), str.end(), str.begin(),
                   [](unsigned char c) { return std::tolower(c); });
}
}
