// frameworks
#include "ion/system.hpp"

// i/o
#include <filesystem>
#include <iostream>

// data types and data structures
#include <vector>
#include <string>

namespace fs = std::filesystem;
namespace ranges = std::ranges;

void print_error(std::string const & error)
{
    std::cout << error << "\n\n";
}

int main()
{
    namespace views = std::views;
    fs::path const system_config = "../examples/simple/system.yaml";

    std::vector<YAML::Exception> config_errors;
    auto system = ion::system::from_config(system_config, config_errors);

    ranges::for_each(config_errors | views::transform(&YAML::Exception::what),
                     print_error);
    config_errors.clear();

    if (not system) {
        std::cout << "Unable to load ion system\n  " << system.error();
        return EXIT_FAILURE;
    }
    system->start();
    return EXIT_SUCCESS;
}