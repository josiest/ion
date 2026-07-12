#pragma once
#include <string>

inline namespace ion
{
struct paths
{
    static std::string root_dir();
    static std::string config_dir();
};
}