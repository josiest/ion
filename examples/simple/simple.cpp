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
namespace gl {
constexpr float const points[] = {
    0.0f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
   -0.5f, -0.5f, 0.0f
};
std::string const vertex_shader_source =
"#version 400\n"
"in vec3 vp;"
"void main() {"
"  gl_Position = vec4(vp, 1.0);"
"}";
std::string const fragment_shader_source =
"#version 400\n"
"out vec4 frag_colour;"
"void main() {"
"  frag_colour = vec4(0.5, 0.0, 0.5, 1.0);"
"}";
GLuint vbo = 0;
GLuint vao = 0;
GLuint vertex_shader = 0;
GLuint fragment_shader = 0;
GLuint shader_program = 0;
}
void start()
{
    // initialize vertex buffers and arrays
    glGenBuffers(1, &gl::vbo);
    glBindBuffer(GL_ARRAY_BUFFER, gl::vbo);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), gl::points, GL_STATIC_DRAW);

    glGenVertexArrays(1, &gl::vao);
    glBindVertexArray(gl::vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, gl::vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    // compile shaders
    char const * const source_code[] = {
            gl::vertex_shader_source.c_str(),
            gl::fragment_shader_source.c_str()
    };
    gl::vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(gl::vertex_shader, 1, &source_code[0], nullptr);
    glCompileShader(gl::vertex_shader);

    gl::fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(gl::fragment_shader, 1, &source_code[1], nullptr);
    glCompileShader(gl::fragment_shader);

    gl::shader_program = glCreateProgram();
    glAttachShader(gl::shader_program, gl::fragment_shader);
    glAttachShader(gl::shader_program, gl::vertex_shader);
    glLinkProgram(gl::shader_program);
}
void render(SDL_Window * window)
{
    glUseProgram(gl::shader_program);
    glBindVertexArray(gl::vao);
    // draw points 0-3 from the currently bound VAO with current in-use shader
    glDrawArrays(GL_TRIANGLES, 0, 3);
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
    system->on_start().connect<&start>();
    system->on_render().connect<&render>();
    system->start();
    return EXIT_SUCCESS;
}