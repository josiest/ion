#pragma once

#include <SDL_opengl.h>
#include <string>

namespace ion {

/** A resource handler for an opengl vertex array object */
class vao {
public:
    // delete unwanted implicit constructors
    vao(vao const &) = delete;

    /** Create a basic vao */
    vao() noexcept;

    /** Create a vao from a temporary vao */
    vao(vao && other) noexcept;
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
