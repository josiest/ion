// entity definitions
#include "entities/munchable.hpp"
// #include "entities/player.hpp"

// game systems
#include "components.hpp"
// #include "systems/physics.hpp"

// frameworks
#include <entt/entity/registry.hpp>
#include <SDL2/SDL.h>

// data types
#include <string>

// math
#include <random>
#include <utility>
#include <cmath>

// deserialzation
#define TOML_EXCEPTIONS 0
#include <toml++/toml.h>
#include <filesystem>

// algorithms
#include <iterator>
#include <ranges>
#include <algorithm>

namespace fs = std::filesystem;
using namespace std::string_literals;

namespace spawn_munchables {

template<std::weakly_incrementable color_writer>
tl::expected<bool, std::string>
load_color(auto const & color_node, color_writer into_colors)
{
    if (not color_node.is_array()) {
        return tl::unexpected("color must be an array"s);
    }
    auto color_vals = *color_node.as_array();
    if (not color_vals.is_homogeneous()) {
        return tl::unexpected("color values must have uniform type"s);
    }
    if (color_vals.size() != 3) {
        return tl::unexpected("colors must have three values"s);
    }
    auto red = color_vals.at(0).as_integer();
    auto green = color_vals.at(1).as_integer();
    auto blue = color_vals.at(2).as_integer();

    if (not red) {
        return tl::unexpected("color values must be integers"s);
    }
    component::color color {
        static_cast<std::uint8_t>(red->get()),
        static_cast<std::uint8_t>(green->get()),
        static_cast<std::uint8_t>(blue->get())
    };
    *into_colors = color;
    ++into_colors;

    return true;
}

template<std::weakly_incrementable color_writer>
tl::expected<bool, std::string>
load_colors(toml::table const & munchable_config,
            color_writer into_colors)
{
    if (not munchable_config["colors"]) {
        return tl::unexpected("munchable settings must have colors"s);
    }
    if (not munchable_config["colors"].is_array()) {
        return tl::unexpected("munchable.colors must be a list"s);
    }
    auto munchable_colors = *munchable_config["colors"].as_array();
    std::vector<std::string> color_errors;
    munchable_colors.for_each([&color_errors, into_colors](auto const & color) {
        auto color_result = load_color(color, into_colors);
        if (not color_result) {
            color_errors.push_back(color_result.error());
        }
    });
    if (not color_errors.empty()) {
        return tl::unexpected(color_errors.at(0));
    }
    return true;
}

tl::expected<float, std::string>
load_float(toml::table const & munchable_config,
           std::string const & name)
{
    if (not munchable_config[name]) {
        return tl::unexpected("munchable config must have a "s +
                              name + " parameter"s);
    }
    if (not munchable_config[name].is_floating_point()) {
        return tl::unexpected("munchable."s + name + " must be a floating "s +
                              "point number"s);
    }
    return static_cast<float>(
            munchable_config[name].as_floating_point()->get());
}
}

tl::expected<munchable_spawner, std::string>
munchable_spawner::from_config(
    std::string const & config_filename,
    SDL_Renderer * renderer)
{
    using namespace spawn_munchables;

    // load the config file
    fs::path const config_path = config_filename;
    if (not fs::exists(config_path)) {
        return tl::unexpected("No file named "s + config_filename);
    }
    toml::parse_result table_result = toml::parse_file(config_filename);
    if (not table_result) {
        return tl::unexpected(std::string(table_result.error().description()));
    }

    // validate the munchable settings
    auto table = std::move(table_result).table();
    if (not table["munchable"]) {
        return tl::unexpected("Spawn config has no munchable settings!"s);
    }
    if (not table["munchable"].is_table()) {
        return tl::unexpected("munchable spawn settings must be a table!"s);
    }

    // load the colors
    auto munchable_config = *table["munchable"].as_table();
    std::vector<component::color> colors;
    auto colors_result = load_colors(
            munchable_config, std::back_inserter(colors));

    if (not colors_result) {
        return tl::unexpected(colors_result.error());
    }

    auto minspeed_result = load_float(munchable_config, "minspeed");
    if (not minspeed_result) {
        return tl::unexpected(minspeed_result.error());
    }
    float const minspeed = std::move(minspeed_result).value();

    auto maxspeed_result = load_float(munchable_config, "maxspeed");
    if (not maxspeed_result) {
        return tl::unexpected(maxspeed_result.error());
    }
    float const maxspeed = std::move(maxspeed_result).value();

    auto anglevar_result = load_float(munchable_config, "angle_variation");
    if (not anglevar_result) {
        return tl::unexpected(anglevar_result.error());
    }
    float const angle_variation = std::move(anglevar_result).value();

    auto sizevar_result = load_float(munchable_config, "size_variation");
    if (not sizevar_result) {
        return tl::unexpected(sizevar_result.error());
    }
    float const size_variation = std::move(sizevar_result).value();

    auto spawn_chance_result = load_float(munchable_config, "spawn_likelihood");
    if (not spawn_chance_result) {
        return tl::unexpected(spawn_chance_result.error());
    }
    float const spawn_likelihood = std::move(spawn_chance_result).value();

    return munchable_spawner{
        renderer, colors, minspeed, maxspeed,
        angle_variation, size_variation, spawn_likelihood
    };
}

