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
    _vaos.push_back(std::make_unique<ion::vao>());
    entities.emplace<cmpt::vao>(entity, *_vaos.back());

    // make sure that the vertex array is ok
    if (not *_vaos.back()) {
        _error = _vaos.back()->error();
    }

    return entity;
}

bool vao_is_bad(std::unique_ptr<ion::vao> const & vao)
{
    return not *vao;
}
std::string const & vao_error(std::unique_ptr<ion::vao> const & vao)
{
    return vao->error();
}

bool point::operator!() const
{
    return ranges::any_of(_vaos, vao_is_bad);
}

std::string const & point::error() const
{
    // get the error message of each vao that has failed
    auto messages = _vaos | views::filter(vao_is_bad)
                          | views::transform(vao_error);

    // concatenate with newlines
    return std::accumulate(messages.begin(), messages.end(), "\n"s);
}
}
