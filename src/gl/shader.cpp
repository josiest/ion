#define GL_GLEXT_PROTOTYPES 1
#include <SDL_opengl.h>
#include <SDL_opengl_glext.h>

#include "ion/gl/shader.hpp"

#include <string>
#include <unordered_map>

#include <filesystem>
#include <system_error>

#include <cstdio>
#include <cstring>
#include <cstdint>

namespace ion {

namespace fs = std::filesystem;

shader::shader(GLenum shader_type, std::string const & source) noexcept

    : _id{glCreateShader(shader_type)}
{
    if (not _validate_shader()) {
        return;
    }
    if (not _compile(source)) {
        return;
    }
}

shader::shader(fs::path const & path) noexcept
    : _id{0}
{
    // make sure the path is a file and not a directory
    std::error_code ec;
    if (fs::is_directory(path, ec)) {
        _error = path.string() + " is a directory";
        return;
    }
    // there might have been some os error in the process
    if (ec) {
        _error = ec.message();
        return;
    }

    // try to find the path extension in the map of valid shader extensions
    auto const extension_search = 
        standard_shader_extensions.find(path.extension().string());

    // if it was found, create the shader
    if (standard_shader_extensions.end() == extension_search) {
        _error = path.extension().string() + " isn't a valid shader extension";
        return;
    }

    // try to load the shader source
    auto fp = std::fopen(path.c_str(), "r");

    // clean up if the file couldn't load
    if (not fp) {
        _error = std::strerror(errno);
        return;
    }

    // read the file
    std::fseek(fp, 0, SEEK_END);
    std::string source(std::ftell(fp), '\0');

    std::rewind(fp);
    std::fread(source.data(), sizeof(char), source.size(), fp);

    // clean up if reading the file failed
    if (std::ferror(fp)) {
        _error = std::strerror(errno);
        std::fclose(fp);
        return;
    }

    // make sure to release the file resource
    std::fclose(fp);

    if (source.empty()) {
        _error = "shader source is empty";
        return;
    }

    // finally, create the shader and compile it
    _id = glCreateShader(extension_search->second);
    if (not _validate_shader()) {
        return;
    }
    if (not _compile(source)) {
        return;
    }
}

shader::~shader()
{
    // delete the shader if it's still valid
    if (_id != 0 and glIsShader(_id)) {
        glDeleteShader(_id);
    }
    _id = 0;
}

bool shader::_validate_shader() noexcept
{
    // glCreateShader might not have been able to create the shader
    if (_id == 0) {
        _error = "Couldn't create the shader object";
        return false;
    }
    // an invalid shader type might have been given
    if (_id == GL_INVALID_ENUM) {
        _error = "Shader was given an invalid type";
        _id = 0;
        return false;
    }
    return true;
}

bool shader::_compile(std::string const & source) noexcept
{
    // link the source code to the shader id
    GLchar const * all_sources = source.c_str();
    glShaderSource(_id, 1, static_cast<GLchar const **>(&all_sources), nullptr);

    // try to compile the shader
    glCompileShader(_id);
    GLint is_compiled = GL_FALSE;
    glGetShaderiv(_id, GL_COMPILE_STATUS, &is_compiled);

    // if the shader couldn't compile, clean up and set the error message
    if (is_compiled != GL_TRUE) {
        _error = gl_shader_error(*this) + source;
        glDeleteShader(_id); _id = 0;
        return false;
    }
    return true;
}

std::string gl_shader_error(shader const & s) noexcept
{
    if (not glIsShader(s)) {
        return "Couldn't get shader error because shader id is invalid";
    }
    // get the length of the shader info message
    int len;
    glGetShaderiv(s, GL_INFO_LOG_LENGTH, &len);

    // get the error message
    char log[len];
    glGetShaderInfoLog(s, len, nullptr, log);
    return log;
}

std::string shader_name(GLenum shader_type) noexcept
{
    // map known shader-types to their names
    static std::unordered_map<GLenum, std::string> as_string{
        {GL_VERTEX_SHADER, "vertex"}, {GL_FRAGMENT_SHADER, "fragment"}
    };
    // return the correct name if it exists
    auto search = as_string.find(shader_type);
    if (search != as_string.end()) {
        return search->second;
    }
    // otherwise return "undefined"
    return "undefined";
}
}
