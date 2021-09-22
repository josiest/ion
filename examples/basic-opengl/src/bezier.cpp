#include "bezier.hpp"
#include "serialize/shader.hpp"

#include <ion/ion.hpp>
#include <SDL2/SDL.h>

#include <iostream>
#include <sstream>
#include <string>

#include <numeric>
#include <fstream>
#include <filesystem>

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
        // glew erorr strings are unsigned chars - reinterpret to c-string
        _error = "Glew couldn't initialize: "s +
            reinterpret_cast<const char *>(glewGetErrorString(glew_error));
        return;
    }

    // try to load the shader sources into filestreams
    auto simple_sources = serialize::load_shader_sources("simple", "../shaders");

    // if any of the shader sources fail to load print out which ones
    if (ranges::any_of(simple_sources, serialize::load_failed)) {

        // initialize the error with a helpful message
        std::stringstream message;
        message << "The following shader files failed to load:" << std::endl;

        // put the error message for each file on a new line
        auto messages = simple_sources | views::filter(serialize::load_failed)
                                       | views::transform(serialize::load_error);
        ranges::copy(messages, std::ostream_iterator<std::string>(message, "\n"));
        _error = message.str();
        return;
    }

    // convert the load-info to a source map
    ion::shader_program::source_map simple_shader;
    auto into_shader = std::inserter(simple_shader, simple_shader.end());
    ranges::transform(simple_sources, into_shader, serialize::load_source);

    // failure if the shader sources fail to compile
    _shaders.push_back(std::make_unique<ion::shader_program>(
                       std::move(simple_shader))
    );
    if (_shaders.back()->bad()) {
        _error = _shaders.back()->error();
    }
}
