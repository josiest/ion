#pragma once

#include <GL/glew.h>
#include <ion/gl.hpp>

#include <vector>
#include <tuple>

#include <string>
#include <fstream>
#include <filesystem>

namespace serialize {

/** The result type of loading a file */
using load_info = std::tuple<std::filesystem::path, bool, std::string>;

/** Load a file */
load_info load_path(std::string const & path);

/** Get the file names associated with a shader name relative to a path */
std::vector<load_info>
load_shader_sources(std::string const & shader_name,
                    std::filesystem::path const & shader_dir) noexcept;

/** Determine if a load failed */
constexpr bool load_failed(load_info const & info) noexcept
{
    return not std::get<bool>(info);
}

/** Get a load error message while invalidating the result */
inline std::string load_error(load_info & info) noexcept
{
    using namespace std; using namespace std::filesystem;
    return get<path>(info).string() + ": " + move(get<string>(info));
}

/** Get a type-source pair while invalidating the load result */
std::pair<GLenum, std::string> load_source(load_info & info) noexcept;

}
