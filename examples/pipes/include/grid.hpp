#pragma once
#include <SDL2/SDL_rect.h>

class Grid {
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
    Grid(int x, int y, int w, int h, int unit_size);

    /**
     * Convert a grid point to pixels
     *
     * \param x the x-coordinate of the grid point to convert
     * \param y the y-coordinate of the grid point to convert
     */
    SDL_Point pixels(int x, int y) const;
    /**
     * Convert a grid point to pixels
     *
     * \param p the grid-point to convert
     */
    SDL_Point pixels(SDL_Point const & p) const;

    /**
     * Get the rect of the grid square in pixel-space at (x, y) in grid-space
     *
     * \param x the x-coordinate of the grid point
     * \param y the y-coordinate of the grid point
     */
    SDL_Rect rect(int x, int y) const;
    /**
     * Get the rect of the grid square in pixel-space at p in grid-space
     *
     * \param p the grid pint
     */
    SDL_Rect rect(SDL_Point const & p) const;

    /**
     * Calculate the nearest grid point from pixels
     *
     * \param p the pixel-point to convert
     */
    SDL_Point nearest_point(SDL_Point const & p) const;

    /**
     * \return he unit size of this grid
     */
    inline int unit_size() const { return _unit_size; }
private:
    SDL_Rect _bounds;
    int _unit_size;
};
