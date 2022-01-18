#pragma once

#include <ion/ion.hpp>
#include <SDL.h>
#include <cstdint>

class IWidget {
public:
    /** Get the boundary of this element. */
    virtual SDL_Rect bounds() const = 0;

    /** Render this gui element.
     * \param window the window to render to
     */
    virtual void render(ion::hardware_renderer & window) = 0;
    virtual ~IWidget() {}
};

class IWidgetFactory {
public:
    /** Create a GUI element */
    virtual IWidget * make(int x, int y, std::uint32_t height) = 0;
    virtual ~IWidgetFactory() {}
};
