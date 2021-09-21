#pragma once

#include <GL/glew.h>
#include <ion/gl.hpp>

#include <vector>
#include <string>
#include <filesystem>

/** Get the file names associated with a shader name relative to a path */
std::vector<std::filesystem::path>
shader_filenames(std::string const & shader_name,
                 std::filesystem::path const & shader_dir) noexcept;

/** Load shader source files and map them according to their shader-type */
ion::shader_program::source_map
shader_sources(std::vector<std::filesystem::path> const & source_paths) noexcept;
