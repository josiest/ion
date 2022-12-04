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
    fs::path const system_config = "../system.yaml";

    std::vector<std::string> system_errors;
    auto system = ion::system::from_config(system_config,
                                           std::back_inserter(system_errors));

    if (not system) {
        std::cout << "Unable to load ion system\n  " << system.error();
        return EXIT_FAILURE;
    }
    ranges::for_each(system_errors, print_error);
    system_errors.clear();
    system->start();
}