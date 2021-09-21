#pragma once

#include <string>

namespace ion {

class shader {
public:
    // delete unwanted implicit constructors
    shader() = delete;
    shader(shader const &) = delete;

    /**
     * Create a shader from source code
     *
     * \param shader_type the type of shader
     * \param source the shader source
     */
    shader(GLenum shader_type, std::string const & source) noexcept;
    ~shader();

    /** The GL id of this shader */
    inline GLuint id() const noexcept { return _id; }

    /** Determine if the shader initialized properly */
    inline bool is_ok() const noexcept { return _id != 0; }

    /** Explains why the shader failed to initialize */
    inline std::string error() const noexcept { return _error; }
private:
    GLuint _id;
    std::string _error;
};

std::string shader_error(shader const & s)
{
    if (not glIsShader(s.id())) {
        return "Couldn't get shader error because shader id is invalid";
    }
    // get the length of the shader info message
    int len;
    glGetShaderiv(s.id(), GL_INFO_LOG_LENGTH, &len);

    // get the error message
    std::string message; message.reserve(len);
    glGetShaderInfoLog(s.id(), len, nullptr, message.data());

    return message;
}

shader::shader(GLenum shader_type, std::string const & source) noexcept

    : _id{glCreateShader(shader_type)}
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
        _error = shader_error(*this);
        glDeleteShader(_id); _id = 0;
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
}
