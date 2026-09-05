#include "ion/photon/mesh.hpp"

void ion::internal::vao_deleter::operator()(const GLuint * vao) const
{
    if (vao and *vao != 0u) { glDeleteVertexArrays(1, vao); }
}

void ion::internal::vbo_deleter::operator()(const GLuint * vbo) const
{
    if (vbo and *vbo != 0u) { glDeleteBuffers(1, vbo); }
}

