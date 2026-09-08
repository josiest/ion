#include "ion/konbu/filesystem.hpp"
#include <SDL3/SDL_filesystem.h>
#include <filesystem>
#include <fstream>
#include <sstream>

std::optional<std::string> ion::internal::read_file(std::string_view path)
{
    static const std::filesystem::path base_dir = SDL_GetBasePath();
    std::optional<std::string> source;
    if (std::ifstream file_stream{ base_dir/path })
    {
        std::stringstream source_stream;
        source_stream << file_stream.rdbuf();
        source = source_stream.str();
    }
    return source;
}
