#pragma once

#include <GL/glew.h>
#include <ion/gl.hpp>

#include <vector>
#include <utility>

#include <string>
#include <fstream>
#include <filesystem>

/** Get the file names associated with a shader name relative to a path */
std::vector<std::pair<std::filesystem::path, std::ifstream>>
stream_shader_sources(std::string const & shader_name,
                      std::filesystem::path const & shader_dir) noexcept;

/** Convert a vector path-stream pairs into shader source-map */
ion::shader_program::source_map
load_shader_sources(std::vector<std::pair<std::filesystem::path,
                                          std::ifstream>> & streams) noexcept;
