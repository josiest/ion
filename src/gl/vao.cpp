#define GL_GLEXT_PROTOTYPES
#include <SDL_opengl.h>
#include <SDL_opengl_glext.h>

#include "ion/gl/vao.hpp"

namespace ion {

vao::vao() noexcept
    : _id{0}
{
    // try to create the vertex array
    glGenVertexArrays(1, &_id);
    glBindVertexArray(_id);

    // abort if failure
    if (not glIsVertexArray(_id)) {
        _error = "Couldn't create a vertex array";
        return;
    }
}
vao::vao(vao && other) noexcept
    : _id{other._id}, _error{other._error}
{
    other._id = 0;
    other._error = "vertex array object was moved";
}
vao::~vao()
{
    if (glIsVertexArray(_id)) {
        glDeleteVertexArrays(1, &_id);
        _id = 0;
    }
}
bool vao::operator!() const noexcept { return not glIsVertexArray(_id); }
}
