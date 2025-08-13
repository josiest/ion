#pragma once
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <yaml-cpp/yaml.h>

#include <algorithm>

template<>
struct YAML::convert<SDL_Color>
{
    static Node encode(const SDL_Color& rhs)
    {
        Node node;
        node.push_back(rhs.r);
        node.push_back(rhs.g);
        node.push_back(rhs.b);
        if (rhs.a != 0xff)
        {
            node.push_back(rhs.a);
        }
        return node;
    }

    static bool decode(const Node& node, SDL_Color& rhs)
    {
        namespace ranges = std::ranges;
        if (!node.IsSequence() || node.size() < 3 || node.size() > 4
            || !ranges::all_of(node, [](const Node& subnode) { return subnode.IsScalar(); }))
        {
            return false;
        }

        rhs.r = node[0].as<std::uint8_t>();
        rhs.g = node[1].as<std::uint8_t>();
        rhs.b = node[2].as<std::uint8_t>();
        rhs.a = node.size() != 4? 0xff : node[3].as<std::uint8_t>();
        return true;
    }
};

template<>
struct YAML::convert<SDL_Point>
{
    static Node encode(const SDL_Point& rhs)
    {
        Node node;
        node.push_back(rhs.x);
        node.push_back(rhs.y);
        return node;
    }

    static bool decode(const Node& node, SDL_Point& rhs)
    {
        namespace ranges = std::ranges;
        if (!node.IsSequence() || node.size() != 2
                               || !ranges::all_of(node, [](const Node& subnode) { return subnode.IsScalar(); }))
        {
            return false;
        }
        rhs.x = node[0].as<int>();
        rhs.y = node[1].as<int>();
        return true;
    }
};