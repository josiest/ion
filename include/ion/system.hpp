#pragma once

// engine
#include "glad/glad.h"
#include "imgui/backends/imgui_impl_sdl.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "ion/try.hpp"

// frameworks
#include <SDL2/SDL.h>
#include <entt/entt.hpp>

// data types and handles
#include <string>
#include <filesystem>

// serialization
#include "yaml-cpp/yaml.h"
#include "konbu/konbu.h"
#include <sstream>
#include <iterator>

// events
#include <vector>
#include <functional>

#include <ranges>
#include <algorithm>

namespace ion {

using flagmap = std::unordered_map<std::string, std::uint32_t>;

class system {
public:
    system() = delete;
    system(system const &) = delete;
    system & operator=(system const &) = delete;

    system(system && temp);
    system & operator=(system && temp);
    ~system();

    //
    // Deserialization
    //

    /**
     * \brief Load SDL from a config table
     * \param config    the SDL config settings
     */
    template<std::weakly_incrementable ErrorOutput>
    requires std::indirectly_writable<ErrorOutput, std::string>

    static std::expected<system, std::string>
    from_config(YAML::Node const & config, ErrorOutput errors);

    /**
     * \brief Load SDL from a file
     * \param config_path   the path to the SDL config
     */
    template<std::weakly_incrementable ErrorOutput>
    requires std::indirectly_writable<ErrorOutput, std::string>

    static std::expected<system, std::string>
    from_config(std::filesystem::path const & config_path, ErrorOutput errors);

    //
    // Logic
    //

    /** Run the main loop. */
    void start();

    //
    // Subsystems
    //
    entt::registry subsystems;

    /** Register a subsystem. */
    template<class subsystem>
    subsystem & add_subsystem();

    template<class subsystem>
    subsystem & get_subsystem();
    template<class subsystem>
    subsystem const & get_subsystem() const;

    template<class subsystem>
    subsystem * try_get_subsystem();
    template<class subsystem>
    subsystem const * try_get_subsystem() const;

    //
    // Events
    //
    inline auto on_render() { return entt::sink{ render_event }; }
    inline auto on_keydown() { return entt::sink{ keydown_event }; }

    //
    // Utility
    //

    constexpr bool operator!() const;

    //
    //  Members
    //
    inline SDL_Window * window() { return _window; }
    inline SDL_Window const * window() const { return _window; }

    inline void * gl_context() { return _gl_context; }
    inline void const * gl_context() const { return _gl_context; }
    inline entt::entity id() { return _id; }
private:
    system(SDL_Window * window, SDL_GLContext gl_context);
    bool moved = false;

    // events
    entt::sigh<void(SDL_Window *)> render_event;
    entt::sigh<void(SDL_Keysym const &)> keydown_event;

    SDL_Window * _window;
    SDL_GLContext _gl_context;
    entt::entity _id;
};

struct init_params {
    std::uint32_t subsystems = SDL_INIT_VIDEO;

    inline static flagmap const subsystem_flags{
        { "timer",              SDL_INIT_TIMER },
        { "audio",              SDL_INIT_AUDIO },
        { "video",              SDL_INIT_VIDEO },
        { "joystick",           SDL_INIT_JOYSTICK },
        { "haptic",             SDL_INIT_HAPTIC },
        { "game-controller",    SDL_INIT_GAMECONTROLLER },
        { "events",             SDL_INIT_EVENTS },
        { "everything",         SDL_INIT_EVERYTHING }
    };
};

struct window_params {
    std::string name = "Window";
    int x = SDL_WINDOWPOS_UNDEFINED;
    int y = SDL_WINDOWPOS_UNDEFINED;
    std::uint32_t width = 640u;
    std::uint32_t height = 480u;
    std::uint32_t flags = 0u;

    inline static flagmap const window_flags{
        { "fullscreen",         SDL_WINDOW_FULLSCREEN },
        { "fullscreen-desktop", SDL_WINDOW_FULLSCREEN_DESKTOP },
        { "opengl",             SDL_WINDOW_OPENGL },
        { "vulkan",             SDL_WINDOW_VULKAN },
        { "metal",              SDL_WINDOW_METAL },
        { "hidden",             SDL_WINDOW_HIDDEN },
        { "borderless",         SDL_WINDOW_BORDERLESS },
        { "resizable",          SDL_WINDOW_RESIZABLE },
        { "minimized",          SDL_WINDOW_MINIMIZED },
        { "maximized",          SDL_WINDOW_MAXIMIZED },
        { "input-grabbed",      SDL_WINDOW_INPUT_GRABBED },
        { "allow-high-dpi",     SDL_WINDOW_ALLOW_HIGHDPI },
    };
};

struct opengl_params{
    std::uint32_t double_buffer = 1;
    std::uint32_t depth_size = 24;
    std::uint32_t stencil_size = 8;

