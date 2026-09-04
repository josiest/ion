#pragma once
#include <memory>
#include <list>
#include <concepts>

#include "ion/engine/sdl_resources.hpp"
#include "ion/engine/engine_component.hpp"

namespace ion
{
class engine
{
public:
    static std::unique_ptr<engine> initialize();
    engine() = default;

    template<std::derived_from<IEngineComponent> ComponentType, typename... Args>
    ComponentType * emplace_component(Args &&... args);

    template<std::derived_from<IEngineComponent> ComponentType>
    ComponentType * find_component() const;

    void start();
    void quit();

    bool has_quit() const { return wants_to_quit; }
protected:
    sdl_context sdl = nullptr;
    std::list<std::unique_ptr<IEngineComponent>> components;

    bool wants_to_quit = false;
};

inline engine * GEngine = nullptr;
}

template<std::derived_from<ion::IEngineComponent> ComponentType, typename... Args>
ComponentType * ion::engine::emplace_component(Args &&... args)
{
    auto * component = new ComponentType(std::forward<Args>(args)...);
    component->owner(this);
    components.emplace_back(dynamic_cast<IEngineComponent *>(component));
    return component;
}

template<std::derived_from<ion::IEngineComponent> ComponentType>
ComponentType * ion::engine::find_component() const
{
    for (const auto & engine_component : components)
    {
        if (auto * component = dynamic_cast<ComponentType *>(engine_component.get()))
        {
            return component;
        }
    }
    return nullptr;
}
