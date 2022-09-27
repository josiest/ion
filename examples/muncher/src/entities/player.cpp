// game framework
#include "entities/player.hpp"
#include "components.hpp"

// frameworks
#include <SDL2/SDL.h>
#include <entt/entity/registry.hpp>

// data types
#include <cstdint>
#include <string>

// deserialization
#include <filesystem>
#include "parsing.hpp"

using namespace std::string_literals;
namespace fs = std::filesystem;

tl::exepcted<player_spawner, std::string>
player_spawner::from_config(std::string const & config_filename,
                            SDL_Renderer * renderer)
{
    fs::path const config_path = config_filename;
    if (not fs::exists(config_path)) {
        return tl::unexpected("No file named "s + config_filename);
    }
    toml::parse_result table_result = toml::parse_file(config_filename);
    if (not table_result) {
        return tl::unexpected(std::string(table_result.error().description()));
    }

    // validate player settings
    auto table = std::move(table_result).table();
    if (not table["player"]) {
        return tl::unexpected("Spawn config has no player sttings"s);
    }
    if (not table["player"].is_table()) {
        return tl::unexpected("player spawn settings must be a table"s);
    }
    auto player_config = *table["player"].as_table();

    // load the player spawn parameters
    if (not player_config["color"]) {
        return tl::unexpected("player config must have a color setting");
    }
    auto color_result = parsing::load_color(player_config["color"]);
    if (not color_result) {
        return tl::unexpected(color_result.error());
    }
    component::color const color = std::move(color_result).value();

    auto size_result = parsing::load_float(player_config, "size");
    if (not size_result) {
        return tl::unexpected(size_result.error());
    }
    float const size = std::move(size_result).value();

    auto acceleration_result = parsing::load_float(
            player_config, "acceleration");
    if (not acceleration_result) {
        return tl::unexpected(acceleration_result.error());
    }
    float const acceleration = std::move(acceleration_result).value();

    auto friction_result = parsing::load_float(
            player_config, "friction");
    if (not friction_result) {
        return tl::unexpected(friction_result.error());
    }
    float const friction = std::move(friction_result).value();

    auto minspeed_result = parsing::load_float(
            player_config, "minspeed");
    if (not minspeed_result) {
        return tl::unexpected(minspeed_result.error());
    }
    float const minspeed = std::move(minspeed_result).value();

    auto maxspeed_result = parsing::load_float(
            player_config, "maxspeed");
    if (not maxspeed_result) {
        return tl::unexpected(maxspeed_result.error());
    }
    float const maxspeed = std::move(maxspeed_result).value();

    auto growth_rate_result = parsing::load_float(
            player_config, "growth_rate");
    if (not growth_rate_result) {
        return tl::unexpected(growth_rate_result.error());
    }
    float const growth_rate = std::move(growth_rate_result).value();

    return player_spawner{
        renderer, color, size, acceleration, friction,
        minspeed, maxspeed, growth_rate
    };
}

player_spawner::player_spawner(
        SDL_Renderer * renderer, component::color const & color,
        float size, float acceleration, float friction,
        float minspeed, float maxspeed, float growth_rate)

    : _renderer{ renderer },
      _color{ _color },
      _size{ size },
      _acceleration{ acceleration },
      _friction{ friction },
      _minspeed{ minspeed },
      _maxspeed{ maxspeed },
      _growth_rate{ growth_rate }
{
}

entt::entity
player_spawner::make_player(entt::registry & registry) const
{
    auto player = registry.create();

    int width, height;
    SDL_GetWindowSize(_window, &width, &height);

    registry.emplace<component::bbox>(player, width/2.f, height/2.f, _size);
    registry.emplace<component::velocity>(player, 0.f, 0.f);
    registry.emplace<component::color>(player, _color);
    registry.emplace<component::dynamic_mover>(player, _acceleration, _friction,
                                                       _minspeed, _maxspeed);
    registry.emplace<component::growth_rate>(player, _growth_rate);
    return player;
}

component::bbox
player_spawner::bbox(entt::registry const & entities,
                     entt::entity playerid) const
{
    // return the player's position if they exist
    if (entities.valid(playerid) and
            entities.all_of<component::bbox>(playerid)) {

        return entities.get<component::bbox>(playerid);
    }
    // otherwise return their default starting size
    int width, height;
    SDL_GetRendererOutputSize(_renderer, &width, &height);
    return { width/2.f, height/2.f, _size };
}
