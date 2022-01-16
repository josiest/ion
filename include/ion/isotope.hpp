#pragma once
#include <string>

namespace ion {

/** A base class for SDL resource handlers. */
class isotope {
public:
    /** Determine if this object is okay to use */
    inline bool operator !() const noexcept { return not _error.empty(); }

    /** The error if this object isn't okay */
    inline std::string get_error() const noexcept { return _error; }

    /** Set the error message */
    inline void set_error(std::string const & message) noexcept { _error = message; }
private:
    std::string _error;
};
}
