#include "bezier.hpp"

#include <ion/ion.hpp>
#include <ion/gl.hpp>

#include <SDL.h>

#include <iostream>
#include <sstream>
#include <string>

#include <ranges>
#include <algorithm>

// aliases and literals
using namespace std::string_literals;
namespace ranges = std::ranges;
namespace views = std::views;

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
    // busy loop until the user quits
    while (not ion::input::has_quit()) {
        _events.process_queue();
    }
}

bezier::bezier(std::uint32_t width, std::uint32_t height) noexcept

    : _window{"Bezier example", width, height},
      _bezier_shader("bezier", "../shaders")
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
    if (not _bezier_shader) {
        _error = "Couldn't create bezier shader: "s + _bezier_shader.error();
        return;
    }
}
