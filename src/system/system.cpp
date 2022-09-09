#include "ion/system/system.hpp"

namespace ion {

system::~system()
{
    if (not moved) {
        SDL_Quit();
    }
    SDL_Quit();
}

tl::expected<system, std::string>
system::with_subsystems(std::uint32_t flags)
{
    if (SDL_Init(flags) != 0) {
        return tl::unexpected{SDL_GetError()};
    }
    return system{};
}

tl::expected<system, std::string>
system::with_defaults(std::uint32_t flags)
{
    return system::with_subsystems(SDL_INIT_VIDEO | SDL_INIT_AUDIO | flags);
}
}
