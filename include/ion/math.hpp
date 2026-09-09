#pragma once
#include <concepts>
#include <cmath>
#include <SDL3/SDL_pixels.h>

namespace ion
{
template<typename Field>
concept numeric = std::is_floating_point_v<Field> or std::is_integral_v<Field>;

template<typename T>
using color_field_t = std::remove_cvref_t<decltype(std::declval<T>().r)>;

template<typename Color>
concept color_class = std::constructible_from<Color, color_field_t<Color>, color_field_t<Color>,
                                                     color_field_t<Color>, color_field_t<Color>> and
requires(Color color)
{
    { color.r } -> std::convertible_to<color_field_t<Color>>;
    { color.g } -> std::convertible_to<color_field_t<Color>>;
    { color.b } -> std::convertible_to<color_field_t<Color>>;
    { color.a } -> std::convertible_to<color_field_t<Color>>;
};

template<typename Vector>
using vector_field_t = std::remove_cvref_t<decltype(std::declval<Vector>().x)>;

template<typename Rect>
concept rect_class = std::constructible_from<Rect, vector_field_t<Rect>, vector_field_t<Rect>,
                                                   vector_field_t<Rect>, vector_field_t<Rect>> and
requires(Rect rect)
{
    { rect.x } -> std::convertible_to<vector_field_t<Rect>>;
    { rect.y } -> std::convertible_to<vector_field_t<Rect>>;
    { rect.w } -> std::convertible_to<vector_field_t<Rect>>;
    { rect.h } -> std::convertible_to<vector_field_t<Rect>>;
};

template<typename Color>
concept integral_color = (color_class<Color> and std::is_integral_v<color_field_t<Color>>)
                       or std::is_integral_v<Color>;

template<typename Color>
concept linear_color = color_class<Color> and std::is_floating_point_v<color_field_t<Color>>;

template<typename Color>
concept color_like = integral_color<Color> or linear_color<Color>;

template<linear_color LinearColor, color_like OtherColor>
LinearColor to_linear(const OtherColor & color)
{
    using Real = color_field_t<LinearColor>;
    if constexpr (std::is_integral_v<OtherColor>)
    {
        return LinearColor(static_cast<Real>((color & 0x00ff0000) >> 16)/Real(255.f),
                           static_cast<Real>((color & 0x0000ff00) >> 8)/Real(255.f),
                           static_cast<Real>(color & 0x000000ff)/Real(255.f),
                           Real(1.f));
    }
    else if constexpr (color_class<OtherColor> and std::is_integral_v<color_field_t<OtherColor>>)
    {
        return LinearColor(static_cast<Real>(color.r)/Real(255.f),
                           static_cast<Real>(color.g)/Real(255.f),
                           static_cast<Real>(color.b)/Real(255.f),
                           static_cast<Real>(color.a)/Real(255.f));
    }
    else if constexpr (color_class<OtherColor> and std::is_floating_point_v<color_field_t<OtherColor>>)
    {
        return LinearColor(static_cast<Real>(color.r),
                           static_cast<Real>(color.g),
                           static_cast<Real>(color.b),
                           static_cast<Real>(color.a));
    }
    return LinearColor(Real(0.f), Real(0.f), Real(0.f), Real(1.f));
}

template<std::integral Integer, std::floating_point Real>
constexpr Integer lerp(Integer a, Integer b, Real t)
{
    return static_cast<Integer>(std::round(std::lerp(static_cast<Real>(a), static_cast<Real>(b), t)));
}

template<std::floating_point Real>
constexpr SDL_Color lerp(const SDL_Color& a, const SDL_Color& b, Real t)
{
    return
    {
        .r = lerp(a.r, b.r, t),
        .g = lerp(a.g, b.g, t),
        .b = lerp(a.b, b.b, t),
        .a = lerp(a.a, b.a, t)
    };
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