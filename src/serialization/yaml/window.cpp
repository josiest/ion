#include "ion/serialization/yaml/window.hpp"
#include "ion/serialization/yaml/sdl_primitives.hpp"
#include "ion/window.hpp"

#include <yaml-cpp/yaml.h>
#include <string>

ion::hardware_renderer ion::cereal::read_renderer_from_config(const YAML::Node & config)
{
    std::string name = "ion window";
    if (const auto& name_config = config["name"])
    {
        name = name_config.as<std::string>();
    }
    SDL_Point position{ SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED };
    if (const auto& position_config = config["position"])
    {
        if (position_config.IsSequence())
        {
            try
            {
                position = position_config.as<SDL_Point>();
            }
            catch (YAML::Exception & error)
            {
                SDL_Log("Couldn't read position as SDL_Point, using defaults: %s\n", error.what());
            }
        }
        else if (position_config.as<std::string>() == "centered")
        {
            position = { SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED };
        }
    }

    SDL_Point dimensions{ 640, 480 };
    if (const auto& dimensions_config = config["dimensions"])
    {
        try
        {
            dimensions = dimensions_config.as<SDL_Point>();
        }
        catch (YAML::Exception & error)
        {
            SDL_Log("Couldn't read dimensions as SDL_Point, using defaults: %s\n", error.what());
        }
    }

    constexpr std::uint32_t window_flags = 0u;
    return hardware_renderer::make_window(name, position.x, position.y, dimensions.x, dimensions.y, window_flags);
}