    std::uint32_t flags = 0;
    std::uint32_t major_version = 4;
    std::uint32_t minor_version = 6;

    inline static flagmap const opengl_flags{
        { "debug",              SDL_GL_CONTEXT_DEBUG_FLAG },
        { "forward-compatible", SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG },
        { "robust-access",      SDL_GL_CONTEXT_ROBUST_ACCESS_FLAG },
        { "reset-isolation",    SDL_GL_CONTEXT_RESET_ISOLATION_FLAG }
    };
};

struct imgui_params{
    std::string glsl_version = "#version 150";
};

template<class subsystem>
subsystem & system::add_subsystem()
{
    return subsystems.emplace<subsystem>(_id);
}

template<class subsystem>
subsystem & system::get_subsystem()
{
    return subsystems.get<subsystem>(_id);
}

template<class subsystem>
subsystem const & system::get_subsystem() const
{
    return subsystems.get<subsystem>(_id);
}

template<class subsystem>
subsystem * system::try_get_subsystem()
{
    return subsystems.try_get<subsystem>(_id);
}

template<class subsystem>
subsystem const * system::try_get_subsystem() const
{
    return subsystems.try_get<subsystem>(_id);
}
}

namespace konbu {

template<std::ranges::output_range<YAML::Exception> error_output>
void read(YAML::Node const & config,
          ion::init_params & params,
          error_output & errors)
{
    namespace ranges = std::ranges;
    namespace views = std::views;

    if (not config.IsMap()) {
        YAML::Exception const error{ config.Mark(), "expecting a map" };
        ranges::copy(views::single(error),
                     konbu::back_inserter_preference(errors));
        return;
    }
    using param = ion::init_params;
    if (auto const subsystem_config = config["subsystems"]) {
        std::vector<YAML::Exception> subsystem_errors;
        auto contextualize = [](YAML::Exception const & error) {
            std::stringstream message;
            message << "encountered error reading subsystem setting\n  "
                    << error.msg;
            return YAML::Exception{ error.mark, message.str() };
        };

        read_flags(subsystem_config, params.subsystems,
                   param::subsystem_flags, subsystem_errors);
        ranges::copy(subsystem_errors | views::transform(contextualize),
                     konbu::back_inserter_preference(errors));
    }
}
}

namespace YAML {

template<std::weakly_incrementable ErrorOutput>
requires std::indirectly_writable<ErrorOutput, Exception>

ErrorOutput
decode_flags(Node const & flagname_sequence,
             std::uint32_t & flags,
             ErrorOutput errors,
             ion::flagmap const & as_flag)
{
    namespace ranges = std::ranges;
    namespace views = std::views;
    using namespace std::string_literals;

    std::vector<std::string> flagnames;
    partition_expect<std::string>(flagname_sequence,
        std::back_inserter(flagnames), errors);

    std::uint32_t parsed_flags = 0u;
    auto is_valid = [&as_flag, &parsed_flags](std::string const & name) {
        auto const search = as_flag.find(name);
        if (search != as_flag.end()) {
            parsed_flags |= search->second;
            return true;
        }
        return false;
    };
    auto as_error = [&flagname_sequence](const std::string& name) {
        return Exception{ flagname_sequence.Mark(),
                          "No flag named "s + name  };
    };
    auto invalid_flagnames = ranges::partition(flagnames, is_valid);
    if (parsed_flags != 0u) {
        flags = parsed_flags;
    }
    auto [_, next_errors] = ranges::copy(
        invalid_flagnames | views::transform(as_error), errors);
    return next_errors;
}

void encode_flags(Node & sequence, std::string const & key,
                  std::uint32_t flags, ion::flagmap const & as_flag);

namespace ErrorMsg {

template<std::ranges::range NameRange>
requires std::convertible_to<std::ranges::range_value_t<NameRange>,
                             std::string>
std::string invalid_flagnames(NameRange && flagnames)
{
    std::stringstream err;
    err << "invalid flags: [";
    std::string sep;
    for (std::string const & name : flagnames) {
        err << sep << name;
        sep = ", ";
    }
    err << "]";
    return err.str();
}
};

template<std::weakly_incrementable ErrorOutput>
requires std::indirectly_writable<ErrorOutput, Exception>
struct expect_default<ion::init_params, Exception, ErrorOutput> {
    
