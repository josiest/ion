#include "ion/photon/shader.hpp"

#include "ion/konbu/filesystem.hpp"
#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_log.h>
#include <yaml-cpp/yaml.h>

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
    const char * const raw_source = source_code.data();
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
    return shader_component(new GLuint(shader_id));
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

void ion::shader_handle::use_program() const
{
    glUseProgram(id);
}

std::optional<ion::shader_data> ion::load_shader_data(const std::filesystem::path & path)
{
    namespace fs = std::filesystem;
    SDL_Log("Reading shader from file \"%s\"\n", path.generic_string().c_str());

    const YAML::Node shader_config = YAML::LoadFile(path.generic_string());
    if (not shader_config.IsDefined() or not shader_config.IsMap()) { return std::nullopt; }

    bool success = true;
    const YAML::Node name_config = shader_config["name"];
    if (not name_config.IsDefined() or not name_config.IsScalar())
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Shader has no name, or name isn't a string\n");
        success = false;
    }

    const YAML::Node vertex_config = shader_config["vertex"];
    if (not vertex_config.IsDefined() or not vertex_config.IsScalar())
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Shader has no vertex path, or vertex path isn't a string\n");
        success = false;
    }

    const YAML::Node fragment_config = shader_config["fragment"];
    if (not fragment_config.IsDefined() or not fragment_config.IsScalar())
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Shader has no fragment path, or fragment path isn't a string\n");
        success = false;
    }

    if (not success)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to read shader\n");
        return std::nullopt;
    }

    SDL_Log("  Read shader \"%s\" successfully\n", name_config.Scalar().c_str());
    return shader_data
    {
        .name = name_config.Scalar(),
        .vertex_path = vertex_config.Scalar(),
        .fragment_path = fragment_config.Scalar()
    };
}

ion::shader_program ion::compile_shader(const shader_data & settings, const std::filesystem::path & shader_root_dir)
{
    namespace fs = std::filesystem;

    const auto vertex_filepath = shader_root_dir/settings.vertex_path;
    if (not fs::exists(vertex_filepath))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "couldn't find vertex shader \"%s\"\n",
                     vertex_filepath.generic_string().c_str());
        return nullptr;
    }
    const auto vertex_source = internal::read_file(vertex_filepath.generic_string());

    const auto fragment_filepath = shader_root_dir/settings.fragment_path;
    if (not fs::exists(fragment_filepath))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "couldn't find fragment shader \"%s\"\n",
                     fragment_filepath.generic_string().c_str());
        return nullptr;
    }
    const auto fragment_source = internal::read_file(fragment_filepath.generic_string());
    if (not vertex_source or not fragment_source) { return nullptr; }
    return compile_shader(*vertex_source, *fragment_source);
}
