#pragma once
#include <SDL2/SDL_rect.h>
#include "types/point.hpp"

class grid {
public:
    /**
     * Create a grid at (x, y) in pixel space
     *
     * \param x the x-coordinate of the grid origin in pixel-space
     * \param y the y-coordinate of the grid origin in pixel-space
     * \param w the width of the bounds
     * \param h the height of the bounds
     * \param unit_size the amount of pixels-per-unit
     */
    constexpr grid(int x, int y, size_t w, size_t h, size_t unit_size) noexcept
        : _bounds{x, y, static_cast<int>(w), static_cast<int>(h)},
          _unit_size{static_cast<int>(unit_size)}
    {}

    /**
     * Convert a grid point to pixels
     *
     * \param x the x-coordinate of the grid point to convert
     * \param y the y-coordinate of the grid point to convert
     */
    inline SDL_Point pixels(int x, int y) const noexcept
    {
        return SDL_Point{
            x*_unit_size + _bounds.x,
            _bounds.y - (y+1)*_unit_size
        };
    }

    /**
     * Convert a grid point to pixels
     *
     * \param p the grid-point to convert
     */
    inline SDL_Point pixels(point auto const & p) const noexcept
    {
        return pixels(p.x, p.y);
    }

    /**
     * Get the rect of the grid square in pixel-space at (x, y) in grid-space
     *
     * \param x the x-coordinate of the grid point
     * \param y the y-coordinate of the grid point
     */
    inline SDL_Rect unit_square(int x, int y) const noexcept
    {
        auto const q = pixels(x, y);
        return SDL_Rect{q.x, q.y, _unit_size, _unit_size};
    }

    /**
     * Get the rect of the grid square in pixel-space at p in grid-space
     *
     * \param p the grid pint
     */
    inline SDL_Rect unit_square(point auto const & p) const noexcept
    {
        return unit_square(p.x, p.y);
    }

    /**
     * Calculate the nearest grid point from pixels
     *
     * \param x the x-coordinate of the pixel-point to convert
     * \param y the y-coordinate of the pixel-point to convert
     */
    inline SDL_Point nearest_point(int x, int y) const noexcept
    {
        float const unit_size = static_cast<float>(_unit_size);
        return SDL_Point {
            static_cast<int>(std::floor((x-_bounds.x)/unit_size)),
            static_cast<int>(std::floor((_bounds.y-y)/unit_size))
        };
    }

    /**
     * Calculate the nearest grid point from pixels
     *
     * \param p the pixel-point to convert
     */
    inline SDL_Point nearest_point(point auto const & p) const noexcept
    {
        return nearest_point(p.x, p.y);
    }

    /**
     * \return he unit size of this grid
     */
    inline int unit_size() const noexcept { return _unit_size; }
private:
    SDL_Rect const _bounds;
    int const _unit_size;
};
