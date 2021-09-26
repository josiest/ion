#include <ion/ion.hpp>
#include <ion/gl.hpp>

#include <SDL.h>

#include "bezier.hpp"
#include "systems.hpp"

#include <iostream>
#include <sstream>

#include <string>
#include <filesystem>

#include <ranges>
#include <algorithm>

// aliases and literals
using namespace std::string_literals;
namespace ranges = std::ranges;
namespace views = std::views;
namespace fs = std::filesystem;

int main()
{
    // initialize the example
    bezier example{800, 600};

    // crash if initialization failed
    if (not example) {
        std::cerr << example.error() << std::endl;
        return 1;
    }
    // otherwise run the program
    example.run();
}

void bezier::run() noexcept
{
    while (not ion::input::has_quit()) {

        // handle system events
        _events.process_queue();

        // render
        systems::render(_window, _point_shader);
        SDL_GL_SwapWindow(_window);
    }
}

bezier::bezier(std::uint32_t width, std::uint32_t height) noexcept

    : _window{"Bezier example", width, height},
      _shader_path{"../shaders"},
      _point_shader("point", _shader_path)
{
    _events.subscribe(SDL_QUIT, &ion::input::quit_on_event);

    // failure if sdl resources failed to initialize
    if (not _sdl) {
        _error = "SDL couldn't initialize: "s + _sdl.error();
        return;
    }
    if (not _window) {
        _error = "Couldn't create a window: "s + _window.error();
        return;
    }

    // failure if shader couldn't be created
    if (not _point_shader) {
        _error = "Couldn't create point shader: "s + _point_shader.error();
        return;
    }
    if (not _vao) {
        _error = "Couldn't create a vertex array object: " + _vao.error();
    }
}
