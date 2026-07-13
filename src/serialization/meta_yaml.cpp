#include "ion/serialization/meta_yaml.hpp"
#include <cstdio>
#include <yaml-cpp/yaml.h>

YAML::Node ion::encode_unsigned_integer(const entt::meta_any & number)
{
    const auto type = number.type();
    if (type.size_of() == sizeof(std::uint8_t))
    {
        return YAML::Node{ number.cast<std::uint8_t>() };
    }
    if (type.size_of() == sizeof(std::uint16_t))
    {
        return YAML::Node{ number.cast<std::uint16_t>() };
    }
    if (type.size_of() == sizeof(std::uint32_t))
    {
        return YAML::Node{ number.cast<std::uint32_t>() };
    }
    if (type.size_of() == sizeof(std::uint64_t))
    {
        return YAML::Node{ number.cast<std::uint64_t>() };
    }
    std::printf("Couldn't encode because no know unsigned integer is %llu bits\n", type.size_of());
    return YAML::Node{};
}

YAML::Node ion::encode_class(const entt::meta_any & obj)
{
    using namespace entt::literals;
    if (auto encoded = obj.invoke("yaml-encode"_hs, obj))
    {
        if (auto * node = encoded.try_cast<YAML::Node>())
        {
            return *node;
        }
        std::printf("yaml-encode didn't return a node\n");
    }
    std::printf("Encountered fatal error when trying to encode\n");
    return YAML::Node{};
}

bool ion::decode_unsigned_integer(const YAML::Node & number, entt::meta_any & obj)
{
    const auto type = obj.type();
    if (type.size_of() == sizeof(std::uint8_t))
    {
        return decode_yaml<std::uint8_t>(number, obj);
    }
    if (type.size_of() == sizeof(std::uint16_t))
    {
        return decode_yaml<std::uint16_t>(number, obj);
    }
    if (type.size_of() == sizeof(std::uint32_t))
    {
        return decode_yaml<std::uint32_t>(number, obj);
    }
    if (type.size_of() == sizeof(std::uint64_t))
    {
        return decode_yaml<std::uint64_t>(number, obj);
    }
    std::printf("Couldn't decode because no known unsigned integer has %llu bits\n", type.size_of());
    return false;
}

bool ion::decode_with_function(const YAML::Node & node, const entt::meta_func & decode_fn, entt::meta_any & obj)
{
    // TODO: add more detail to messages
    if (not decode_fn.is_static())
    {
        std::printf("decode function isn't static\n");
        return false;
    }
    if (decode_fn.arity() != 2)
    {
        std::printf("wrong number of arguments to yaml decode\n");
        return false;
    }
    if (const auto node_arg = decode_fn.arg(0); node_arg != entt::resolve<const YAML::Node &>())
    {
        std::printf("first argument of yaml-decode is not const node ref\n");
        return false;
    }
    if (const auto obj_arg = decode_fn.arg(1); obj_arg != obj.type())
    {
        std::printf("second argument of yaml-decode has incorrect type\n");
        return false;
    }
    if (const auto ret_type = decode_fn.ret(); ret_type != entt::resolve<bool>())
    {
        std::printf("return type of yaml-decode isn't boolean\n");
        return false;
    }
    std::array args{ entt::meta_any{ node }, obj.as_ref() };
    auto decoded = decode_fn.invoke(obj, args.data(), args.size());
    return decoded.cast<bool>();
}

bool ion::decode_class(const YAML::Node & node, entt::meta_any & obj)
{
    using namespace entt::literals;
    if (const auto decode_fn = obj.type().func("yaml-decode"_hs))
    {
        return decode_with_function(node, decode_fn, obj);
    }
    if (obj.type() == entt::resolve<std::string>())
    {
        return ion::decode_yaml<std::string>(node, obj);
    }
    std::printf("Couldn't decode because type isn't yet supported.\n");
    return false;
}

bool ion::decode_scalar(const YAML::Node & node, entt::meta_any & obj)
{
    const auto type = obj.type();
    if (type.is_integral())
    {
        if (not type.is_signed())
        {
            return decode_unsigned_integer(node, obj);
        }
    }
    if (type.is_class())
    {
        return decode_class(node, obj);
    }
    std::printf("Couldn't decode because type is not yet supported\n");
    return false;
}

bool ion::decode_map(const YAML::Node & node, entt::meta_any & obj)
{
    if (not node or not node.IsMap()) { return false; }
    bool success = true;
    using as_any = YAML::convert<entt::meta_any>;
    for (const auto & elem : node)
    {
        const std::string prop_name = elem.first.Scalar();
        const auto prop_id = entt::hashed_string{ prop_name.c_str() };
        auto prop = obj.get(prop_id);
        // TODO: add base case for max recursion depth
        success = success and as_any::decode(elem.second, prop);
        obj.set(prop_id, prop);
    }
    if (success) { return true; }
    std::printf("Encountered fatal error when decoding map value\n");
    return false;
}
