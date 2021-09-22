#pragma once

#include "serialize/shader.hpp"

#include <GL/glew.h>
#include <GL/glu.h>

#include <ion/ion.hpp>
#include <ion/gl.hpp>

#include <unordered_map>
#include <vector>

#include <memory>
#include <cstdint>

class bezier {
public:
    bezier(std::uint32_t width, std::uint32_t height) noexcept;

    void run() noexcept;
    inline bool good() const noexcept { return _error.empty(); }
    inline bool bad() const noexcept { return not _error.empty(); }
    inline std::string error() const noexcept { return _error; }
private:
    ion::sdl_context _sdl;
    ion::glwindow _window;

    ion::event_system _events;

    std::vector<std::unique_ptr<ion::shader_program>> _shaders;
    std::string _error;
};
