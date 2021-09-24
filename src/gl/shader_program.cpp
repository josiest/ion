#define GL_GLEXT_PROTOTYPES 1
#include <SDL_opengl.h>
#include <SDL_opengl_glext.h>

#include "ion/gl/shader_program.hpp"

#include <unordered_map>
#include <initializer_list>
#include <utility>

#include <filesystem>
#include <memory>
#include <string>

#include <algorithm>
#include <numeric>
#include <ranges>

// aliases and namespaces for convenience
namespace ranges = std::ranges;
namespace views = std::views;
namespace fs = std::filesystem;

using namespace std::string_literals;

namespace ion {

shader_program::shader_program(std::string const & vertex_source) noexcept
    : _id{glCreateProgram()}
{
    _shaders.try_emplace(GL_VERTEX_SHADER,
                         std::make_unique<shader>(GL_VERTEX_SHADER, vertex_source));

    _validate_program() and _validate_shaders() and _link_shaders();
}

shader_program::shader_program(std::initializer_list<fs::path> paths) noexcept
    : _id{glCreateProgram()}
{
    // add all specified shaders to the program
    for (auto const & path : paths) {
        shader s{path};
        auto const type = s.type();
        _shaders.try_emplace(type, std::make_unique<shader>(std::move(s)));
    }
    _validate_program() and _validate_shaders() and _link_shaders();
}

bool shader_program::_validate_program() noexcept
{
    if (_id == 0) {
        _error = "program couldn't be created";
        return false;
    }
    return true;
}

bool shader_program::_validate_shaders() noexcept
{
    // make sure the shaders compiled correctly
    auto shader_is_bad = [](auto const & pair) {
        return not *pair.second;
    };
    auto shader_error = [](auto const & pair) { return pair.second->error(); };

    if (ranges::any_of(_shaders, shader_is_bad)) {

        // get the error message of each failed shader
        auto errors = _shaders | views::filter(shader_is_bad)
                               | views::transform(shader_error);

        // and compile them into one long message
        _error = "One or more shaders failed to compile:"s
               + std::accumulate(errors.begin(), errors.end(), "\n"s);
        // then clean up
        // TODO: support non-vertex shader-failure as a recoverable failure
        _shaders.clear(); glDeleteProgram(_id); _id = 0;
        return false;
    }
    // make sure the program has a vertex shader
    auto const vertex_search = _shaders.find(GL_VERTEX_SHADER);
    if (_shaders.end() == vertex_search) {
        _error = "program must at least have a vertex shader";
        _shaders.clear(); glDeleteProgram(_id); _id = 0;
        return false;
    }
    return true;
}

bool shader_program::_link_shaders() noexcept
{
    // try to link the shaders to the program
    ranges::for_each(_shaders, [this](auto const & pair) {
        glAttachShader(_id, *pair.second);
    });
    glLinkProgram(_id);

    // check to see if the program linked properly
    GLint is_linked = GL_FALSE;
    glGetProgramiv(_id, GL_LINK_STATUS, &is_linked);

    // set the error and clean up if not
    if (is_linked != GL_TRUE) {
        _error = gl_shader_program_error(*this);
        _shaders.clear(); glDeleteProgram(_id); _id = 0;
        return false;
    }
    // otherwise, free any unused references to the shaders
    ranges::for_each(_shaders, [](auto const & pair) {
        glDeleteShader(*pair.second);
    });
    return true;
}

//shader_program::shader_program(shader_program::source_map const & sources) noexcept
//    : _id{glCreateProgram()}
//{
//
//    if (not _validate_program()) {
//        return;
//    }
//
//    // compile a shader and project it to a type-shader pair
//    auto compile_shader = [](auto const & pair) {
//        auto const & [shader_type, source] = pair;
//        return std::make_pair(shader_type,
//                              std::make_unique<shader>(shader_type, source));
//    };
//    // convert all the shader sources into compiled shaders
//    auto into_shaders = std::inserter(_shaders, _shaders.end());
//    ranges::transform(sources, into_shaders, compile_shader);
//
//    if (not _validate_shaders()) {
//        return;
//    }
//    if (not _link_shaders()) {
//        return;
//    }
//}

shader_program::~shader_program()
{
    // release all shaders
    _shaders.clear();

    // destroy the program if it's valid
    if (_id != 0 and glIsProgram(_id)) {
        glDeleteProgram(_id); _id = 0;
    }
}

/** Get the error associated with a shader program */
std::string gl_shader_program_error(shader_program const & program) noexcept
{
    if (not glIsProgram(program)) {
        return "Couldn't get program error because program is invalid";
    }
    // get the length of the shader info message
    GLint len;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
    if (len == 0) {
        return "Couldn't get program error because there is none";
    }

    // get the error message
    std::string error(len, '\0');
    glGetProgramInfoLog(program, error.size(), nullptr, error.data());

    return error;
}
}
