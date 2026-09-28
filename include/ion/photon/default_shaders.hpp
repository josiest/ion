#pragma once
#include <string_view>
#include <gl/glew.h>
#include <glm/glm.hpp>

#include "shader.hpp"

namespace ion::shader
{
class solid_fill
{
    // shader-like interface
public:
    static constexpr std::string_view shader_name = "solid-fill";

    explicit  solid_fill(GLuint program);
    solid_fill() = delete;

    void use_program() const;

    // public interface members
    glm::mat4x4 transform;
    glm::vec4 color;

    // internal interface
private:
    shader_handle handle;
};

class rounded_rect
{
    // shader-like interface
public:
    static constexpr std::string_view shader_name = "rounded-rect";
    explicit rounded_rect(GLuint program);
    rounded_rect() = delete;

    void use_program() const;

    // public interface members
    glm::mat4x4 transform{};
    glm::vec4 fill_color{ 0.f, 0.f, 0.f, 1.f };
    glm::vec2 bounds{ 1.f, 1.f };
    float rounding = 0.f;

    // internal interface
private:
    shader_handle handle;
};
}
