#pragma once
#include <concepts>
#include <entt/meta/meta.hpp>
#include <yaml-cpp/node/node.h>

#include "ion/mylar/reflect.hpp"

namespace ion
{
template<typename T>
concept yaml_decodable =
    std::default_initializable<T> and
requires(T& val, const YAML::Node & node)
{
    { YAML::convert<T>::decode(node, val) } -> std::same_as<bool>;
};

YAML::Node encode_unsigned_integer(const entt::meta_any & number);
YAML::Node encode_class(const entt::meta_any & obj);

template<yaml_decodable ValueType>
bool decode_yaml(const YAML::Node & node, entt::meta_any & obj)
{
    ValueType val;
    if (YAML::convert<ValueType>::decode(node, val))
    {
        obj = val;
        return true;
    }
    return false;
}

bool decode_unsigned_integer(const YAML::Node & number, entt::meta_any & obj);
bool decode_with_function(const YAML::Node & node, const entt::meta_func & decode_fn, entt::meta_any & obj);
bool decode_class(const YAML::Node & node, entt::meta_any & obj);
bool decode_scalar(const YAML::Node & node, entt::meta_any & obj);
bool decode_map(const YAML::Node & node, entt::meta_any & obj);
}

template<>
struct YAML::convert<entt::meta_any>
{
    static Node encode(const entt::meta_any & obj);
    static bool decode(const Node & node, entt::meta_any & obj);
};

namespace ion
{
template<reflectable T>
bool meta_decode(const YAML::Node & node, T & val)
{
    using as_any = YAML::convert<entt::meta_any>;

    reflect<T>();
    if (entt::meta_any any_val{ std::in_place_type<T &>, val };
        as_any::decode(node, any_val))
    {
        val = any_val.cast<T>();
        return true;
    }
    return false;
}

template<reflectable T>
requires std::default_initializable<T>
T read_yaml(const YAML::Node & node)
{
    T val;
    meta_decode(node, val);
    return val;
}
}

inline YAML::Node YAML::convert<entt::meta_any>::encode(const entt::meta_any & obj)
{
    const auto type = obj.type();
    if (type.is_integral())
    {
        if (not type.is_signed())
        {
            return ion::encode_unsigned_integer(obj);
        }
    }
    if (type.is_class())
    {
        return ion::encode_class(obj);
    }
    std::printf("Failed to encode because type is currently unsupported\n");
    return Node{};
}

inline bool YAML::convert<entt::meta_any>::decode(const Node & node, entt::meta_any & obj)
{
    if (not node) { return false; }
    if (node.IsScalar()) { return ion::decode_scalar(node, obj); }
    if (node.IsMap()) { return ion::decode_map(node, obj); }
    std::printf("Failed to decode because type is neither scalar nor map (currently unsupported\n");
    return false;
}