munchable_spawner::munchable_spawner(
        SDL_Renderer * renderer,
        std::vector<component::color> const & colors,
        float minspeed, float maxspeed,
        float angle_variation,
        float size_variation,
        float spawn_likelihood)

    : _renderer{ renderer },
      _colors{ colors },
      _minspeed{ minspeed },
      _maxspeed{ maxspeed },
      _angle_variation{ angle_variation },
      _size_variation{ size_variation },
      _spawn_likelihood{ spawn_likelihood }
{
}

// entt::entity
// munchable::create(entt::registry & entities, entt::entity playerid,
//                   player const & player_settings, engine_t rng) const noexcept
// {
//     // get the size of the player as a mean for the size of the munchable
//     auto const player_box = player_settings.try_get_bbox(entities, playerid);
// 
//     // create the munchable entity
//     auto munchableid = entities.create();
// 
//     // assign the munchable a random bbox component
//     auto const & munchable_box = random_bbox(entities, munchableid,
//                                              player_box, rng);
// 
//     // assign the munchable a random velocity and color component
//     random_velocity(entities, munchableid, munchable_box, player_box, rng);
//     random_color(entities, munchableid, rng);
// 
//     // tag the munchable and return its id
//     entities.emplace<component::munchable>(munchableid);
//     return munchableid;
// }
// 
// bool munchable::should_spawn(float dt, engine_t & rng) const noexcept
// {
//     // convert dt to milliseconds
//     dt *= 1000.f;
// 
//     // coin flip distribution determine wether to munch
//     std::binomial_distribution<bool> is_munchtime{1, _munchtime_likelihood*dt};
//     return is_munchtime(rng);
// }
// 
// component::bbox &
// munchable::random_bbox(entt::registry & entities, entt::entity munchableid,
//                        component::bbox const & player_box,
//                        engine_t & rng) const noexcept
// {
//     // query the window for it's size
//     int width, height;
//     SDL_GetWindowSize(_window, &width, &height);
// 
//     // the distribution for the x and y position
//     std::uniform_real_distribution<float> xdist(0.f, width);
//     float const x = xdist(rng);
// 
//     std::uniform_real_distribution<float> ydist(0.f, height);
//     float const y = ydist(rng);
// 
//     // the choices for the positions (must be on a boundary)
//     using point = std::pair<float, float>;
//     std::vector<point> positions{{0, y}, {width, y}, {x, 0}, {x, height}};
// 
//     // choose a random position
//     std::uniform_int_distribution<std::size_t> idist(0, positions.size()-1);
//     size_t const i = idist(rng);
// 
//     // use lognomal distribution to generate a random size to ensure positivity
//     //  player_box.size is assumed to be positive
//     std::lognormal_distribution size_dist{std::log(player_box.size),
//                                           std::log(_size_variation)};
// 
//     // add the new bbox component to the munchable entitiy
//     return entities.emplace<component::bbox>(munchableid,
//             positions[i].first, positions[i].second, size_dist(rng));
// }
// 
// component::velocity &
// munchable::random_velocity(entt::registry & entities, entt::entity munchableid,
//                            component::bbox const & munchable_box,
//                            component::bbox const & player_box,
//                            engine_t & rng) const noexcept
// {
//     // the distributions for velocity speed and angle
//     std::uniform_real_distribution speed_dist(_minspeed, _maxspeed);
//     float const speed = speed_dist(rng);
// 
//     // calculate the angle to the player
//     auto const d = systems::distance(munchable_box, player_box);
//     auto const dn = systems::normalized(d, .1f);
//     float const angle_to_player = std::atan2(dn.y, dn.x);
// 
//     // generate a random angle
//     std::normal_distribution angle_dist(angle_to_player, _angle_variation);
//     float const phi = angle_dist(rng);
// 
//     // calculate the velocity from the random speed and angle
//     return entities.emplace<component::velocity>(munchableid,
//             speed*std::cos(phi), speed*std::sin(phi));
// }
// 
// component::color &
// munchable::random_color(entt::registry & entities, entt::entity munchableid,
//                         engine_t & rng) const noexcept
// {
//     // choose a color
//     std::uniform_int_distribution<std::size_t> idist(0, _colors.size()-1);
//     auto const color = _colors[idist(rng)];
// 
//     // add it as a component to the munchable entity
//     return entities.emplace<component::color>(munchableid,
//             color.r, color.g, color.b);
// }
