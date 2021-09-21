#include "serialize/shader.hpp"

#include <vector>
#include <string>

#include <filesystem>
#include <algorithm>
#include <functional>

#include <fstream>
#include <utility>

namespace fs = std::filesystem;
namespace ranges = std::ranges;

std::vector<fs::path>
shader_filenames(std::string const & shader_name,
                 fs::path const & shader_dir) noexcept
{
    // an iterator of all paths in the shader directory
    auto all_paths = fs::directory_iterator{shader_dir};

    // determine if a path is a source file for this shader
    auto is_shader_source = [&shader_name](auto const & path) {
        return fs::path{path}.stem().string() == shader_name;
    };

    // copy the paths that are source files for this shader
    std::vector<fs::path> shader_paths;
    ranges::copy_if(all_paths, std::back_inserter(shader_paths), is_shader_source);

    return shader_paths;
}

ion::shader_program::source_map
shader_sources(std::vector<fs::path> const & source_paths) noexcept
{
    auto const & extensions = ion::standard_shader_extensions;

    // determine if a path has a valid shader extension
    auto has_valid_extension = [&extensions](auto const & path) {
        return extensions.contains(path.extension());
    };
    // only load the shaders with valid extensions
    // use a reference to avoid copying
    std::vector<std::reference_wrapper<fs::path const>> valid_paths;
    auto into_valid = std::back_inserter(valid_paths);
    ranges::copy_if(source_paths, into_valid, has_valid_extension);

    // load the source code of a path
    auto load_source = [&extensions](fs::path const & path) {
        std::ifstream stream{path};
        std::string source(std::istream_iterator<char>{stream}, {});
        return std::make_pair(extensions.at(path.extension()), std::move(source));
    };

    // load the paths
    ion::shader_program::source_map sources;
    auto into_sources = std::inserter(sources, sources.end());
    ranges::transform(valid_paths, into_sources, load_source);

    return sources;
}
