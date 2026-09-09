#pragma once
#include <optional>
#include <glm/glm.hpp>

#include "photon/default_shaders.hpp"
#include "photon/mesh.hpp"
#include "math.hpp"

struct SDL_Window;

namespace ion
{
class shader_manager;

namespace shader
{
class solid_fill;
}

class canvas
{
    // public constructors
public:
    static std::optional<canvas> create(const shader_manager & shaders, SDL_Window * window);
    canvas() = delete;

    // internal constructors
protected:
    explicit canvas(const shader::solid_fill & solid_fill);

    // public interface
public:
    int width() const { return _width; }
    int height() const { return _height; }

    template<numeric Field> Field width_as() const { return static_cast<Field>(_width); }
    template<numeric Field> Field height_as() const { return static_cast<Field>(_height); }

    template<color_like Color>
    void background(const Color & color) { background_internal(to_linear<SDL_FColor>(color)); }

    template<color_like Color>
    void fill(const Color & color) { fill_internal(to_linear<SDL_FColor>(color)); }

    void triangle(float x1, float y1, float x2, float y2, float x3, float y3);
    void rect(float x, float y, float w, float h);
    template<rect_class Rect> void rect(const Rect & rect);

    // internal interface
protected:
    static void background_internal(const SDL_FColor & color);

    void update_clip_space();
    void fill_internal(const SDL_FColor & color);

    shader::solid_fill solid_fill;

    glm::mat4x4 clip_from_world;
    static_mesh2f<4> rect_mesh;
    static_mesh2f<3> triangle_mesh;

    int _width = 640;
    int _height = 480;
};
}

template<ion::rect_class Rect>
void ion::canvas::rect(const Rect & rect)
{
    if constexpr (std::is_floating_point_v<vector_field_t<Rect>>)
    {
        rect(rect.x, rect.y, rect.w, rect.h);
    }
    else if constexpr (std::is_integral_v<vector_field_t<Rect>>)
    {
        rect(static_cast<float>(rect.x), static_cast<float>(rect.y),
             static_cast<float>(rect.w), static_cast<float>(rect.h));
    }
}
