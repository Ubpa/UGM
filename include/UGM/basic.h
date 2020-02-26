#pragma once

#include <cmath>

namespace Ubpa {
	template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
	constexpr auto EPSILON = static_cast<T>(0.000001);
	template<typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
	constexpr auto PI = static_cast<T>(3.1415926535897932384626433832795);

	namespace detail {
		template<typename T>
		struct rmv_epsilon {
			static const T run(T val) noexcept {
				if (std::abs(std::round(val) - val) < EPSILON<T>)
					return std::round(val + EPSILON<T>); // + epsilon for -0 case
				else
					return val;
			}
		};
	}

	template<typename T>
	const T rmv_epsilon(T val) noexcept {
		return detail::rmv_epsilon<T>::run(val);
	}

	template<typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
	constexpr T to_radian(T degree) noexcept {
		return degree* (PI<T> / static_cast<T>(180));
	}

	template<typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
	constexpr T to_degree(T radian) noexcept {
		return radian * (static_cast<T>(180) / PI<T>);
	}

	enum class Axis : size_t {
		X = 0,
		Y = 1,
		Z = 2
	};

	template<typename T>
	T sgn(T v) noexcept {
		if (v > static_cast<T>(0))
			return static_cast<T>(1);
		else if (v < static_cast<T>(0))
			return static_cast<T>(-1);
		else
			return static_cast<T>(0);
	}
}
