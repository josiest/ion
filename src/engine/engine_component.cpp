#include "ion/engine/engine_component.hpp"

void ion::window_component::set_owner(engine * new_owner)
{
    owner = new_owner;
}

ion::engine * ion::window_component::get_owner() const
{
    return owner;
}

void ion::renderer_component::set_owner(engine * new_owner)
{
    owner = new_owner;
}

ion::engine * ion::renderer_component::get_owner() const
{
    return owner;
}
