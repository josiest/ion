// game library
#include "muncher.hpp"

// data structures and algorithms
#include <vector>
#include <ranges>

// i/o and resource management
#include <iostream>
#include <memory>

int main()
{
    std::string const config_path = "../assets/muncher.toml";
    auto game_result = muncher::from_config(config_path);

    if (not game_result) {
        std::cerr << "Couldn't load game: " << game_result.error() << "\n";
        return EXIT_FAILURE;
    }

    auto game = std::move(game_result).value();
    SDL_Delay(2000);

    return EXIT_SUCCESS;
}

muncher::muncher(ion::system && system, ion::window && window,
                 ion::renderer && renderer)

    : _system{std::move(system)}, _window{std::move(window)},
      _renderer{std::move(renderer)}
{
}

template<ranges::input_range name_input>
void log_bad_flags(std::string const & flag_type,
                   name_input const & invalid_names)
{
    for (auto const & name : invalid_names) {
        std::cerr << "No " << flag_type << " flag named " << name
                  << ", skipping\n";
    }
}

tl::expected<muncher, std::string>
muncher::from_config(std::string const & config_path)
{
    std::vector<std::string> invalid_names;
    auto system_result = ion::system::from_config(
            config_path,
            std::back_inserter(invalid_names));

    log_bad_flags("subsystem", invalid_names);
    invalid_names.clear();
    if (not system_result) {
        return tl::unexpected(system_result.error());
    }

    auto window_result = ion::window::from_config(
            config_path,
            std::back_inserter(invalid_names));

    log_bad_flags("window", invalid_names);
    invalid_names.clear();
    if (not window_result) {
        return tl::unexpected(window_result.error());
    }

    auto renderer_result = ion::renderer::from_config(
            window_result.value(),
            config_path,
            std::back_inserter(invalid_names));

    log_bad_flags("renderer", invalid_names);
    invalid_names.clear();
    if (not renderer_result) {
        return tl::unexpected(renderer_result.error());
    }

    return muncher{ std::move(system_result).value(),
                    std::move(window_result).value(),
                    std::move(renderer_result).value() };
}
