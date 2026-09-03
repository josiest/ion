#include "ion/engine/engine.hpp"

std::unique_ptr<ion::engine> ion::engine::initialize()
{
    return nullptr;
}

void ion::engine::start()
{
    for (const auto & component : components)
    {
        component->start();
    }
}

void ion::engine::quit()
{
    wants_to_quit = true;
    for (const auto & component : components)
    {
        component->quit();
    }
}