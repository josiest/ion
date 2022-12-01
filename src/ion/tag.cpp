#include "ion/tag.hpp"
#include <iostream>

std::ostream & operator<<(std::ostream & os, ion::tag const & tag)
{
    os << tag.string();
    return os;
}
std::istream & operator>>(std::istream & is, ion::tag & tag)
{
    std::string input;
    is >> input;
    tag = ion::tag{ input };
    return is;
}
