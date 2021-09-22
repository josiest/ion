#pragma once

#include "ion/gl/shader.hpp"

#include <string>
#include <memory>
#include <unordered_map>
#include <utility>

#include <list>
#include <algorithm>
#include <numeric>

namespace ion {

/** Map file extensions to their respective shader-type */
inline std::unordered_map<std::string, GLenum> const standard_shader_extensions{
    {".vert", GL_VERTEX_SHADER}, {".tesc", GL_TESS_CONTROL_SHADER},
    {".tese", GL_TESS_EVALUATION_SHADER}, {".geom", GL_GEOMETRY_SHADER},
    {".frag", GL_FRAGMENT_SHADER}, {".comp", GL_COMPUTE_SHADER}
};

/**
 * A resource manager for an OpenGL shader program
 */
class shader_program {
public:
    /** A type alias of a map of shader-types to their source code */
    using source_map = std::unordered_map<GLenum, std::string>;

    // delete unwanted implicit constructors
    shader_program() = delete;
    shader_program(shader_program const &) = delete;

    /**
     * Create a shader program from only a vertex source
     * \param vertex_source the source code of the vertex shader
     */
    explicit inline shader_program(std::string const & vertex_source) noexcept;

    /**
     * Create a shader program from a map of shader sources
     * \param sources a map of shader-types to their source code
     */
    inline shader_program(source_map const & sources) noexcept;

    inline ~shader_program();

    /** The id of this program */
    inline GLuint id() const noexcept { return _id; }

    /** Determine if the shader program initialized properly */
    inline bool good() const noexcept { return _id != 0; }

    /** Determine if the shader program encountered a nonrecoverable error */
    inline bool bad() const noexcept { return _id == 0; }

    /** Explains why the shader program failed to initialize */
    inline std::string error() const noexcept { return _error; }
private:
    /** A type alias of a map of shader-types to their owning pointers */
    using shader_map = std::unordered_map<GLenum, std::unique_ptr<shader>>;

    GLuint _id;
    shader_map _shaders;

    std::string _error;
};

/** Get the error associated with a shader program */
inline std::string shader_program_error(shader_program const & program) noexcept
{
    if (not glIsProgram(program.id())) {
        return "Couldn't get shader program error because its id is invalid";
    }
    // get the length of the shader info message
    int len;
    glGetProgramiv(program.id(), GL_INFO_LOG_LENGTH, &len);

    // get the error message
    char log[len];
    glGetProgramInfoLog(program.id(), len, nullptr, log);

    return log;
}

shader_program::shader_program(std::string const & vertex_source) noexcept
    : shader_program({{GL_VERTEX_SHADER, vertex_source}})
{}

shader_program::shader_program(shader_program::source_map const & sources) noexcept
    : _id{glCreateProgram()}
{
    namespace ranges = std::ranges;
    namespace views = std::views;

    if (_id == 0) {
        _error = "An error occured while creating the shader program!";
        return;
    }

    // failure if no vertex shader supplied
    if (not sources.contains(GL_VERTEX_SHADER)) {
        _error = "A shader program must at least have a vertex shader";
        glDeleteProgram(_id); _id = 0; // clean up
        return;
    }

    // compile a shader and project it to a type-shader pair
    auto compile_shader = [](auto const & pair) {
        auto const & [shader_type, source] = pair;
        return std::make_pair(shader_type,
                              std::make_unique<shader>(shader_type, source));
    };
    // convert all the shader sources into compiled shaders
    auto into_shaders = std::inserter(_shaders, _shaders.end());
    ranges::transform(sources, into_shaders, compile_shader);

    // determine if a shader failed to compile
    auto compile_failed = [](auto const & pair) { return pair.second->bad(); };
    // get the error message for a failed shader
    auto compile_error = [](auto const & pair) {
        return shader_name(pair.first) + " shader failed to compile: "
                                       + pair.second->error();
    };

    // get all the error message of the shaders that failed to compile
    auto compile_errors = _shaders | views::filter(compile_failed)
                                   | views::transform(compile_error);

    // failure if any shaders couldn't compile
    if (ranges::any_of(_shaders, compile_failed)) {

        // reduce the errors into a single string
        _error = std::accumulate(compile_errors.begin(), compile_errors.end(),
                                 std::string{});

        // clean up
        _shaders.clear();
        glDeleteProgram(_id); _id = 0;
        return;
    }

    // try to link the program
    ranges::for_each(_shaders, [this](auto const & pair) {
        glAttachShader(_id, pair.second->id());
    });
    glLinkProgram(_id);

    // failure if the program couldn't link
    GLint is_linked = GL_FALSE;
    glGetProgramiv(_id, GL_LINK_STATUS, &is_linked);
    if (is_linked != GL_TRUE) {

        _error = shader_program_error(*this);

        // clean up
        _shaders.clear();
        glDeleteProgram(_id); _id = 0;
        return;
    }

    // free any unused references to the shaders
    ranges::for_each(_shaders, [](auto const & pair) {
        glDeleteShader(pair.second->id());
    });
}

shader_program::~shader_program()
{
    // release all shaders
    _shaders.clear();

    // destroy the program if it's valid
    if (_id != 0 and glIsProgram(_id)) {
        glDeleteProgram(_id); _id = 0;
    }
}
}
