#pragma once

#include <SDL_opengl.h>

#include <string>
#include <filesystem>
#include <unordered_map>

namespace ion {

/** Map file extensions to their respective shader-type */
inline std::unordered_map<std::string, GLenum> const standard_shader_extensions{
    {".vert", GL_VERTEX_SHADER}, {".tesc", GL_TESS_CONTROL_SHADER},
    {".tese", GL_TESS_EVALUATION_SHADER}, {".geom", GL_GEOMETRY_SHADER},
    {".frag", GL_FRAGMENT_SHADER}, {".comp", GL_COMPUTE_SHADER}
};

/** A resource manager for an OpenGL shader */
class shader {
public:
    // delete unwanted implicit constructors and declare wanted ones
    shader() = delete;
    shader(shader const &) = delete;

    /**
     * Create a shader from source code
     *
     * \param shader_type the type of shader
     * \param source the shader source
     */
    shader(GLenum shader_type, std::string const & source) noexcept;

    /**
     * Create a shader by loading a source file
     *
     * \param path the path of the shader source
     */
    shader(std::filesystem::path const & path) noexcept;

    /** Create a shader from another temporary shader */
    shader(shader && s) noexcept;

    ~shader();

    /** The GL id of this shader */
    inline operator GLuint() const noexcept { return _id; }

    /** What type of shader this is */
    inline GLenum type() const noexcept { return _type; }

    /** Determine if this shader is okay to use */
    inline bool operator!() const noexcept
    {
        return not glIsShader(_id) or _id == 0;
    }

    /** The error if this shader isn't okay to use */
    inline std::string error() const noexcept { return _error; }
private:
    GLenum _type;
    GLuint _id;
    std::string _error;

    /** \return true if this shader id is valid or invalidate the shader */
    bool _validate_shader() noexcept;
    /** \return true if compiling succeeds or invalidate the shader */
    bool _compile(std::string const & source) noexcept;
};

/** The OpenGL error associated with a shader */
std::string gl_shader_error(shader const & s) noexcept;

/**
 * The string associated with a shader type
 *
 * \param shader_type the type to query
 * \return the name associated withe the shader type or "undefined"
 */
std::string shader_name(GLenum shader_type) noexcept;

}
