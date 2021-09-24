#pragma once
#include <string>

namespace ion {

/** Set the ion error message */
void set_error(std::string const & message) noexcept;

/** Get the ion error message */
std::string const & get_error() noexcept;
}
