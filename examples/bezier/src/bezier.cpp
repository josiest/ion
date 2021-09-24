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

    : _window{"Bezier example", width, height}
{
    _events.subscribe(SDL_QUIT, &ion::input::quit_on_event);

    // failure if sdl resources failed to initialize
    if (not _sdl) {
        _error = "SDL couldn't initialize: "s + _sdl.error();
        return;
    }
    if (not _window) {
        _error = "Window couldn't initialize: "s + _window.error();
        return;
    }

    ion::shader vertex_shader{"../shaders/bezier.vert"};
    if (not vertex_shader) {
        _error = "Couldn't compile vertex shader: " + vertex_shader.error();
        return;
    }

    // try to load the shader sources into filestreams
    //auto bezier_sources = serialize::load_shader_sources("bezier", "../shaders");

    //// if any of the shader sources fail to load print out which ones
    //if (ranges::any_of(bezier_sources, serialize::load_failed)) {

    //    // initialize the error with a helpful message
    //    std::ostringstream message;
    //    message << "The following shader files failed to load:" << std::endl;

    //    // put the error message for each file on a new line
    //    auto messages = bezier_sources | views::filter(serialize::load_failed)
    //                                   | views::transform(serialize::load_error);
    //    ranges::copy(messages, std::ostream_iterator<std::string>(message, "\n"));
    //    _error = message.str();
    //    return;
    //}

    //// convert the load-info to a source map
    //ion::shader_program::source_map bezier_shader;
    //auto into_shader = std::inserter(bezier_shader, bezier_shader.end());
    //ranges::transform(bezier_sources, into_shader, serialize::load_source);

    //// failure if the shader sources fail to compile
    //_shaders.push_back(std::make_unique<ion::shader_program>(
    //                   std::move(bezier_shader))
    //);
    //if (_shaders.back()->bad()) {
    //    _error = _shaders.back()->error();
    //}
}
