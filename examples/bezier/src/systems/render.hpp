#pragma once

#include <ion/gl.hpp>
#include <glm/glm.hpp>
#include <cstdint>

namespace systems {

void render(ion::glwindow & window,
            ion::shader_program const & point_shader);

/** Render a single point */
void render_point(glm::vec2 const & p, float r,
                  glm::vec4 const & color, std::uint32_t n = 100);
}
