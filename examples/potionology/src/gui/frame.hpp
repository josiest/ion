#pragma once

#include "gui/widget.hpp"

#include <ion/ion.hpp>
#include <SDL.h>

// data types
#include <cstdint>
#include <utility>

// data structures
#include <vector>

class frame_template;

class frame : public IWidget {
public:
    frame() = delete;

    /** Use a gui element factory to create an element in this frame
     * \param element a pointer to the element to add
     */
    void produce_element_from(IWidgetFactory * factory);

    /** Add two frames, vertically splitting this frame
     * \param left_ratio the fraction of the frame to allocate to the left side
     */
    std::pair<frame *, frame *>
        vsplit(frame_template & factory, float left_ratio = .5f);

    // interface methods
    SDL_Rect bounds() const;
    void render(ion::hardware_renderer & window);
private:
    friend class frame_template;

    SDL_Rect _bounds;
    int _padding;
    std::vector<IWidget *> _elements;

    /** Create a frame for gui elements
     * \param bounds the boundaries of the frame
     * \param the padding in pixels to use for elements in the frame
     */
    frame(SDL_Rect const & bounds, std::uint32_t padding);

    // get the width of the frame after padding
    inline int padded_width() const { return _bounds.w - 2*_padding; }

    // get the height of the frame after padding
    inline int padded_height() const { return _bounds.h - 2*_padding; }

    // get the next valid point for a gui element
    SDL_Point next_point() const;
};
