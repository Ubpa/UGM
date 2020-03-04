#pragma once

#include <cmath>

namespace Ubpa {
	template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
	constexpr auto EPSILON = static_cast<T>(0.000001);

	template<typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
	constexpr auto PI = static_cast<T>(3.1415926535897932384626433832795);

	enum class Axis : size_t {
		X = 0,
		Y = 1,
		Z = 2
	};

	template<typename T>
	inline const T rmv_epsilon(T val) noexcept;

	template<typename T>
	inline constexpr T to_radian(T degree) noexcept;

	template<typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
	inline constexpr T to_degree(T radian) noexcept;

	template<typename T>
	inline T sgn(T v) noexcept;

	template<typename T>
	bool is_zero(T v) noexcept;

	template<typename T>
	bool is_nan(T v) noexcept;

	template<typename T, typename F>
	T lerp(T x, T y, F t) noexcept;
}

#include "detail/basic.inl"
