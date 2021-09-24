#pragma once

#include "ion/gl/shader.hpp"

#include <unordered_map>
#include <initializer_list>

#include <filesystem>
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
     * Create a shader program from a list of source files
     * \param paths the source files of the shaders to use
     */
    shader_program(std::initializer_list<std::filesystem::path> paths) noexcept;

    /**
     * Create a shader program from a map of shader sources
     * \param sources a map of shader-types to their source code
     */
    //inline shader_program(source_map const & sources) noexcept;
    ~shader_program();

    /** The id of this program */
    inline operator GLuint() const noexcept { return _id; }

    /** Determine if this shader program is not okay to use */
    inline bool operator!() const noexcept
    {
        return _id == 0 or not glIsProgram(_id);
    }

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
};

std::string gl_shader_program_error(shader_program const & program) noexcept;
}
