#pragma once
#include <gl/glew.h>
#include <memory>
#include <string_view>
#include <cstdint>

namespace ion
{
namespace internal
{
constexpr std::uint32_t ION_GL_LOG_LENGTH = 512u;
struct shader_component_deleter
{
    void operator()(const GLuint * id) const;
};

struct shader_program_deleter
{
    void operator()(const GLuint * id) const;
};
}

using shader_component = std::unique_ptr<GLuint, internal::shader_component_deleter>;
using shader_program = std::unique_ptr<GLuint, internal::shader_program_deleter>;

shader_component compile_shader_component(GLenum component_type, std::string_view source_code);
shader_component compile_vertex_shader(std::string_view source_code);
shader_component compile_fragment_shader(std::string_view source_code);

shader_program link_shader(GLuint vertex_shader, GLuint fragment_shader);
shader_program compile_shader(std::string_view vertex_source, std::string_view fragment_source);
}