    ErrorOutput operator()(const Node& node,
                           ion::init_params & params,
                           ErrorOutput errors) const noexcept
    {
        if (auto const subsystems = node["subsystems"]) {
            errors = decode_flags(subsystems, params.subsystems, errors,
                                  ion::init_params::subsystem_flags);
        }
        return errors;
    }
};
template<>
struct convert<ion::init_params> {
    static Node encode(const ion::init_params& rhs)
    {
        Node node;
        encode_flags(node, "subsystems", rhs.subsystems,
                     ion::init_params::subsystem_flags);
        return node;
    }
};

template<std::weakly_incrementable ErrorOutput>
requires std::indirectly_writable<ErrorOutput, Exception>

struct expect_default<ion::window_params, Exception, ErrorOutput> {
    ErrorOutput operator()(const Node& node,
                           ion::window_params& params,
                           ErrorOutput errors) const noexcept
    {
        using namespace std::string_literals;
        if (not node.IsMap()) {
            *errors++ = Exception{ node.Mark(), ErrorMsg::NOT_A_MAP };
            return errors;
        }
        if (auto const name = node["name"]) {
            errors = name.expect(params.name, errors);
        }
        if (auto const x = node["x"]) {
            errors = x.expect(params.x, errors);
        }
        if (auto const y = node["y"]) {
            errors = y.expect(params.y, errors);
        }
        if (auto const width = node["width"]) {
            errors = width.expect(params.width, errors);
        }
        if (auto const height = node["height"]) {
            errors = height.expect(params.height, errors);
        }
        if (auto const flag_sequence = node["flags"]) {
            errors = decode_flags(flag_sequence, params.flags, errors,
                                  ion::window_params::window_flags);
        }
        return errors;
    }
};
template<>
struct convert<ion::window_params> {
    static Node encode(ion::window_params const & rhs)
    {
        Node node;
        node["name"] = rhs.name;
        node["x"] = rhs.x;
        node["y"] = rhs.y;
        node["width"] = rhs.width;
        node["height"] = rhs.height;
        encode_flags(node, "flags", rhs.flags,
                     ion::window_params::window_flags);
        return node;
    }
};

template<std::weakly_incrementable ErrorOutput>
requires std::indirectly_writable<ErrorOutput, Exception>

struct expect_default<ion::opengl_params, Exception, ErrorOutput> {
    ErrorOutput operator()(const Node& node,
                           ion::opengl_params& rhs,
                           ErrorOutput errors) const noexcept
    {
        if (not node.IsMap()) {
            *errors++ = Exception{ node.Mark(), ErrorMsg::NOT_A_MAP };
            return errors;
        }
        if (auto const double_buffer = node["double-buffer"]) {
            errors = double_buffer.expect(rhs.double_buffer, errors);
        }
        if (auto const depth_size = node["depth-size"]) {
            errors = depth_size.expect(rhs.depth_size, errors);
        }
        if (auto const stencil_size = node["stencil-size"]) {
            errors = stencil_size.expect(rhs.stencil_size, errors);
        }
        if (auto const major_version = node["major-version"]) {
            errors = major_version.expect(rhs.major_version, errors);
        }
        if (auto const minor_version = node["minor-version"]) {
            errors = minor_version.expect(rhs.minor_version, errors);
        }
        if (auto const flag_sequence = node["flags"]) {
            errors = decode_flags(flag_sequence, rhs.flags, errors,
                                  ion::opengl_params::opengl_flags);
        }
        return errors;
    }
};
template<>
struct convert<ion::opengl_params> {
    static Node encode(ion::opengl_params const & rhs)
    {
        Node node;
        node["double-buffer"] = rhs.double_buffer;
        node["depth-size"] = rhs.depth_size;
        node["stencil-size"] = rhs.stencil_size;
        node["major-version"] = rhs.major_version;
        node["minor-version"] = rhs.minor_version;
        encode_flags(node, "flags", rhs.flags,
                     ion::opengl_params::opengl_flags);
        return node;
    }
};

template<std::weakly_incrementable ErrorOutput>
requires std::indirectly_writable<ErrorOutput, Exception>

struct expect_default<ion::imgui_params, Exception, ErrorOutput> {
    ErrorOutput operator()(const Node& node,
                           ion::imgui_params& rhs,
                           ErrorOutput errors) const noexcept
    {
        if (auto const glsl_version = node["glsl-version"]) {
            errors = glsl_version.expect(rhs.glsl_version, errors);
        }
        return errors;
    }
};
template<>
struct convert<ion::imgui_params> {
    static Node encode(ion::imgui_params const & rhs)
    {
        Node node;
        node["glsl-version"] = rhs.glsl_version;
        return node;
    }
};
}

