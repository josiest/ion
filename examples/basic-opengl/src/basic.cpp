#include <GL/glew.h>
#include <GL/glu.h>
#include <ion/glewgl.hpp>

#include <ion/ion.hpp>
#include <SDL2/SDL.h>
#include <sstream>

int main()
{
    // intialize sdl before other sdl resources
    ion::sdl_context sdl;

    // create the event and input system
    ion::event_system events;
    events.subscribe(SDL_QUIT, &ion::input::quit_on_event);

    // create the window
    ion::glew_window window{"basic", 800, 600};

    // define the source
    std::stringstream vertex_source;
    vertex_source << "#version 330" << std::endl
                  << "in vec2 pos;"
                  << "int main() {"
                  << "    gl_Position = vec4(pos.x, pos.y, 0, 1);"
                  << "}";
    ion::shader vertex_shader{GL_VERTEX_SHADER, vertex_source.str()};

    // busy loop until the user quits
    while (!ion::input::has_quit()) {
        events.process_queue();
    }
}
