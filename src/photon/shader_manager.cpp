#include "ion/photon/shader_manager.hpp"
#include <algorithm>

void ion::shader_manager::add_shaders_in_directory(const std::filesystem::path & directory_to_scan,
                                                   const std::filesystem::path & shader_root_dir)
{
    std::vector<shader_data> shader_definitions;
    scan_shader_definitions(directory_to_scan, std::back_inserter(shader_definitions));

    for (const auto & settings : shader_definitions)
    {
        if (auto shader = compile_shader(settings, shader_root_dir))
        {
            shaders.emplace_back(settings, std::move(shader));
        }
    }
}

void ion::shader_manager_component::start()
{

}
