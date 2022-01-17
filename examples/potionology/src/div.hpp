#pragma once

#include "gui_element.hpp"

#include <ion/ion.hpp>
#include <SDL.h>

// data types
#include <cstdint>

// data structures
#include <vector>

class divider : public IGuiElement {
public:
    divider() = delete;

    /** Create a divider for gui elements
     * \param bounds the boundaries of the divider
     * \param the padding in pixels to use for elements in the divider
     */
    divider(SDL_Rect const & bounds, std::uint32_t padding = 5);

    /** Use a gui element factory to create an element in this divider
     * \param element a pointer to the element to add
     */
    void produce_element_from(IGuiElementFactory * factory);
    inline std::uint32_t padded_width() { return _bounds.w - 2*_padding; }

    // interface methods
    SDL_Rect bounds() const;
    void render(ion::hardware_renderer & window);
private:
    SDL_Rect _bounds;
    std::uint32_t _padding;
    std::vector<IGuiElement *> _elements;
};
