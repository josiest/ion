#pragma once

#include <ion/ion.hpp>
#include <ion/gl.hpp>

#include <cstdint>
#include <string>

#include <filesystem>

/** A class that manages game resources */
class bezier {
public:
    // delete unwanted implicit constructors
    bezier() = delete;
    bezier(bezier const &) = delete;

    /** Create a game with specified dimensions */
    bezier(std::uint32_t width, std::uint32_t height) noexcept;

    /** Run this game */
    void run() noexcept;

    /** Determine if this game is not okay to run */
    inline bool operator!() const noexcept { return not _error.empty(); }

    /** The error if this game isn't okay to run */
    inline std::string const & error() const noexcept { return _error; }
private:
    ion::sdl_context _sdl;
    ion::glwindow _window;

    std::filesystem::path _shader_path;
    ion::shader_program _point_shader;
    ion::vao _vao;

    ion::event_system _events;
    std::string _error;
};
