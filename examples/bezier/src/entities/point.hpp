#pragma once

#include <ion/ion.hpp>
#include <ion/gl.hpp>
#include <entt/entity/registry.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <memory>

#include <string>
#include <cstdint>

namespace prefab {

/** A prefab to create points */
class point {
public:
    /** Initialize a point prefab */
    point(float radius, glm::vec4 const & color, std::uint32_t resolution = 20);

    /** Create a new point */
    entt::entity create(entt::registry & entities, glm::vec2 const & p);
    entt::entity create(entt::registry & entities, float x, float y);

    float const * color() const { return glm::value_ptr(_color); }
    float radius() const { return _radius; }
    std::uint32_t resolution() const { return _resolution; }

    bool operator!() const;
    std::string const & error() const;
private:
    float _radius;
    glm::vec4 _color;
    std::uint32_t _resolution;

    std::vector<std::unique_ptr<ion::vao>> _vaos;
    std::string _error;
};
}
