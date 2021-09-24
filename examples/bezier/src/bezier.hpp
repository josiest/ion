#pragma once

#include <ion/ion.hpp>
#include <ion/gl.hpp>

#include <cstdint>
#include <string>

class bezier {
public:
    bezier(std::uint32_t width, std::uint32_t height) noexcept;

    void run() noexcept;

    inline bool operator!() const noexcept { return not _error.empty(); }
    inline std::string const & error() const noexcept { return _error; }
private:
    ion::sdl_context _sdl;
    ion::glwindow _window;

    ion::event_system _events;
    std::string _error;
};
