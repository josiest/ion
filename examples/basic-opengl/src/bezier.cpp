#include "bezier.hpp"
#include "serialize/shader.hpp"

#include <ion/ion.hpp>
#include <SDL2/SDL.h>

#include <iostream>
#include <sstream>
#include <string>

using namespace std::string_literals;
namespace ranges = std::ranges;

int main()
{
    // initialize the example
    bezier example{800, 600};

    // crash if initialization failed
    if (example.bad()) {
        std::cout << example.error();
        return 1;
    }
    example.run();
}

void bezier::run() noexcept
{
    // busy loop until the user quits
    while (!ion::input::has_quit()) {
        _events.process_queue();
    }
}

bezier::bezier(std::uint32_t width, std::uint32_t height) noexcept

    : _window{"Bezier example", width, height}
{
    _events.subscribe(SDL_QUIT, &ion::input::quit_on_event);
    // failure if sdl resources failed to initialize
    if (_sdl.bad()) {
        _error = "SDL couldn't initialize: "s + _sdl.error();
        return;
    }
    if (_window.bad()) {
        _error = "Window couldn't initialize: "s + _window.error();
        return;
    }

    // intialize glew
    glewExperimental = GL_TRUE;
    GLenum const glew_error = glewInit();

    // failure if flew couldn't initialize
    if (glew_error != GLEW_OK) {
        _error = "Glew couldn't initialize: "s +
            reinterpret_cast<const char *>(glewGetErrorString(glew_error));
        return;
    }

    auto simple_paths = shader_filenames("simple", "../shaders");
    auto simple_sources = shader_sources(std::move(simple_paths));

    _shaders.push_back(std::make_unique<ion::shader_program>(
                       std::move(simple_sources)));

    if (_shaders.back()->bad()) {
        _error = _shaders.back()->error();
        return;
    }
}
