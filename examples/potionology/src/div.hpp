#pragma once

#include "gui_element.hpp"

#include <ion/ion.hpp>
#include <SDL.h>

// data types
#include <cstdint>
#include <utility>

// data structures
#include <vector>

class div_template;

class divider : public IGuiElement {
public:
    divider() = delete;

    /** Use a gui element factory to create an element in this divider
     * \param element a pointer to the element to add
     */
    void produce_element_from(IGuiElementFactory * factory);

    /** Add two divs, vertically splitting this div
     * \param left_ratio the fraction of the div to allocate to the left side
     */
    std::pair<divider *, divider *>
        vsplit(div_template & factory, float left_ratio = .5f);

    // interface methods
    SDL_Rect bounds() const;
    void render(ion::hardware_renderer & window);
private:
    friend class div_template;

    SDL_Rect _bounds;
    int _padding;
    std::vector<IGuiElement *> _elements;

    /** Create a divider for gui elements
     * \param bounds the boundaries of the divider
     * \param the padding in pixels to use for elements in the divider
     */
    divider(SDL_Rect const & bounds, std::uint32_t padding);

    // get the width of the div after padding
    inline int padded_width() const { return _bounds.w - 2*_padding; }

    // get the height of the div after padding
    inline int padded_height() const { return _bounds.h - 2*_padding; }

    // get the next valid point for a gui element
    SDL_Point next_point() const;
};
