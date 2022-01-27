#pragma once

// data types
#include <cstddef>

// data structures
#include <array>

// template types
#include <concepts>
#include <type_traits>

namespace prefab {

/** An enum type for mapping elements of a finite field. */
template<class Field>
concept finite_field = requires {
    std::is_enum_v<Field>;
    std::convertible_to<std::underlying_type_t<Field>, std::size_t>;
    Field::size;
};

/** The cardinality of a finite_field. */
template<finite_field Field>
constexpr std::size_t field_size()
{
    return static_cast<std::size_t>(Field::size);
}

/** A field value as an index. */
template<finite_field Field>
constexpr std::size_t as_idx(Field value)
{
    return static_cast<std::size_t>(value);
}

/** A mapping of a finite field to another type. */
template<finite_field Field, typename Value>
using finite_map = std::array<Value, field_size<Field>()>;

namespace main_menu {

/** A main menu button finite field declaration. */
enum class button { forage, size };
}
}
