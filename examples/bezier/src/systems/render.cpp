#define GL_GLEXT_PROTOTYPES
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_opengl_glext.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "components.hpp"
#include "systems/render.hpp"

#include <type_traits>
#include <cstdint>

namespace systems {

// some nice colors
static glm::vec4 constexpr blue{.26f, .53f, .96f, 1.f};
static glm::vec4 constexpr pink{.99f, .74f, .71f, 1.f};

enum class point_shader_attributes : GLuint {
    p = 0, r, n, color
};

template<typename enum_class>
decltype(auto) as_idx(enum_class const & val)
{
    return static_cast<std::underlying_type_t<enum_class>>(val);
}

void render(ion::glwindow & window,
            ion::shader_program const & point_shader)
{
    // get the size of the window in order to find where to draw the point
    int width, height;
    SDL_GetWindowSize(window, &width, &height);
    glm::vec2 const p{0.f, 0.f};

    // clear the screen blue
    glClearBufferfv(GL_COLOR, 0, glm::value_ptr(blue));

    glUseProgram(point_shader);
    render_point(p, 40.f, pink);
}

void render_point(glm::vec2 const & p, float r,
                  glm::vec4 const & color, std::uint32_t n)
{
    using ps_attr = point_shader_attributes;

    glVertexAttrib2fv(as_idx(ps_attr::p), glm::value_ptr(p));
    glVertexAttrib1f(as_idx(ps_attr::r), .5f);
    glVertexAttribI1ui(as_idx(ps_attr::n), n+2);
    glVertexAttrib4fv(as_idx(ps_attr::color), glm::value_ptr(pink));

    glDrawArrays(GL_TRIANGLE_FAN, 0, n+2);
}

}
