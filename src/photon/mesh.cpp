#include <gl/glew.h>
#include "ion/photon/mesh.hpp"

void ion::internal::vao_deleter::operator()(const GLuint * vao) const
{
    if (vao and *vao != 0u) { glDeleteVertexArrays(1, vao); }
}

void ion::internal::vbo_deleter::operator()(const GLuint * vbo) const
{
    if (vbo and *vbo != 0u) { glDeleteBuffers(1, vbo); }
}

void ion::simple_vertex2f::bind_attributes()
{
    glVertexAttribPointer(0u,                           // attribute at location 0
                          2, GL_FLOAT,                  // Has 2 float elements
                          internal::USE_FIXED_POINT_VERTEX,
                          sizeof(simple_vertex2f),      // stride of the array
                          reinterpret_cast<void *>(offsetof(simple_vertex2f, x)));
    glEnableVertexAttribArray(0u);
}

void ion::quad_vertex2f::bind_attributes()
{
    glVertexAttribPointer(0, 2, GL_FLOAT,
                          internal::USE_FIXED_POINT_VERTEX,
                          sizeof(quad_vertex2f),
                          reinterpret_cast<void *>(offsetof(quad_vertex2f, x)));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT,
                          internal::USE_FIXED_POINT_VERTEX,
                          sizeof(quad_vertex2f),
                          reinterpret_cast<void *>(offsetof(quad_vertex2f, u)));
    glEnableVertexAttribArray(1);
}
