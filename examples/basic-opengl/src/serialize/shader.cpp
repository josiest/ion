#include "serialize/shader.hpp"

#include <vector>
#include <string>

#include <filesystem>
#include <algorithm>
#include <functional>

#include <fstream>
#include <utility>

// aliases for convenience
namespace fs = std::filesystem;
namespace views = std::views;
auto const & extensions = ion::standard_shader_extensions;

load_info load_path(std::string const & path)
{
    // load the file into a stream
    std::ifstream stream{path};
    bool success = false;   // determines if the load was successful
    std::string source;     // the loaded source or an error message

    try {
        // cause an exception if the stream couldn't load
        stream.exceptions(std::ifstream::failbit);
        // read the file (causes an exception if the file is actually a directory)
        source.assign(std::istreambuf_iterator<char>(stream), {});
        success = true; // finally, signal success
    }
    catch (std::ios_base::failure & err) {
        source = strerror(errno); // get the error message set by the iostreams
    }
    return std::make_tuple(path, success, std::move(source));
}

std::vector<load_info>
load_shader_sources(std::string const & shader_name,
                    fs::path const & shader_dir) noexcept
{
    // an iterator of all paths in the shader directory
    std::vector<fs::path> all_paths(fs::directory_iterator{shader_dir}, {});

    // determine if a path is a source file for this shader
    auto is_shader_source = [&shader_name](auto const & path) {
        return path.stem().string() == shader_name and
               extensions.contains(path.extension());
    };

    // only load the shaders with valid extensions
    auto sources = all_paths | views::filter(is_shader_source)
                             | views::transform(load_path);

    return std::vector<load_info>(sources.begin(), sources.end());
}

std::pair<GLenum, std::string> load_source(load_info & info) noexcept
{
    using namespace std;
    GLenum shader_type = extensions.at(get<fs::path>(info).extension());
    return make_pair(shader_type, move(get<string>(info)));
}
