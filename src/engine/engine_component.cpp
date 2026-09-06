#include "ion/engine/engine_component.hpp"

ion::window_component::window_component(std::string_view title, int width, int height, SDL_WindowFlags flags)
    : window_handle(create_window(title, width, height, flags))
{
}

SDL_Window * ion::window_component::get() const
{
    return window_handle.get();
}

ion::opengl_component::opengl_component(SDL_Window * window)
    : gl_context(init_opengl(window))
{
}

ion::renderer_component::renderer_component(SDL_Window * window)
    : renderer(create_renderer(window))
{
}
