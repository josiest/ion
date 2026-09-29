#pragma once
#include <gl/glew.h>
#include <memory>
#include <cstdint>
#include <span>

namespace ion
{
namespace internal
{
static constexpr GLboolean USE_FIXED_POINT_VERTEX = GL_FALSE;

struct vbo_deleter
{
    void operator()(const GLuint * vbo) const;
};

struct vao_deleter
{
    void operator()(const GLuint * vao) const;
};
}

using vertex_buffer = std::unique_ptr<GLuint, internal::vbo_deleter>;
using vertex_array = std::unique_ptr<GLuint, internal::vao_deleter>;

template<typename Field>
concept RealNumber = requires()
{
    std::is_floating_point_v<Field>;
};

template<RealNumber Field>
constexpr GLenum gl_float_type()
{
    if constexpr (std::is_same_v<Field, float>) { return GL_FLOAT; }
    if constexpr (std::is_same_v<Field, double>) { return GL_DOUBLE; }
    return GL_DOUBLE;
}

struct simple_vertex2f
{
    static void bind_attributes();
    using field_t = float;

    float x, y;
};

struct quad_vertex2f
{
    static void bind_attributes();
    using field_t = float;

    float x, y;
    float u, v;
};

template<typename T>
concept shader_vertex = requires
{
    typename T::field_t;
    { T::bind_attributes() };
};

template<shader_vertex Vertex>
using vertex_field_t = Vertex::field_t;

template<shader_vertex Vertex, std::size_t N> requires (N >= 3)
class static_mesh
{
    // meta info
public:
    using field_t = vertex_field_t<Vertex>;
    static constexpr uint32_t size = N;

    // constructors
protected:
    void initialize();
public:
    explicit static_mesh(std::span<const Vertex, N> in_vertices, GLenum array_mode = GL_TRIANGLE_STRIP);
    static_mesh();

    // mesh interface
    void set_vertices(std::span<const Vertex, N> in_vertices);

    void vertices(std::span<const Vertex, N> in_vertices);

    template<std::ranges::input_range VertexInput>
    requires std::same_as<std::ranges::range_value_t<VertexInput>, Vertex>
    void vertices(const VertexInput & in_vertices);

    template<std::output_iterator<Vertex> VertexOutput>
    VertexOutput vertices(VertexOutput into_vertices) const;

    Vertex vertex(std::size_t idx) const;

    void render() const;

    // members
protected:
    void update_vertices();

    static constexpr GLuint VERTEX_START_LOCATION = 0u;
    GLenum array_mode = GL_TRIANGLE_STRIP;

    std::array<Vertex, N> vertices_;
    vertex_array vao = nullptr;
    vertex_buffer vbo = nullptr;
};
}

template<ion::shader_vertex Vertex, std::size_t N> requires (N >= 3)
void ion::static_mesh<Vertex, N>::initialize()
{
    // Generate the mesh's buffer and array objects
    vao.reset(new GLuint(0));
    glGenVertexArrays(1, vao.get());

    vbo.reset(new GLuint(0));
    glGenBuffers(1, vbo.get());

    // copy vertex data
    glBindVertexArray(*vao);
    glBindBuffer(GL_ARRAY_BUFFER, *vbo);
    glBufferData(GL_ARRAY_BUFFER, N*sizeof(Vertex), vertices_.data(), GL_DYNAMIC_DRAW);

    // specify the format of the vertices
    Vertex::bind_attributes();
}

template<ion::shader_vertex Vertex, std::size_t N> requires (N >= 3)
ion::static_mesh<Vertex, N>::static_mesh(std::span<const Vertex, N> in_vertices, GLenum array_mode)
    : array_mode(array_mode)
{
    std::ranges::copy(in_vertices, vertices_.begin());
    initialize();
}

template<ion::shader_vertex Vertex, std::size_t N> requires (N >= 3)
ion::static_mesh<Vertex, N>::static_mesh()
{
    initialize();
}

template<ion::shader_vertex Vertex, std::size_t N> requires (N >= 3)
void ion::static_mesh<Vertex, N>::set_vertices(std::span<const Vertex, N> in_vertices)
{
    std::ranges::copy(in_vertices, vertices_.begin());
    update_vertices();
}

template<ion::shader_vertex Vertex, std::size_t N> requires (N >= 3)
void ion::static_mesh<Vertex, N>::vertices(std::span<const Vertex, N> in_vertices)
{
    std::ranges::copy(in_vertices, vertices_.begin());
    update_vertices();
}

template<ion::shader_vertex Vertex, std::size_t N> requires (N >= 3)
template<std::ranges::input_range VertexInput>
requires std::same_as<std::ranges::range_value_t<VertexInput>, Vertex>
void ion::static_mesh<Vertex, N>::vertices(const VertexInput & in_vertices)
{
    std::ranges::copy(in_vertices, vertices_.begin());
    update_vertices();
}

template<ion::shader_vertex Vertex, std::size_t N> requires (N >= 3)
template<std::output_iterator<Vertex> VertexOutput>
VertexOutput ion::static_mesh<Vertex, N>::vertices(VertexOutput into_vertices) const
{
    return std::ranges::copy(vertices_, into_vertices).out;
}

template<ion::shader_vertex Vertex, std::size_t N> requires (N >= 3)
Vertex ion::static_mesh<Vertex, N>::vertex(std::size_t idx) const
{
    return vertices_[idx];
}

template<ion::shader_vertex Vertex, std::size_t N> requires (N >= 3)
void ion::static_mesh<Vertex, N>::render() const
{
    glBindVertexArray(*vao);
    glDrawArrays(array_mode, 0, N);
}

template<ion::shader_vertex Vertex, std::size_t N> requires (N >= 3)
void ion::static_mesh<Vertex, N>::update_vertices()
{
    glBindBuffer(GL_ARRAY_BUFFER, *vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, N*sizeof(Vertex), vertices_.data());
}

