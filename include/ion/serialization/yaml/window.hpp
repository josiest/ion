#pragma once

#ifndef YAML_CPP_STATIC_DEFINE
#define YAML_CPP_STATIC_DEFINE
#endif
#include <yaml-cpp/yaml.h>

namespace ion
{
class hardware_renderer;
}

namespace ion::cereal
{
hardware_renderer read_renderer_from_config(const YAML::Node& config);
}
