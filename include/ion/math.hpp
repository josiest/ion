#pragma once
#include <concepts>
#include <cmath>

#include <SDL2/SDL_pixels.h>

namespace ion
{
template<std::integral Integer, std::floating_point Real>
constexpr Integer lerp(Integer a, Integer b, Real t)
{
return static_cast<Integer>(std::round(std::lerp(static_cast<Real>(a), static_cast<Real>(b), t)));
}

template<std::floating_point Real>
constexpr SDL_Color lerp(const SDL_Color& a, const SDL_Color& b, Real t)
{
    return { lerp(a.r, b.r, t), lerp(a.g, b.g, t), lerp(a.b, b.b, t), lerp(a.a, b.a, t) };
}

template<std::integral Integer, std::floating_point Real>
constexpr Integer rounded_divide(Integer p, Real q)
{
    return static_cast<Integer>(std::round(static_cast<Real>(p)/q));
}

template<std::integral Integer, std::floating_point Real>
constexpr Integer rounded_divide(Real p, Integer q)
{
    return static_cast<Integer>(std::round(p/static_cast<Real>(q)));
}
}