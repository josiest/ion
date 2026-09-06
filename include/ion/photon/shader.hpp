#pragma once
#include <gl/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <memory>
#include <string_view>
#include <cstdint>

#include <SDL3/SDL_log.h>

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

template<typename T>
concept shader_uniform = requires(const T & value)
{
    glm::value_ptr(value);
};

struct shader_handle
{
    std::string shader_name;
    GLuint id = 0;

    void use_program() const;

    template<shader_uniform T>
    void uniform(std::string_view uniform_name, const T & value) const;
};

}

template<ion::shader_uniform T>
void ion::shader_handle::uniform(std::string_view uniform_name, const T & value) const
{
    const GLint location = glGetUniformLocation(id, uniform_name.data());
    if (location < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "shader \"%s\" has no uniform named \"%s\"\n",
                     shader_name.c_str(), uniform_name.data());
        return;
    }
    if constexpr (std::is_same_v<T, glm::mat4x4>)
    {
        glUniformMatrix4fv(location, 1u, GL_FALSE, glm::value_ptr(value));
    }
    else if constexpr (std::is_same_v<T, glm::vec4>)
    {
        glUniform4fv(location, 1u, glm::value_ptr(value));
    }
}
