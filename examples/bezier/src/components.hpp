#pragma once

#include <SDL_opengl.h>
#include <ion/gl.hpp>
#include <glm/glm.hpp>

namespace component {

struct position {
    float x, y;
    operator glm::vec2() const { return glm::vec2(x, y); }
};

struct vao {
    GLuint value;
    operator GLuint() const { return value; }
};
}
