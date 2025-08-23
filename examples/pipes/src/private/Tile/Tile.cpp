#include "Pipes/Tile/Tile.hpp"
#include "Pipes/Tile/TileInfo.hpp"

Pipes::TileHandle::TileHandle()
    : entities(nullptr), id(entt::null)
{
}

Pipes::TileHandle::TileHandle(entt::registry & entities)
    : entities(&entities), id(entities.create())
{
    entities.emplace<component::position>(id, 0, 0);
    entities.emplace<Component::Tile>(id, TileInfo::Name::Bar, TileInfo::Rotation::Down, SDL_Color{});
}

Pipes::TileHandle::~TileHandle()
{
    if (entities and id != entt::null)
    {
        entities->destroy(id);
    }
    if (id != entt::null)
    {
        id = entt::null;
    }
}

Pipes::TileHandle::TileHandle(const Pipes::TileHandle & other)
    : entities{ other.entities }, id{ entt::null }
{
    id = entities->create();
    const SDL_Point position = other.position();
    entities->emplace<component::position>(id, position.x, position.y);
    entities->emplace<Component::Tile>(id, other.name(), other.rotation(), other.color());
}

Pipes::TileHandle & Pipes::TileHandle::operator=(const Pipes::TileHandle & other) noexcept
{
    const SDL_Point position = other.position();
    entities->replace<component::position>(id, position.x, position.y);
    entities->replace<Component::Tile>(id, other.name(), other.rotation(), other.color());
    return *this;
}

Pipes::TileHandle::TileHandle(Pipes::TileHandle && other)
    : entities{ std::exchange(other.entities, nullptr) }, id{ std::exchange(other.id, entt::null) }
{
}

Pipes::TileHandle & Pipes::TileHandle::operator=(Pipes::TileHandle && other) noexcept
{
    std::swap(entities, other.entities);
    std::swap(id, other.id);
    return *this;
}

Pipes::TileInfo::Name Pipes::TileHandle::name() const
{
    return entities->get<Component::Tile>(id).name;
}

Pipes::TileInfo::Name & Pipes::TileHandle::name()
{
    return entities->get<Component::Tile>(id).name;
}

SDL_Point Pipes::TileHandle::position() const
{
    return static_cast<SDL_Point>(entities->get<component::position>(id));
}

void Pipes::TileHandle::position(int x, int y)
{
    auto & position = entities->get<component::position>(id);
    position.x = x;
    position.y = y;
}

Pipes::TileInfo::Rotation Pipes::TileHandle::rotation() const
{
    return entities->get<Component::Tile>(id).rotation;
}

Pipes::TileInfo::Rotation& Pipes::TileHandle::rotation()
{
    return entities->get<Component::Tile>(id).rotation;
}

const SDL_Color& Pipes::TileHandle::color() const
{
    return entities->get<Component::Tile>(id).color;
}

SDL_Color& Pipes::TileHandle::color()
{
    return entities->get<Component::Tile>(id).color;
}