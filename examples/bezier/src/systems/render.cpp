#define GL_GLEXT_PROTOTYPES
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_opengl_glext.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "components.hpp"
#include "entities/point.hpp"
#include "systems/render.hpp"

#include <type_traits>
#include <cstdint>

namespace systems {

namespace cmpt = component;

enum class point_shader_attributes : GLuint {
    screen_dim = 0, p, r, n, color
};

template<typename enum_class>
decltype(auto) as_idx(enum_class const & val)
{
    return static_cast<std::underlying_type_t<enum_class>>(val);
}

void render(ion::glwindow & window, entt::registry const & entities,
            prefab::point const & point_prefab,
            ion::shader_program const & point_shader)
{
    // clear the screen blue
    glClearBufferfv(GL_COLOR, 0, glm::value_ptr(blue));
    render_points(window, entities, point_prefab, point_shader);
}

decltype(auto) render_point(auto const & point_prefab)
{
    return [&point_prefab](auto const & vao, auto const & p) {

        // get the vertex array object and the position to render it
        using var = point_shader_attributes; // alias the attribute enum
        glm::vec2 q = p;

        // bind the vertex array and assign the attributes their respective values
        glBindVertexArray(vao);
        glVertexAttrib2fv(as_idx(var::p), glm::value_ptr(q));
        glVertexAttrib1f(as_idx(var::r), point_prefab.radius());
        glVertexAttribI1ui(as_idx(var::n), point_prefab.resolution()+2);
        glVertexAttrib4fv(as_idx(var::color), point_prefab.color());

        // draw the point as a triangle fan
        glDrawArrays(GL_TRIANGLE_FAN, 0, point_prefab.resolution()+2);
    };
}

void render_points(ion::glwindow & window, entt::registry const & entities,
                   prefab::point const & point_prefab,
                   ion::shader_program const & point_shader)
{
    // use the appropriate shader
    glUseProgram(point_shader);
    using var = point_shader_attributes; // alias the attribute enum

    // pass the screen dimension and time uniform constants
    int width, height;
    SDL_GetWindowSize(window, &width, &height);
    glUniform2f(as_idx(var::screen_dim), 1.f*width, 1.f*height);

    // render each point
    auto const points = entities.view<cmpt::vao, cmpt::position>();
    points.each(render_point(point_prefab));
}

}
