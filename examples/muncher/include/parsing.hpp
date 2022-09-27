#pragma once

// game framework
#include "components.hpp"

// data types
#include <string>

// resource handles
#include <tl/expected.hpp>

// i/o
#define TOML_EXCEPTIONS 0
#include <toml++/toml.h>
#include <iterator>

namespace parsing {

/**
 * \brief Load a color from a toml array node.
 *
 * \param color_node the toml::array node to parse
 */
tl::expected<component::color, std::string>
load_color(auto const & color_node)
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

    return true;
}

template<std::weakly_incrementable color_writer>
tl::expected<bool, std::string>
load_colors(toml::table const & config,
            color_writer into_colors)
{
    if (not config["colors"]) {
        return tl::unexpected("settings has no colors"s);
    }
    if (not config["colors"].is_array()) {
        return tl::unexpected("colors must be a list"s);
    }
    auto colors = *config["colors"].as_array();
    std::vector<std::string> color_errors;
    colors.for_each([&color_errors, into_colors](auto const & color) {
        auto color_result = load_color(color, into_colors);
        if (not color_result) {
            color_errors.push_back(color_result.error());
        }
        else {
            *into_colors = color_result.value();
            ++into_colors;
        }
    });
    if (not color_errors.empty()) {
        return tl::unexpected(color_errors.at(0));
    }
    return true;
}

tl::expected<float, std::string>
load_float(toml::table const & config, std::string const & name)
{
    if (not config[name]) {
        return tl::unexpected("config must have a "s + name + " parameter"s);
    }
    if (not config[name].is_floating_point()) {
        return tl::unexpected(name + " must be a floating point number"s);
    }
    return static_cast<float>(config[name].as_floating_point()->get());
}

}
