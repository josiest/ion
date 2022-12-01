#pragma once
#include "ion/try.hpp"
#include "yaml-cpp/yaml.h"
#include <string>
#include <iostream>

namespace ion {

/**
 * \brief A string-type that supports dot-separated tagging operations
 */
class tag {
public:
    inline tag() = default;

    inline tag(std::string const & str) : _str{ str } {}
    inline tag(char const * str) : _str{ str } {}
    inline std::string string() const { return _str; }
    inline char const * c_str() const { return _str.c_str(); }
    auto operator<=>(tag const & rhs) const = default;
private:
    std::string _str;
};
}
std::ostream & operator<<(std::ostream & os, ion::tag const & tag);
std::istream & operator>>(std::istream & is, ion::tag & tag);

namespace std {
template<>
struct hash<ion::tag>
{
    size_t operator()(ion::tag const& tag) const
    {
        hash<string> strhash;
        return strhash(tag.string());
    }
};
}

namespace YAML
{
template<>
struct expect<ion::tag, Exception> {
    inline Expected<ion::tag> operator()(Node const & node) const noexcept;
};
template<>
struct convert<ion::tag> {
    static inline Node encode(ion::tag const & tag);
    static inline bool decode(Node const & node, ion::tag & tag);
};
inline Expected<ion::tag>
expect<ion::tag, Exception>::operator()(Node const & node) const noexcept
{
    std::string const input = TRY(node.expect<std::string>());
    return ion::tag{ input };
}
inline Node convert<ion::tag>::encode(ion::tag const & tag)
{
    return convert<std::string>::encode(tag.string());
}

inline bool convert<ion::tag>::decode(Node const & node, ion::tag & tag)
{
    auto result = node.expect<ion::tag>();
    if (not result) { throw result.error(); }
    return {};
}
}
