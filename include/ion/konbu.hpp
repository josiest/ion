#pragma once

// reflection and serialization
#include <entt/meta/factory.hpp>
#ifndef YAML_CPP_STATIC_DEFINE
#define YAML_CPP_STATIC_DEFINE
#endif
#include <yaml-cpp/yaml.h>
#include <SDL2/SDL_log.h>

// data types
#include <concepts>
#include <string_view>
#include <filesystem>

#include <SDL2/SDL_pixels.h>

// logging

namespace ion
{
namespace _globals
{
constexpr std::string_view DEFAULT_RESOURCE_DIR = "resources";
}

template<typename T>
constexpr bool is_yaml_encodable = requires(T value)
{
    { YAML::convert<T>::encode(value) } -> std::same_as<YAML::Node>;
};

template<typename T>
constexpr bool is_yaml_decodable = requires(YAML::Node node, T value)
{
    { YAML::convert<T>::decode(node, value) } -> std::same_as<bool>;
};

template<typename T>
concept yaml_convertible = is_yaml_encodable<T> and is_yaml_decodable<T>;

template<std::default_initializable T>
bool decode(const YAML::Node& config, T& value)
{
    reflect<T>();
    auto reflection = entt::forward_as_meta(T{});
    const auto type = reflection.type();

    for (const auto member_pair : config)
    {
        const entt::hashed_string member_name{ member_pair.first.Scalar().c_str() };
        const YAML::Node member_config = member_pair.second;

        const auto member_data = type.data(member_name);
        const auto member_type = member_data.type();

        if (member_type.is_integral() and not member_type.is_signed())
        {
            if (member_type.size_of() == sizeof(std::uint8_t))
            {
                member_data.set(reflection, member_config.as<std::uint8_t>());
            }
        }
        else if (member_type == entt::resolve<SDL_Color>())
        {
            member_data.set(reflection, member_config.as<SDL_Color>());
        }
    }
    value = reflection.template cast<T>();
    return true;
}

class asset_loader
{
public:
    inline asset_loader() = default;
    inline explicit asset_loader(std::string_view root_path)
        : asset_root_path{ root_path }
    {
    }

    template<std::default_initializable T>
    T from_file(std::string_view asset_path)
    {
        auto filepath = asset_root_path/asset_path;
        filepath.replace_extension(".yaml");
        try
        {
            return from_config<T>(YAML::LoadFile(filepath.string()));
        }
        catch (const YAML::Exception& error)
        {
            SDL_Log("Couldn't read asset: %s\n", error.what());
            return T{};
        }
    }

    template<std::default_initializable T>
    T from_config(const YAML::Node& config)
    {
        if (config.IsScalar())
        {
            return from_file<T>(config.Scalar());
        }

        T value;
        if (config.IsMap())
        {
            decode(config, value);
        }

        return value;
    }

    template<yaml_convertible T>
    T from_config(const YAML::Node& config)
    {
        return config.as<T>();
    }

    std::filesystem::path asset_root_path = _globals::DEFAULT_RESOURCE_DIR;
};
}
