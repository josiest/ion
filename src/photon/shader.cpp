#include "ion/photon/shader.hpp"
#include <SDL3/SDL_log.h>

void ion::internal::shader_component_deleter::operator()(const GLuint * id) const
{
    if (id and *id != 0u) { glDeleteShader(*id); }
}

void ion::internal::shader_program_deleter::operator()(const GLuint * id) const
{
    if (id and *id != 0u) { glDeleteProgram(*id); }
}

ion::shader_component ion::compile_shader_component(GLenum component_type, std::string_view source_code)
{
    const GLuint shader_id = glCreateShader(component_type);
    const char * raw_source = source_code.data();
    glShaderSource(shader_id, 1, &raw_source, nullptr);
    glCompileShader(shader_id);

    GLint did_shader_compile = 0;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &did_shader_compile);

    if (not did_shader_compile)
    {
        GLchar error_log[internal::ION_GL_LOG_LENGTH];
        glGetShaderInfoLog(shader_id, internal::ION_GL_LOG_LENGTH, nullptr, error_log);
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't compile shader: %s\n", error_log);
        return nullptr;
    }
    return shader_component(new GLuint(shader_id), internal::shader_component_deleter{});
}

ion::shader_component ion::compile_vertex_shader(std::string_view source_code)
{
    return compile_shader_component(GL_VERTEX_SHADER, source_code);
}

ion::shader_component ion::compile_fragment_shader(std::string_view source_code)
{
    return compile_shader_component(GL_FRAGMENT_SHADER, source_code);
}

ion::shader_program ion::link_shader(GLuint vertex_shader, GLuint fragment_shader)
{
    const GLuint program_id = glCreateProgram();
    glAttachShader(program_id, vertex_shader);
    glAttachShader(program_id, fragment_shader);
    glLinkProgram(program_id);

    GLint did_program_link = 0;
    glGetProgramiv(program_id, GL_LINK_STATUS, &did_program_link);

    if (not did_program_link)
    {
        GLchar error_log[internal::ION_GL_LOG_LENGTH];
        glGetProgramInfoLog(program_id, internal::ION_GL_LOG_LENGTH, nullptr, error_log);
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", error_log);
        return nullptr;
    }
    return shader_program(new GLuint(program_id), internal::shader_program_deleter{});
}

ion::shader_program ion::compile_shader(std::string_view vertex_source, std::string_view fragment_source)
{
    const auto vertex_shader = compile_vertex_shader(vertex_source);
    const auto fragment_shader = compile_fragment_shader(fragment_source);
    if (not vertex_shader or not fragment_shader)
    {
        return nullptr;
    }
    return link_shader(*vertex_shader.get(), *fragment_shader.get());
}
