#pragma once

#include "config.hpp"

#include <cmath>
#include <random>

namespace Ubpa {
	template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
	constexpr auto EPSILON = static_cast<T>(0.001);

	template<typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
	constexpr auto PI = static_cast<T>(3.141592653589793);

	template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
	constexpr auto ZERO = static_cast<T>(0);

	template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
	constexpr auto ONE = static_cast<T>(1);

	enum class Axis : uint8_t {
		X = 0,
		Y = 1,
		Z = 2,
		INVALID = 3,
	};

	template<typename T>
	T rmv_epsilon(T val) noexcept;

	template<typename T>
	constexpr T to_radian(T degree) noexcept;

	template<typename T>
	constexpr T to_degree(T radian) noexcept;

	template<typename T>
	T sgn(T v) noexcept;

	template<typename T>
	bool is_zero(T v) noexcept;

	template<typename T>
	bool is_nan(T v) noexcept;

	template<typename T, typename F>
	T lerp(T x, T y, F t) noexcept;

	// 1 - epsilon, T: float / double
	template<typename T>
	constexpr T one_epsilon() noexcept;

	// range: [0, 1), T: float / double
	template<typename T>
	T rand01() noexcept;

	// range: [0, 2^64-1]
	size_t randi() noexcept;

	template<typename T>
	constexpr T pow2(T x) noexcept;
	template<typename T>
	constexpr T pow5(T x) noexcept;

	template<typename T>
	constexpr T gamma_to_linear(T value) noexcept;

	template<typename T>
	constexpr T linear_to_gamma(T value) noexcept;
}

#include "details/basic.inl"
