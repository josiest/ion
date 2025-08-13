#pragma once
#include <yaml-cpp/yaml.h>

namespace ion
{
class hardware_renderer;
}

namespace ion::cereal
{
hardware_renderer read_renderer_from_config(const YAML::Node& config);
}
