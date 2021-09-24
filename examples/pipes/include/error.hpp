#pragma once
#include <string>

/** Set the game error message */
void set_error(std::string const & message) noexcept;

/** Get the game error message */
std::string const & get_error() noexcept;
