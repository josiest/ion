#pragma once
#include <yaml-cpp/yaml.h>

namespace ion
{
struct project_settings;
struct engine_settings;
struct window_settings;

YAML::Node load_project_config();
project_settings load_project_settings(const YAML::Node & project_config);
engine_settings load_engine_settings(const YAML::Node & engine_config);
window_settings load_window_settings(const YAML::Node & window_config);
}
