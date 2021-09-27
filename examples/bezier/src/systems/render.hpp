#pragma once
#include <ion/gl.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_access.hpp>

#include "entities/point.hpp"
#include <entt/entity/registry.hpp>

#include <cstdint>

// some nice colors
glm::vec4 constexpr blue{.26f, .53f, .96f, 1.f};
glm::vec4 constexpr pink{.99f, .74f, .71f, 1.f};

namespace systems {

/** Render the program */
void render(ion::glwindow & window, entt::registry const & entities,
            prefab::point const & point_prefab,
            ion::shader_program const & point_shader);

/** Render all point */
void render_points(ion::glwindow & window, entt::registry const & entities,
                   prefab::point const & point_prefab,
                   ion::shader_program const & point_shader);
}
