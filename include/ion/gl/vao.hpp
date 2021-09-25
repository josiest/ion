#pragma once

#include <SDL_opengl.h>
#include <string>

namespace ion {

/** A resource handler for an opengl vertex array object */
class vao {
public:
    /** Create a basic vao */
    vao() noexcept;
    ~vao();

    /** Cast this vao to its underling opengl id */
    inline operator GLuint() const noexcept { return _id; }

    /** Determine if this vao is not okay to use */
    bool operator!() const noexcept;

    /** The error if this vao isn't okay to use */
    std::string const & error() const noexcept { return _error; }
private:
    GLuint _id;
    std::string _error;
};
}
