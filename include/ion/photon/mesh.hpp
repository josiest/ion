#pragma once
#include <gl/glew.h>
#include <memory>
#include <cstdint>
#include <span>

namespace ion
{
namespace internal
{
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

template<RealNumber Field, std::uint8_t Dim, std::uint32_t N> requires (Dim >= 2 and N >= 3)
class static_mesh
{
    // meta info
public:
    using field_t = Field;
    static constexpr uint32_t size = N;
    static constexpr std::uint8_t dim = Dim;

    // constructors
protected:
    void initialize();
public:
    explicit static_mesh(std::span<const Field, N*Dim> in_vertices, GLenum array_mode = GL_TRIANGLE_STRIP);
    static_mesh();

    // mesh interface
    void set_vertices(std::span<Field, N*Dim> in_vertices);
    void render() const;

    // members
protected:
    static constexpr GLuint VERTEX_START_LOCATION = 0u;
    GLenum array_mode = GL_TRIANGLE_STRIP;

    std::array<Field, Dim*N> vertices;
    vertex_array vao = nullptr;
    vertex_buffer vbo = nullptr;
};

template<std::uint32_t N> requires (N >= 3)
using static_mesh2f = static_mesh<float, 2u, N>;
}

template<ion::RealNumber Field, std::uint8_t Dim, std::uint32_t N> requires (Dim >= 2 && N >= 3)
void ion::static_mesh<Field, Dim, N>::initialize()
{
    // Generate the mesh's buffer and array objects
    vao.reset(new GLuint(0));
    glGenVertexArrays(1, vao.get());

    vbo.reset(new GLuint(0));
    glGenBuffers(1, vbo.get());

    // copy vertex data
    glBindVertexArray(*vao.get());
    glBindBuffer(GL_ARRAY_BUFFER, *vbo.get());
    glBufferData(GL_ARRAY_BUFFER, N*Dim*sizeof(Field), vertices.data(), GL_DYNAMIC_DRAW);

    // specify the format of the vertices
    glVertexAttribPointer(VERTEX_START_LOCATION, Dim, gl_float_type<Field>(), GL_FALSE,
                          Dim*sizeof(Field), (void*)nullptr);
    glEnableVertexAttribArray(VERTEX_START_LOCATION);
}

template<ion::RealNumber Field, std::uint8_t Dim, std::uint32_t N> requires (Dim >= 2 && N >= 3)
ion::static_mesh<Field, Dim, N>::static_mesh(std::span<const Field, N * Dim> in_vertices, GLenum array_mode)
    : array_mode(array_mode)
{
    std::ranges::copy(in_vertices, vertices.begin());
    initialize();
}

template<ion::RealNumber Field, std::uint8_t Dim, std::uint32_t N> requires (Dim >= 2 && N >= 3)
ion::static_mesh<Field, Dim, N>::static_mesh()
{
    initialize();
}

template<ion::RealNumber Field, std::uint8_t Dim, std::uint32_t N> requires (Dim >= 2 && N >= 3)
void ion::static_mesh<Field, Dim, N>::set_vertices(std::span<Field, N * Dim> in_vertices)
{
    std::ranges::copy(in_vertices, vertices.begin());
    glBindBuffer(GL_ARRAY_BUFFER, *vbo.get());
    glBufferSubData(GL_ARRAY_BUFFER, 0, N*Dim*sizeof(Field), vertices.data());
}

template<ion::RealNumber Field, std::uint8_t Dim, std::uint32_t N> requires (Dim >= 2 && N >= 3)
void ion::static_mesh<Field, Dim, N>::render() const
{
    glBindVertexArray(*vao.get());
    glDrawArrays(array_mode, 0, N);
}

