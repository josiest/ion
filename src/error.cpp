#include "ion/error.hpp"
#include <string>

namespace ion {

namespace _error {
std::string message;
};

void set_error(std::string const & message) noexcept { _error::message = message; }
std::string const & get_error() noexcept { return _error::message; }
}
