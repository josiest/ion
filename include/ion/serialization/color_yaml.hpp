#pragma once
#include "ion/mylar.hpp"

#include <regex>
#include <string_view>
#include <string>

#include <SDL3/SDL_pixels.h>
#include <yaml-cpp/yaml.h>
#include <entt/meta/factory.hpp>

namespace ion
{
namespace regex_patterns {
constexpr std::string_view hexadecimal_rgb = "0x([0-9a-fA-F]{6})";
constexpr std::string_view hexadecimal_rgba = "0x([0-9a-fA-F]{8})";
}

template<std::unsigned_integral Integer>
bool parse_hex_rgb(std::string_view text, Integer& r, Integer& g, Integer& b);

template<std::unsigned_integral Integer>
bool parse_hex_rgba(std::string_view text, Integer& r, Integer& g, Integer& b, Integer& a);

std::string write_hex_rgb(std::uint8_t r, std::uint8_t g, std::uint8_t b);
std::string write_hex_rgba(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a);
}

template<>
struct YAML::convert<SDL_Color> {
    static Node encode(const SDL_Color& color);
    static bool decode(const Node& node, SDL_Color& color);

    static auto reflect()
    {
        using namespace entt::literals;
        return ion::reflect<SDL_Color>()
            .func<&convert::encode>("yaml-encode"_hs)
            .func<&convert::decode>("yaml-decode"_hs);
    }
};

template<std::unsigned_integral Integer>
bool ion::parse_hex_rgb(std::string_view text, Integer& r, Integer& g, Integer& b)
{
    static const std::regex hex_pattern{ regex_patterns::hexadecimal_rgb.data() };
    std::cmatch match;
    if (not std::regex_match(text.data(), match, hex_pattern)) {
        return false;
    }
    int ir, ig, ib;
    std::sscanf(match.str(1).c_str(), "%2x%2x%2x", &ir, &ig, &ib);
    r = static_cast<Integer>(ir);
    g = static_cast<Integer>(ig);
    b = static_cast<Integer>(ib);
    return true;
}

template<std::unsigned_integral Integer>
bool ion::parse_hex_rgba(std::string_view text, Integer& r, Integer& g, Integer& b, Integer& a)
{
    static const std::regex hex_pattern{ regex_patterns::hexadecimal_rgba.data() };
    std::cmatch match;
    if (not std::regex_match(text.data(), match, hex_pattern)) {
        return false;
    }
    int ir, ig, ib, ia;
    std::sscanf(match.str(1).c_str(), "%2x%2x%2x%2x", &ir, &ig, &ib, &ia);
    r = static_cast<Integer>(ir);
    g = static_cast<Integer>(ig);
    b = static_cast<Integer>(ib);
    a = static_cast<Integer>(ia);
    return true;
}


inline YAML::Node YAML::convert<SDL_Color>::encode(const SDL_Color& color)
{
    using as_string = convert<std::string>;
    return as_string::encode(ion::write_hex_rgba(color.r, color.g, color.b, color.a));
}

inline bool YAML::convert<SDL_Color>::decode(const Node & node, SDL_Color & color)

{
    if (not node) { return false; }
    if (not node.IsScalar()) { return false; }
    if (ion::parse_hex_rgba(node.Scalar(), color.r, color.g, color.b, color.a)) { return true; }
    if (ion::parse_hex_rgb(node.Scalar(), color.r, color.g, color.b)) { return true; }
    return false;
}
