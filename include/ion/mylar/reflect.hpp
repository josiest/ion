#pragma once
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_pixels.h>
#include <entt/meta/factory.hpp>

namespace ion {

template<typename T>
auto reflect()
{
    return entt::meta_factory<T>{};
}

template<>
auto reflect<SDL_Point>()
{
    using namespace entt::literals;
    return entt::meta_factory<SDL_Point>{}
        .type("sdl::point"_hs)
        .data<&SDL_Point::x>("x"_hs)
        .data<&SDL_Point::y>("y"_hs);
}

template<>
auto reflect<SDL_Rect>()
{
    using namespace entt::literals;
    return entt::meta_factory<SDL_Rect>{}
        .type("sdl::rect"_hs)
        .data<&SDL_Rect::x>("x"_hs)
        .data<&SDL_Rect::y>("y"_hs)
        .data<&SDL_Rect::w>("w"_hs)
        .data<&SDL_Rect::h>("h"_hs);
}

template<>
auto reflect<SDL_Color>()
{
    using namespace entt::literals;
    return entt::meta_factory<SDL_Color>{}
        .type("sdl::color"_hs)
        .data<&SDL_Color::r>("r"_hs)
        .data<&SDL_Color::g>("g"_hs)
        .data<&SDL_Color::b>("b"_hs)
        .data<&SDL_Color::a>("a"_hs);
}
}