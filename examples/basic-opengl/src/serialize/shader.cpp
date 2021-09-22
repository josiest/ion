#include "serialize/shader.hpp"

#include <vector>
#include <string>

#include <filesystem>
#include <algorithm>
#include <functional>

#include <fstream>
#include <utility>

namespace fs = std::filesystem;
namespace views = std::views;
namespace ranges = std::ranges;

// some aliases for convenience
using stream_pair = std::pair<fs::path, std::ifstream>;
using source_map = ion::shader_program::source_map;
auto const & extensions = ion::standard_shader_extensions;

std::vector<stream_pair>
stream_shader_sources(std::string const & shader_name,
                      fs::path const & shader_dir) noexcept
{
    // an iterator of all paths in the shader directory
    std::vector<fs::path> all_paths(fs::directory_iterator{shader_dir}, {});

    // determine if a path is a source file for this shader
    auto is_shader_source = [&shader_name](auto const & path) {
        return path.stem().string() == shader_name and
               extensions.contains(path.extension());
    };

    // load a path into an ifstream
    auto load_path = [](auto const & path) {
        return std::make_pair(path, std::ifstream{path});
    };

    // only load the shaders with valid extensions
    auto sources = all_paths | views::filter(is_shader_source)
                             | views::transform(load_path);

    return std::vector<stream_pair>(sources.begin(), sources.end());
}

source_map load_shader_sources(std::vector<stream_pair> & streams) noexcept
{
    // convert a path-stream pair into a type-source pair
    auto as_source_pair = [](auto & pair) {
        std::string source(std::istreambuf_iterator<char>(pair.second), {});
        GLenum shader_type = extensions.at(pair.first.extension());
        return std::make_pair(shader_type, std::move(source));
    };

    // load all the path-stream pairs into a source map
    source_map sources;
    auto into_sources = std::inserter(sources, sources.end());
    ranges::transform(streams, into_sources, as_source_pair);

    return sources;
}
