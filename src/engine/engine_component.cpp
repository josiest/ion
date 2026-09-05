#include "ion/engine/engine_component.hpp"

ion::window_component::window_component(std::string_view title, int width, int height, SDL_WindowFlags flags)
    : window(create_window(title, width, height, flags))
{
}

ion::opengl_component::opengl_component(SDL_Window * window)
    : gl_context(init_opengl(window))
{
}

ion::renderer_component::renderer_component(SDL_Window * window)
    : renderer(create_renderer(window))
{
}
