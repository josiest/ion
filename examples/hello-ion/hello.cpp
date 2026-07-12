#include <ion/engine/sdl_resources.hpp>
#include <SDL3/SDL.h>

constexpr SDL_InitFlags INIT_FLAGS = SDL_INIT_VIDEO;
constexpr SDL_WindowFlags WINDOW_FLAGS = SDL_WINDOW_RESIZABLE;

constexpr std::string_view WINDOW_NAME = "Hello ion";
constexpr std::uint32_t SCREEN_WIDTH = 1920;
constexpr std::uint32_t SCREEN_HEIGHT = 1080;

int main(int argc, char * argv[])
{
    const auto sdl = init_sdl(INIT_FLAGS);
    if (not sdl)
    {
        SDL_Log("Couldn't initialize SDL: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }
    const auto window = SDL_CreateWindow(WINDOW_NAME.data(), SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_FLAGS);
    if (not window)
    {
        SDL_Log("Couldn't create window: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    bool has_quit = false;
    while (not has_quit)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_EVENT_QUIT:
                has_quit = true;
                break;
            default:
                break;
            }
        }
    }
    return EXIT_SUCCESS;
}
