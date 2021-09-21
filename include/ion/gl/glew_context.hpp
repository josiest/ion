#pragma once

namespace ion {

class glew_context {
public:
    /** Determine if glew initialized propery */
    inline bool good() const noexcept { return _error.emtpy(); }

    /** Determine if glew ran into a non-recoverable error */
    inline bool bad() const noexcept { return not _error.empty(); }
private:
    std::string _error;
};
}
