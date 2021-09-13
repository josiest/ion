#include "systems/grid.hpp"
#include <SDL2/SDL_rect.h>
#include <cmath>

grid::grid(int x, int y, int w, int h, int unit_size)
    : _bounds{x, y, w, h}, _unit_size{unit_size}
{}

SDL_Point grid::pixels(int x, int y) const
{
    return SDL_Point{
        x*_unit_size + _bounds.x,
        _bounds.y - (y+1)*_unit_size
    };
}
SDL_Point grid::pixels(SDL_Point const & p) const { return pixels(p.x, p.y); }

SDL_Rect grid::unit_square(int x, int y) const
{
    auto const q = pixels(x, y);
    return SDL_Rect{q.x, q.y, _unit_size, _unit_size};
}
SDL_Rect grid::unit_square(SDL_Point const & p) const
{
    return unit_square(p.x, p.y);
}

SDL_Point grid::nearest_point(int x, int y) const
{
    float const unit_size = static_cast<float>(_unit_size);
    return SDL_Point {
        static_cast<int>(std::floor((x-_bounds.x)/unit_size)),
        static_cast<int>(std::floor((_bounds.y-y)/unit_size))
    };
}
