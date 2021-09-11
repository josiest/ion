#include <ion/ion.hpp>
#include <SDL2/SDL.h>
#include <string>

class Pipes {
public:
    Pipes(int width, int height)
        : _title{"Pipes"}, _width{width}, _height{height}, _handler{}
    {
        _handler.subscribe(SDL_QUIT, &ion::input::quit_on_event);
    }
    ~Pipes() { SDL_Quit(); }

    void render(ion::Window & window)
    {
        auto screen = window.sdl_surface();
        SDL_FillRect(screen, nullptr, SDL_MapRGB(screen->format, 0, 0, 0));
    }

    void run()
    {
        auto window = ion::basic_blit_window(_title, _width, _height);
        while (!ion::input::has_quit()) {
            _handler.process_queue();
            render(window);
        }
    }
private:
    std::string _title;
    int _width, _height;
    ion::EventHandler _handler;
};

int main()
{
    Pipes game(800, 600);
    game.run();
}
