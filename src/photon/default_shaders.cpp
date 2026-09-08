#include "ion/photon/default_shaders.hpp"

ion::shader::solid_fill::solid_fill(GLuint program)
    : handle{ .id = program, .shader_name = std::string(shader_name) }
{
}

void ion::shader::solid_fill::use_program() const
{
    glUseProgram(handle.id);
    handle.uniform("transform", transform);
    handle.uniform("fill_color", color);
}