namespace ion::detail {

inline void cleanup(SDL_Window * window,
                    SDL_GLContext gl_context)
{
    if (gl_context) {
        SDL_GL_DeleteContext(gl_context);
    }
    if (window) {
        SDL_DestroyWindow(window);
    }
    SDL_Quit();
}

inline std::expected<void, std::string>
init_sdl(const init_params& params)
{
    if (SDL_Init(params.subsystems) != 0) {
        return std::unexpected(SDL_GetError());
    }
    return {};
}

inline std::expected<SDL_Window*, std::string>
load_window(const window_params& params)
{
    auto const &[name, x, y, width, height, _] = params;

    // window _must_ be opengl
    std::uint32_t const flags = params.flags
                              | SDL_WINDOW_OPENGL;

    SDL_Window * window = SDL_CreateWindow(name.c_str(), x, y,
                                           width, height, flags);
    if (not window) {
        return std::unexpected(SDL_GetError());
    }
    return window;
}
inline std::expected<SDL_GLContext, std::string>
load_opengl(const opengl_params& gl, SDL_Window* window)
{
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, gl.double_buffer);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, gl.depth_size);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, gl.stencil_size);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, gl.flags);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, gl.major_version);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, gl.minor_version);

    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    if (not gl_context) {
        return std::unexpected(SDL_GetError());
    }
    SDL_GL_MakeCurrent(window, gl_context);
    if (not gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
        return std::unexpected("Couldn't initialize glad");
    }
    return gl_context;
}

inline std::expected<void, std::string>
init_imgui(const imgui_params& params,
           SDL_Window* window,
           SDL_GLContext gl_context)
{
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.Fonts->AddFontDefault();

    if (not ImGui_ImplSDL2_InitForOpenGL(window, gl_context)) {
        return std::unexpected("Couldn't initialize imgui SDL2");
    }
    if (not ImGui_ImplOpenGL3_Init(params.glsl_version.c_str())) {
        return std::unexpected("Couldn't initialize imgui OpenGL3");
    }
    return {};
}
}

template<std::weakly_incrementable ErrorOutput>
requires std::indirectly_writable<ErrorOutput, std::string>

std::expected<ion::system, std::string>
ion::system::from_config(const YAML::Node& config, ErrorOutput errors)
{
    namespace ranges = std::ranges;
    namespace views = std::views;

    SDL_Window* window;
    SDL_GLContext gl_context;

    ion::init_params init_params;
    std::vector<YAML::Exception> yaml_errors;

    if (auto const system_config = config["system"]) {
        konbu::read(system_config, init_params, yaml_errors);

        auto contextualize = [](YAML::Exception const & error) {
            std::stringstream message;
            message << "encountered error reading system config\n  "
                    << error.msg;
            return YAML::Exception{ error.mark, message.str() }.what();
        };
        ranges::copy(yaml_errors | views::transform(contextualize), errors);
        yaml_errors.clear();
    }
    TRY_VOID(detail::init_sdl(init_params));

    ion::window_params window_params;
    if (auto const window_config = config["window"]) {
        window_config.expect(window_params, std::back_inserter(yaml_errors));
    }
    ranges::copy(yaml_errors | views::transform(&YAML::Exception::what),
                 errors);
    yaml_errors.clear();
    window = TRY(detail::load_window(window_params),
                 detail::cleanup(nullptr, nullptr));

    ion::opengl_params gl_params;
    if (auto const opengl_config = config["opengl"]) {
        opengl_config.expect(gl_params, std::back_inserter(yaml_errors));
    }
    ranges::copy(yaml_errors | views::transform(&YAML::Exception::what),
                 errors);
    yaml_errors.clear();
    gl_context = TRY(detail::load_opengl(gl_params, window),
                     detail::cleanup(window, nullptr));

    ion::imgui_params imgui_params;
    if (auto const imgui_config = config["imgui"]) {
        imgui_config.expect(imgui_params, std::back_inserter(yaml_errors));
    }
    ranges::copy(yaml_errors | views::transform(&YAML::Exception::what),
                 errors);
    yaml_errors.clear();
    TRY_VOID(detail::init_imgui(imgui_params, window, gl_context),
             detail::cleanup(window, gl_context));

    return system(window, gl_context);
}

template<std::weakly_incrementable ErrorOutput>
requires std::indirectly_writable<ErrorOutput, std::string>

std::expected<ion::system, std::string>
ion::system::from_config(std::filesystem::path const & config_path,
                         ErrorOutput errors)
{
    using namespace std::string_literals;
    namespace fs = std::filesystem;

    if (not fs::exists(config_path)) {
        return std::unexpected("No file named "s + config_path.string());
    }
    YAML::Node const config = YAML::LoadFile(config_path.string());
    if (not config) {
        return std::unexpected("Unable to load config at " +
                               config_path.string());
    }
    return from_config(config, errors);
}