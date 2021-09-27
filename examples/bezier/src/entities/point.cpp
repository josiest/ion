#include "entities/point.hpp"
#include "components.hpp"

#include <glm/glm.hpp>

#include <string>

#include <algorithm>
#include <numeric>
#include <ranges>

namespace cmpt = component;
namespace ranges = std::ranges;
namespace views = std::views;

using namespace std::string_literals;

namespace prefab {

point::point(float r, glm::vec4 const & color, std::uint32_t resolution)

    : _radius{r}, _color{color}, _resolution{resolution}
{}

entt::entity point::create(entt::registry & entities, glm::vec2 const & p)
{
    return create(entities, p.x, p.y);
}
entt::entity point::create(entt::registry & entities, float x, float y)
{
    // create the point with the specified coordinates
    auto const entity = entities.create();
    entities.emplace<cmpt::position>(entity, x, y);

    // give the point a unique vao object
    auto & vao = _vaos.emplace_back();
    entities.emplace<cmpt::vao>(entity, vao);

    // make sure that the vertex array is ok
    if (not vao) {
        _error = vao.error();
    }

    return entity;
}

bool point::operator!() const
{
    return ranges::any_of(_vaos, &ion::vao::operator!);
}

std::string point::error() const
{
    // get the error message of each vao that has failed
    auto messages = _vaos | views::filter(&ion::vao::operator!)
                          | views::transform(&ion::vao::error);

    // concatenate with newlines
    return std::accumulate(messages.begin(), messages.end(), "\n"s);
}
}
