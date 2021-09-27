#pragma once
#include "systems/point.hpp"
#include "types/components.hpp"

#include <cstdint>
#include <SDL.h>

namespace systems {

class grid {
public:
    /**
     * Create a grid at (x, y) in pixel space
     *
     * \param width the width of the bounds
     * \param height the height of the bounds
     *
     * \param unit_size the amount of pixels-per-unit
     */
    constexpr grid(std::uint32_t width, std::uint32_t height,
                   std::uint32_t unit_size)

        : _width{width}, _height{height}, _unit_size{unit_size}
    {}

    /**
     * Convert a grid point to pixels
     *
     * \param x the x-coordinate of the grid point to convert
     * \param y the y-coordinate of the grid point to convert
     */
    inline SDL_Point pixels(int x, int y) const
    {
        return {static_cast<int>(x*_unit_size),
                static_cast<int>(_height - (y+1)*_unit_size)};
    }

    /**
     * Convert a grid point to pixels
     * \param p the grid-point to convert
     */
    inline SDL_Point pixels(point auto const & p) const
    {
        return pixels(p.x, p.y);
    }

    /**
     * Get the rect of the grid square in pixel-space at (x, y) in grid-space
     *
     * \param x the x-coordinate of the grid point
     * \param y the y-coordinate of the grid point
     */
    inline SDL_Rect unit_square(int x, int y) const
    {
        auto const q = pixels(x, y);
        int const unit_size = static_cast<int>(_unit_size);
        return SDL_Rect{q.x, q.y, unit_size, unit_size};
    }

    /**
     * Get the rect of the grid square in pixel-space at p in grid-space
     * \param p the grid pint
     */
    inline SDL_Rect unit_square(point auto const & p) const
    {
        return unit_square(p.x, p.y);
    }

    /**
     * Calculate the nearest grid point from pixels
     *
     * \param x the x-coordinate of the pixel-point to convert
     * \param y the y-coordinate of the pixel-point to convert
     */
    inline component::position nearest_point(int x, int y) const
    {
        int const height = static_cast<int>(_height);
        int const unit_size = static_cast<int>(_unit_size);
        return { x/unit_size, (height-y)/unit_size };
    }

    /**
     * Calculate the nearest grid point from pixels
     * \param p the pixel-point to convert
     */
    inline component::position nearest_point(point auto const & p) const
    {
        return nearest_point(p.x, p.y);
    }

    /**
     * \return he unit size of this grid
     */
    inline int unit_size() const { return _unit_size; }
private:
    std::uint32_t _width, _height;
    std::uint32_t _unit_size;
};

}
