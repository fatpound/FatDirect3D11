#pragma once

#include <cmath>
#include <cassert>

#include <limits>
#include <numeric>
#include <numbers>
#include <concepts>

namespace fatpound::math
{
    template <typename T>
    concept Number = std::is_integral_v<T> || std::is_floating_point_v<T>;
    
    template <typename T>
    inline auto Square(const T& x)
    {
        return x * x;
    }

    template <std::floating_point T>
    inline constexpr T twoPi = 2.0f * std::numbers::pi_v<T>;

    template <std::floating_point T>
    T WrapAngle(T theta)
    {
        const T modded = std::fmod(theta, twoPi<T>);

        return modded > std::numbers::pi_v<T>
            ? modded - twoPi<T>
            : modded
            ;
    }

    template <typename T>
    T Interpolate(const T& src, const T& dst, float splitRatio)
    {
        return src + (dst - src) * splitRatio;
    }

    template <std::floating_point F>
    constexpr F ToRadians(F deg)
    {
        return deg * std::numbers::pi_v<F> / static_cast<F>(180.0);
    }
}