#pragma once
#include <SDL2/SDL_pixels.h>
#include <entt/entity/registry.hpp>

#include "Pipes/Tile/TileInfo.hpp"
#include "Pipes/Grid.hpp"

namespace Pipes::Component
{
struct Tile
{
    TileInfo::Name name;
    TileInfo::Rotation rotation;
    SDL_Color color;
};
}

namespace Pipes
{
class TileHandle
{
public:
    TileHandle();
    explicit TileHandle(entt::registry & entities);
    virtual ~TileHandle();

    TileHandle(const TileHandle & other);
    TileHandle& operator=(const TileHandle & other) noexcept;

    TileHandle(TileHandle && other);
    TileHandle& operator=(TileHandle && other) noexcept;

    TileInfo::Name name() const;
    TileInfo::Name& name();

    SDL_Point position() const;
    void position(int x, int y);
    void position(const Point auto & p);

    TileInfo::Rotation rotation() const;
    TileInfo::Rotation& rotation();

    const SDL_Color& color() const;
    SDL_Color& color();
protected:
    entt::entity id;
    entt::registry * entities;
};
}

void Pipes::TileHandle::position(const Point auto & p)
{
    position(p.x, p.y);
}
