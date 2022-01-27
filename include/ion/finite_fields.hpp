#pragma once

// data types
#include <cstddef>

// data structures
#include <array>

// template types
#include <concepts>
#include <type_traits>

namespace ion {

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

/** A mapping of a finite field to another type. */
template<finite_field Field, typename Value>
using finite_map = std::array<Value, field_size<Field>()>;

/** Get the value of a finite_map at an index.
 *
 * No bounds checking performed.
 */
template<finite_field Field, typename Value>
Value const & field_at(finite_map<Field, Value> const & map, Field idx)
{
    return map[static_cast<std::size_t>(idx)];
}

template<finite_field Field, typename Value>
Value & field_at(finite_map<Field, Value> & map, Field idx)
{
    return map[static_cast<std::size_t>(idx)];
}
}
