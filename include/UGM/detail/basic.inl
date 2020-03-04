#pragma once

namespace Ubpa::detail::Basic {
	template<typename T> struct rmv_epsilon;
	template<typename T> struct is_zero;
	template<typename T> struct is_nan;
}

namespace Ubpa {
	template<typename T>
	inline const T rmv_epsilon(T val) noexcept {
		return detail::Basic::rmv_epsilon<T>::run(val);
	}

	template<typename T>
	inline constexpr T to_radian(T degree) noexcept {
		static_assert(std::is_floating_point_v<T>);
		return degree * (PI<T> / static_cast<T>(180));
	}

	template<typename T>
	inline constexpr T to_degree(T radian) noexcept {
		static_assert(std::is_floating_point_v<T>);
		return radian * (static_cast<T>(180) / PI<T>);
	}

	template<typename T>
	inline T sgn(T v) noexcept {
		if (v > static_cast<T>(0))
			return static_cast<T>(1);
		else if (v < static_cast<T>(0))
			return static_cast<T>(-1);
		else
			return static_cast<T>(0);
	}

	template<typename T>
	bool is_zero(T v) noexcept {
		return detail::Basic::is_zero<T>::run(v);
	}

	template<typename T>
	bool is_nan(T v) noexcept {
		return detail::Basic::is_nan<T>::run(v);
	}
}

namespace Ubpa::detail::Basic {
	template<typename T>
	struct rmv_epsilon {
		static const T run(T val) noexcept {
			if (std::abs(std::round(val) - val) < EPSILON<T>)
				return std::round(val + EPSILON<T>); // + epsilon for -0 case
			else
				return val;
		}
	};

	template<typename T>
	struct is_zero {
		static bool run(T v) noexcept {
			return v == static_cast<T>(0);
		}
	};

	template<typename T>
	struct is_nan {
		static bool run(T val) noexcept {
			return std::isnan(static_cast<float>(val));
		}
	};
}
