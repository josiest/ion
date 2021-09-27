#define GL_GLEXT_PROTOTYPES 1
#include <SDL_opengl.h>
#include <SDL_opengl_glext.h>

#include "ion/gl/shader_program.hpp"

#include <unordered_map>
#include <initializer_list>
#include <utility>

#include <filesystem>
#include <system_error>
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

// compile a shader from a type-source pair
decltype(auto) _compile_shader_source(std::pair<GLenum, std::string> && pair)
{
    return shader{pair.first, std::move(pair.second)};
}

// convert a shader to a type-unique_shader pair
decltype(auto) _to_pair(shader && s) noexcept
{
    GLuint const type = s.type();
    return std::make_pair(type, std::make_unique<shader>(std::move(s)));
}

shader_program::shader_program(std::string const & vertex_source) noexcept
    : _id{glCreateProgram()}
{
    // compile the given source as a vertex shader
    auto source_pair = std::make_pair(GL_VERTEX_SHADER, vertex_source);
    _shaders.insert(_to_pair(_compile_shader_source(std::move(source_pair))));

    // then make sure things are ok
    _validate_program() and _validate_shaders() and _link_shaders();
}

shader_program::shader_program(std::initializer_list<fs::path> paths) noexcept
    : _id{glCreateProgram()}
{
    // compile each shader
    auto into_shaders = std::inserter(_shaders, _shaders.end());
    ranges::transform(paths, into_shaders, &_to_pair);

    // then make sure things are ok
    _validate_program() and _validate_shaders() and _link_shaders();
}

shader_program::shader_program(std::string const & name,
                               fs::path const & dir) noexcept
    : _id{glCreateProgram()}
{
    // get all the paths in the given directory
    std::error_code ec;
    std::vector<fs::path> paths(fs::directory_iterator(dir, ec), {});

    // determine if a path is a valid source file
    auto is_source = [&name, &ec](auto const & path) {
        return fs::is_regular_file(path, ec) and path.stem().string() == name
           and standard_shader_extensions.contains(path.extension());
    };

    // abort if an os-call failed
    if (not _validate_error_code(ec)) return;

    // compile all paths that are valid sources with the specified name
    auto sources = paths | views::filter(is_source);
    auto into_shaders = std::inserter(_shaders, _shaders.end());
    ranges::transform(sources, into_shaders, &_to_pair);

    // then make sure everything is ok
    _validate_error_code(ec) and _validate_program() and _validate_shaders()
                             and _link_shaders();
}

shader_program::shader_program(
        std::unordered_map<GLenum, std::string> const & sources) noexcept

    : _id{glCreateProgram()}
{
    // compile all shader sources
    auto compiled_shaders = sources | views::transform(&_compile_shader_source);
    auto into_shaders = std::inserter(_shaders, _shaders.end());
    ranges::transform(compiled_shaders, into_shaders, &_to_pair);

    // then make sure things are ok
    _validate_program() and _validate_shaders() and _link_shaders();
}

shader_program::shader_program(shader_program && temp)
    : _id{temp._id}, _shaders{std::move(temp._shaders)},
      _error{std::move(temp._error)}
{
    temp._id = 0;
    temp._error = "shader program moved to another object";
}

bool shader_program::operator!() const noexcept
{
    return not glIsProgram(_id);
}

bool shader_program::_validate_error_code(std::error_code const & ec) noexcept
{
    // clean up if the error code has been set
    if (ec) {
        _error = ec.message();
        _clean_up();
        return false;
    }
    return true;
}

bool shader_program::_validate_program() noexcept
{
    if (not glIsProgram(_id)) {
        _error = "program couldn't be created";
        _clean_up();
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
        _clean_up();
        return false;
    }
    // make sure the program has a vertex shader
    auto const vertex_search = _shaders.find(GL_VERTEX_SHADER);
    if (_shaders.end() == vertex_search) {
        _error = "program must at least have a vertex shader";
        _clean_up();
        return false;
    }
    return true;
}

decltype(auto) shader_program::_detach_shader() noexcept
{
    return [this](auto const & pair) {
        glDetachShader(_id, *pair.second);
    };
}

decltype(auto) shader_program::_delete_shader() noexcept
{
    return [](auto const & pair) {
        glDeleteShader(*pair.second);
    };
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
        _clean_up();
        return false;
    }
    // otherwise, free any unused references to the shaders
    ranges::for_each(_shaders, _delete_shader());
    return true;
}

void shader_program::_clean_up() noexcept
{
    ranges::for_each(_shaders, _detach_shader());
    glDeleteProgram(_id);
    _id = 0;
    _shaders.clear();
}

shader_program::~shader_program()
{
    _clean_up();
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
