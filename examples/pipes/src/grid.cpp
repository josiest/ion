#include "grid.hpp"
#include <SDL2/SDL_rect.h>
#include <cmath>
#include <iostream>

Grid::Grid(int x, int y, int w, int h, int unit_size)
    : _bounds{x, y, w, h}, _unit_size{unit_size}
{}

SDL_Point Grid::pixels(int x, int y) const
{
    return SDL_Point{
        x*_unit_size + _bounds.x,
        _bounds.y - (y+1)*_unit_size
    };
}
SDL_Point Grid::pixels(SDL_Point const & p) const { return pixels(p.x, p.y); }

SDL_Rect Grid::rect(int x, int y) const
{
    auto const q = pixels(x, y);
    return SDL_Rect{q.x, q.y, _unit_size, _unit_size};
}
SDL_Rect Grid::rect(SDL_Point const & p) const { return rect(p.x, p.y); }

SDL_Point Grid::nearest_point(SDL_Point const & p) const
{
    float const unit_size = static_cast<float>(_unit_size);
    return SDL_Point {
        static_cast<int>(std::floor((p.x-_bounds.x)/unit_size)),
        static_cast<int>(std::floor((_bounds.y-p.y)/unit_size))
    };
}
