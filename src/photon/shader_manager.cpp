#include "ion/photon/shader_manager.hpp"
#include <algorithm>

void ion::shader_manager::add_shaders_in_directory(std::string_view path)
{
    std::vector<shader_data> shader_definitions;
    scan_shader_definitions(path, std::back_inserter(shader_definitions));

    for (const auto & settings : shader_definitions)
    {
        if (auto shader = compile_shader(settings))
        {
            shaders.emplace_back(settings, std::move(shader));
        }
    }
}

void ion::shader_manager_component::start()
{

}
