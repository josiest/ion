#pragma once

#include <memory>
#include <cstdint>

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Surface;

namespace ion
{
namespace internal
{
class sdl_system
{
public:
    sdl_system() = delete;
    explicit sdl_system(std::uint32_t init_flags);
    bool was_init() const { return was_init_; }
private:
    bool was_init_ = false;
};

struct sdl_deleter
{
    void operator()(const sdl_system * sdl) const;
    void operator()(SDL_Window * window) const;
    void operator()(SDL_Renderer * renderer) const;
    void operator()(SDL_Surface * surface) const;
};
}

using sdl_system = std::unique_ptr<internal::sdl_system, internal::sdl_deleter>;
using sdl_window = std::unique_ptr<SDL_Window, internal::sdl_deleter>;
using sdl_renderer = std::unique_ptr<SDL_Renderer, internal::sdl_deleter>;
using sdl_surface = std::unique_ptr<SDL_Surface, internal::sdl_deleter>;

sdl_system init_sdl(std::uint32_t init_flags);
sdl_window create_window(std::string_view name, int width, int height, std::uint32_t window_flags);
sdl_renderer create_renderer(SDL_Window * window);
sdl_surface load_bitmap(std::string_view path);
}