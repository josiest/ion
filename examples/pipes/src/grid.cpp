#include "grid.hpp"
#include <SDL2/SDL_rect.h>
#include <cmath>

Grid::Grid(int x, int y, int w, int h, int unit_size)
    : _bounds{x, y, w, h}, _unit_size{unit_size}
{}

SDL_Point Grid::pixels(int x, int y)
{
    return SDL_Point{
        x*_unit_size + _bounds.x,
        _bounds.y - (y+1)*_unit_size
    };
}
SDL_Point Grid::pixels(SDL_Point const & p) { return pixels(p.x, p.y); }

SDL_Rect Grid::rect(int x, int y)
{
    auto const q = pixels(x, y);
    return SDL_Rect{q.x, q.y, _unit_size, _unit_size};
}

SDL_Point Grid::nearest_point(SDL_Point const & p)
{
    float const unit_size = static_cast<float>(unit_size);
    float x = p.x/unit_size;
    float y = (_bounds.y+_bounds.h-p.y)/unit_size;

    return SDL_Point {
        static_cast<int>(std::round(x)), static_cast<int>(std::round(y))
    };
}
