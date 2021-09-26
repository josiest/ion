#pragma once

#define GL_GLEXT_PROTOTYPES 1
#include <SDL_opengl.h>
#include <SDL_opengl_glext.h>

#include "ion/gl/shader.hpp"

#include <unordered_map>
#include <initializer_list>

#include <filesystem>
#include <system_error>

#include <string>
#include <memory>

namespace ion {

/**
 * A resource manager for an OpenGL shader program
 */
class shader_program {
public:
    /** A type alias of a map of shader-types to their source code */
    //using source_map = std::unordered_map<GLenum, std::string>;

    // delete unwanted implicit constructors
    shader_program() = delete;
    shader_program(shader_program const &) = delete;

    /**
     * Create a shader program from only a vertex source
     * \param vertex_source the source code of the vertex shader
     */
    explicit shader_program(std::string const & vertex_source) noexcept;

    /**
     * Create a shader program from a mapping of shader-types to their sources
     * \param sources the source mapping to compile
     */
    shader_program(
        std::unordered_map<GLenum, std::string> const & sources) noexcept;

    /**
     * Create a shader program from a list of source files
     * \param paths the source files of the shaders to use
     */
    shader_program(std::initializer_list<std::filesystem::path> paths) noexcept;

    /**
     * Create a shader program by finding its sources
     *
     * \param the name of the shader
     * \param dir the directory the shader sources are located in
     */
    shader_program(std::string const & name,
                   std::filesystem::path const & dir) noexcept;

    /**
     * Create a shader program from a map of shader sources
     * \param sources a map of shader-types to their source code
     */
    //inline shader_program(source_map const & sources) noexcept;
    ~shader_program();

    /** The id of this program */
    inline operator GLuint() const noexcept { return _id; }

    /** Determine if this shader program is not okay to use */
    bool operator!() const noexcept;

    /** The error if this shader program isn't okay to use */
    inline std::string const & error() const noexcept { return _error; }
private:
    /** A type alias of a map of shader-types to their owning pointers */
    using shader_map = std::unordered_map<GLenum, std::unique_ptr<shader>>;

    GLuint _id;
    shader_map _shaders;

    std::string _error;

    /** Return true if program was successfully created or set errors */
    bool _validate_program() noexcept;
    /** Return true if shaders are valid or set errors */
    bool _validate_shaders() noexcept;
    /** Return true if program successfully linked shaders or set errors */
    bool _link_shaders() noexcept;
    /** Retrun true if the error code is not set otherwise invalidate program */
    bool _validate_error_code(std::error_code const & ec) noexcept;
    /** Return a function that detaches a shader */
    decltype(auto) _detach_shader() noexcept;
    /** Return a function that deletes a shader */
    decltype(auto) _delete_shader() noexcept;
    /** Clean up the shader program to ensure no memory leaks */
    void _clean_up() noexcept;
};

/** Get the OpenGL error message associated with this program */
std::string gl_shader_program_error(shader_program const & program) noexcept;
}
