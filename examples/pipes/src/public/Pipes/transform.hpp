#pragma once
#include <concepts>
#include <numbers>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>

namespace ion
{
template<std::floating_point Float>
class transform2
{
public:
    using matrix_t = glm::mat<3, 3, Float, glm::defaultp>;
    using vector_t = glm::vec<2, Float, glm::defaultp>;

    transform2() = default;
    transform2(transform2<Float> & parent);

    template<typename NumericX, typename NumericY>
    requires std::is_arithmetic_v<NumericX> and std::is_arithmetic_v<NumericY>
    void scale_by(NumericX x, NumericY y);

    template<typename NumericX, typename NumericY>
    requires std::is_arithmetic_v<NumericX> and std::is_arithmetic_v<NumericY>
    void translate(NumericX x, NumericY y);

    template<typename Numeric>
    requires std::is_arithmetic_v<Numeric>
    void rotate(Numeric theta);

    matrix_t local_basis() const;
    matrix_t global_basis() const;

    vector_t translation{ 0.f, 0.f };
    vector_t scale{ 1.f, 1.f };
    Float rotation = 0.f;

    transform2<Float> * parent = nullptr;
};

using transform2f = transform2<float>;
using transform2d = transform2<double>;
using transform2l = transform2<long double>;
}

template<std::floating_point Float>
ion::transform2<Float>::transform2(transform2<Float> & parent)
    : parent(&parent)
{
}

template<std::floating_point Float>
template<typename NumericX, typename NumericY>
requires std::is_arithmetic_v<NumericX> and std::is_arithmetic_v<NumericY>
void ion::transform2<Float>::scale_by(NumericX x, NumericY y)
{
    scale.x *= static_cast<Float>(x);
    scale.y *= static_cast<Float>(y);
}

template<std::floating_point Float>
template<typename NumericX, typename NumericY>
requires std::is_arithmetic_v<NumericX> and std::is_arithmetic_v<NumericY>
void ion::transform2<Float>::translate(NumericX x, NumericY y)
{
    translation += vector_t{ static_cast<Float>(x), static_cast<Float>(y) };
}

template<std::floating_point Float>
template<typename Numeric>
requires std::is_arithmetic_v<Numeric>
void ion::transform2<Float>::rotate(Numeric theta)
{
    rotation += theta;
    rotation = std::fmod(static_cast<Float>(theta), static_cast<Float>(360));
}

template<std::floating_point Float>
ion::transform2<Float>::matrix_t ion::transform2<Float>::local_basis() const
{
    auto basis = glm::translate(matrix_t(1), translation);
    basis = glm::scale(basis, scale);
    basis = glm::rotate(basis, glm::radians(rotation));
    return basis;
}

template<std::floating_point Float>
ion::transform2<Float>::matrix_t ion::transform2<Float>::global_basis() const
{
    auto basis = local_basis();
    const auto * parent_transform = parent;
    while (parent_transform != nullptr)
    {
        // if transform hierarchy is A -> B -> C, we want matrix multiplication C * B * A
        basis = basis * parent_transform->local_basis();
        parent_transform = parent_transform->parent;
    }
    return basis;
}