// frameworks
#include "glad/glad.h"
#include "imgui/backends/imgui_impl_sdl.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "ion/system.hpp"

#include <ranges>

// data types
#include <unordered_map>
#include <cstdint>

// serialization
#include "yaml-cpp/yaml.h"
#include <filesystem>
#include <iostream>

using namespace std::string_literals;
namespace fs = std::filesystem;
namespace ranges = std::ranges;

namespace ion {

system::system(SDL_Window * window, SDL_GLContext gl_context)
    : _window{ window }, _gl_context{ gl_context },
      _id{ subsystems.create() }
{
}

system::system(system && temp)
    : subsystems{ std::move(temp.subsystems) },
      render_event{ std::move(temp.render_event) },
      keydown_event{ std::move(temp.keydown_event) },

      _window{ temp._window },
      _gl_context{ temp._gl_context }
{
    temp._window = nullptr;
    temp._gl_context = nullptr;
    temp.moved = true;
}

system & system::operator=(system && temp)
{
    if (this != &temp) {
        *this = std::move(temp);
    }
    return *this;
}

system::~system()
{
    if (moved) return;
    detail::cleanup(_window, _gl_context);
    _window = nullptr; _gl_context = nullptr;
}

constexpr bool system::operator!() const
{
    return _window != nullptr and _gl_context != nullptr;
}

void system::start()
{
    bool has_quit = false;
    while (not has_quit) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT
                                    | GL_STENCIL_BUFFER_BIT);

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);

            switch (event.type) {
            case SDL_QUIT:
                has_quit = true;
                break;

            case SDL_WINDOWEVENT:
                switch (event.window.event) {
                case SDL_WINDOWEVENT_RESIZED:
                    int width = event.window.data1;
                    int height = event.window.data2;
                    glViewport(0, 0, width, height);
                }
                break;

            case SDL_KEYDOWN:
                keydown_event.publish(event.key.keysym);
            }
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(_window);
        ImGui::NewFrame();

        render_event.publish(_window);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        SDL_GL_SwapWindow(_window);
    }
}

}

namespace YAML {

void encode_flags(Node & sequence, std::string const & key,
                  std::uint32_t mask, ion::flagmap const & as_flag)
{
    for (auto const & [name, flag] : as_flag) {
        if ((flag & mask) != 0) {
            sequence[key].push_back(name);
        }
    }
}
}
