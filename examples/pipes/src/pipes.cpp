#include "grid.hpp"

#include <ion/ion.hpp>
#include <SDL2/SDL.h>
#include <string>
#include <unordered_map>

enum class Tile {
    L, T, I, U
};

class Pipes {
public:
    Pipes(int width, int height)
        : _window{ion::basic_blit_window("Pipes", width, height)},
          _grid{0, height, width, height, 50}
    {
        _handler.subscribe(SDL_QUIT, &ion::input::quit_on_event);
        _images.try_emplace(Tile::L,
                ion::load_bitmap_image("../resources/l-piece.bmp")
        );
        _images.try_emplace(Tile::T,
                ion::load_bitmap_image("../resources/t-piece.bmp")
        );
        _images.try_emplace(Tile::I,
                ion::load_bitmap_image("../resources/bar-piece.bmp")
        );
        _images.try_emplace(Tile::U,
                ion::load_bitmap_image("../resources/end-piece.bmp")
        );
    }
    ~Pipes() { _images.clear(); SDL_Quit(); }

    void render(ion::Window & window)
    {
        auto screen = window.sdl_surface();
        SDL_FillRect(screen, nullptr, SDL_MapRGB(screen->format, 0, 0, 0));

        auto image = _images.at(Tile::T).sdl_surface();
        SDL_Rect grid_square = _grid.rect(0, 0);

        SDL_BlitScaled(image, nullptr, screen, &grid_square);
        SDL_UpdateWindowSurface(window.sdl_window());
    }

    void run()
    {
        while (!ion::input::has_quit()) {
            _handler.process_queue();
            render(_window);
        }
    }
private:
    ion::Window _window;
    std::unordered_map<Tile, ion::Bitmap> _images;
    ion::EventHandler _handler;
    Grid _grid;
};

int main()
{
    Pipes game(800, 600);
    game.run();
}
