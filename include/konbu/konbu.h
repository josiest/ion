#pragma once

// data types and resource handles
#include <optional>
#include <expected>

// type constraints
#include <concepts>
#include <ranges>

// i/o
#include <sstream>
#include <regex>
#include <yaml-cpp/yaml.h>

namespace konbu {

template<typename container>
concept can_push_back =
requires(container & c, container::value_type const & v)
{
    c.push_back(v);
};
template<typename container>
concept can_push_front =
requires(container & c, container::value_type const & v)
{
    c.push_front(v);
};

template<std::ranges::range container>
auto back_inserter_preference(container & c)
{
    return std::inserter(c, std::ranges::begin(c));
}
template<std::ranges::range container>
requires can_push_back<container>
auto back_inserter_preference(container & c)
{
    return std::back_inserter(c);
}
template<std::ranges::range container>
requires can_push_front<container> and (not can_push_back<container>)
auto back_inserter_preference(container & c)
{
    return std::front_inserter(c);
}

template<typename container>
using lookup_key_t = typename container::key_type;

template <typename container>
using lookup_mapped_t = typename container::mapped_type;

template<typename container>
concept lookup_table =
requires(container const & c, lookup_key_t<container> const & key)
{
    { c.find(key) } -> std::indirectly_readable;
    { c.find(key)->first } -> std::convertible_to<lookup_key_t<container>>;
    { c.find(key)->second } -> std::convertible_to<lookup_mapped_t<container>>;
};

template<std::ranges::output_range<YAML::Exception> error_output,
         lookup_table name_lookup>
requires std::convertible_to<std::string, lookup_key_t<name_lookup>>

void read_lookup(YAML::Node const & config,
                 lookup_mapped_t<name_lookup> & value,
                 name_lookup const & lookup,
                 error_output & errors)
{
    namespace ranges = std::ranges;
    namespace views = std::views;
    if (not config.IsScalar()) {
        YAML::Exception const error{ config.Mark(), "expecting a string" };
        ranges::copy(views::single(error),
                     back_inserter_preference(errors));
        return;
    }
    auto const search = lookup.find(config.as<std::string>());
    if (search != lookup.end()) {
        value = search->second;
        return;
    }
    std::stringstream message;
    message << "expecting value to be one of the following: [";
    std::string sep;
    for (const auto & name : lookup | views::keys) {
        message << sep << name;
        sep = ", ";
    }
    message << "]";
    YAML::Exception const error{ config.Mark(), message.str() };
    ranges::copy(views::single(error),
                 back_inserter_preference(errors));
}

template<std::integral number,
         std::ranges::output_range<YAML::Exception> error_output>
void read(YAML::Node const & config, number & value, error_output & errors)
{
    namespace ranges = std::ranges;
    namespace views = std::views;

    if (not config.IsScalar()) {
        YAML::Exception const error{ config.Mark(), "expecting an integer" };
        ranges::copy(views::single(error),
                     back_inserter_preference(errors));
        return;
    }
    std::regex const negative_pattern{ "^-" };
    if (std::is_unsigned_v<number> and
        std::regex_search(config.Scalar(), negative_pattern)) {

        YAML::Exception const error{ config.Mark(),
                                     "expecting a non-negative integer" };
        ranges::copy(views::single(error),
                     back_inserter_preference(errors));
        return;
    }
    std::regex const integer_pattern{ "-?[0-9]+[ \t]*" };
    if (not std::regex_match(config.Scalar(), integer_pattern)) {
        YAML::Exception const error{ config.Mark(), "expecting an integer" };
        ranges::copy(views::single(error),
                     back_inserter_preference(errors));
        return;
    }
    value = config.as<number>();
}

template<std::floating_point number,
         std::ranges::output_range<YAML::Exception> error_output>
void read(YAML::Node const & config, number & value, error_output & errors)
{
    namespace ranges = std::ranges;
    namespace views = std::views;
    if (not config.IsScalar()) {
        YAML::Exception const error{ config.Mark(), "expecting a number" };
        ranges::copy(views::single(error),
                     back_inserter_preference(errors));
        return;
    }
    std::regex const integer_pattern{ "-?[0-9]+\\.?" };
    std::regex const decimal_pattern{ "-?\\.[0-9]+" };
    std::regex const real_pattern{ "-?[0-9]+\\.[0-9]+" };

    std::string const& scalar_value = config.Scalar();
    if (not std::regex_match(scalar_value, integer_pattern) and
        not std::regex_match(scalar_value, decimal_pattern) and
        not std::regex_match(scalar_value, real_pattern)) {

        YAML::Exception const error{ config.Mark(), "expecting a number" };
        ranges::copy(views::single(error),
                     back_inserter_preference(errors));
        return;
    }
    value = config.as<number>();
}
}