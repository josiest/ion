#pragma once
#include <memory>
#include <vector>
#include <concepts>

#include "ion/engine/sdl_resources.hpp"
#include "ion/engine/engine_component.hpp"

namespace ion
{
class engine
{
public:
    static std::unique_ptr<engine> initialize();

    template<std::derived_from<IEngineComponent> ComponentType>
    void add_component(ComponentType * component);

    template<std::derived_from<IEngineComponent> ComponentType>
    ComponentType * find_component() const;

    void start();
    void quit();

    bool has_quit() const { return wants_to_quit; }
protected:
    std::vector<std::unique_ptr<IEngineComponent>> components;
    sdl_context sdl = nullptr;
    bool wants_to_quit = false;
};
}

template<std::derived_from<ion::IEngineComponent> ComponentType>
void ion::engine::add_component(ComponentType * component)
{
    components.emplace_back(component);
    component->set_owner(this);
}

template<std::derived_from<ion::IEngineComponent> ComponentType>
ComponentType * ion::engine::find_component() const
{
    for (const auto & engine_component : components)
    {
        if (auto * component = dynamic_cast<ComponentType>(engine_component.get()))
        {
            return component;
        }
    }
    return nullptr;
}
