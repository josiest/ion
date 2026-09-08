#pragma once
#include "ion/engine/engine_component.hpp"
#include "ion/photon/shader.hpp"
#include "ion/konbu/filesystem.hpp"

#include <gl/glew.h>

#include <optional>
#include <concepts>
#include <string_view>
#include <filesystem>
#include <ranges>

namespace ion
{

template<std::output_iterator<shader_data> ShaderDataOutput>
ShaderDataOutput scan_shader_definitions(std::string_view root_dir, ShaderDataOutput into_shader_data)
{
    namespace fs = std::filesystem; namespace vies = std::views;
    if (not fs::exists(root_dir) or not fs::is_directory(root_dir)) { return into_shader_data; }
    for (const auto & path : fs::directory_iterator(root_dir) | std::views::transform(&fs::directory_entry::path))
    {
        if (path.extension().generic_string() != ".yml") { continue; }
        if (auto loaded_shader = load_shader_data(path)) { *into_shader_data++ = *loaded_shader; }
    }
    return into_shader_data;
}
template<typename T>
concept shader_like = requires(T shader)
{
    requires std::constructible_from<T, GLuint>;
    requires std::convertible_to<decltype(T::shader_name), std::string_view>;
    shader.use_program();
};

class shader_manager
{
public:
    template<shader_like ShaderType>
    std::optional<ShaderType> find_shader();

    template<shader_like ShaderType>
    std::optional<ShaderType> add_shader(const shader_data & settings);

    void add_shaders_in_directory(std::string_view path);
private:
    struct shader_entry
    {
        shader_data info;
        shader_program program;
    };
    std::vector<shader_entry> shaders;
};

class shader_manager_component : public IEngineComponent
{
public:
    shader_manager shaders;

    void start() override;
};
}

template<ion::shader_like ShaderType>
std::optional<ShaderType> ion::shader_manager::find_shader()
{
    auto matches_name = [](const shader_data & data) { return data.name == ShaderType::shader_name; };
    const auto search = std::ranges::find_if(shaders, matches_name, &shader_entry::info);
    return search != shaders.end()
        ? std::optional<ShaderType>(std::in_place, *search->program)
        : std::nullopt;
}

template<ion::shader_like ShaderType>
std::optional<ShaderType> ion::shader_manager::add_shader(const shader_data & settings)
{
    const auto vertex_source = internal::read_file(settings.vertex_path);
    const auto fragment_source = internal::read_file(settings.fragment_path);
    if (not vertex_source or not fragment_source) { return std::nullopt; }

    auto program = compile_shader(*vertex_source, *fragment_source);
    if (not program) { return std::nullopt; }

    const auto & entry = shaders.emplace_back(settings, std::move(program));
    return std::optional<ShaderType>(std::in_place, *entry.program);
}
