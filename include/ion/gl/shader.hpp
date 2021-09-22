#pragma once

#include <string>
#include <unordered_map>

namespace ion {

/**
 * A resource manager for an OpenGL shader
 */
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
    inline shader(GLenum shader_type, std::string const & source) noexcept;
    inline ~shader();

    /** The GL id of this shader */
    inline GLuint id() const noexcept { return _id; }

    /** Determine if the shader initialized properly */
    inline bool good() const noexcept { return _id != 0; }

    /** Determine if the shader encountered a nonrecoverable error */
    inline bool bad() const noexcept { return _id == 0; }

    /** Explains why the shader failed to initialize */
    inline std::string error() const noexcept { return _error; }
private:
    GLuint _id;
    std::string _error;
};

/** Get the error associated with a shader */
inline std::string shader_error(shader const & s) noexcept
{
    if (not glIsShader(s.id())) {
        return "Couldn't get shader error because shader id is invalid";
    }
    // get the length of the shader info message
    int len;
    glGetShaderiv(s.id(), GL_INFO_LOG_LENGTH, &len);

    // get the error message
    char log[len];
    glGetShaderInfoLog(s.id(), len, nullptr, log);
    return log;
}

/**
 * The string associated with a shader type
 * \param shader_type the type to query
 * \return the name associated withe the shader type or "undefined"
 */
inline std::string shader_string(GLenum shader_type) noexcept
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
        _error = shader_error(*this) + source;
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
