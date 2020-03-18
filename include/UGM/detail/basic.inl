#pragma once

#include <type_traits>

namespace Ubpa::detail::Basic {
	template<typename T> struct rmv_epsilon;
	template<typename T> struct is_zero;
	template<typename T> struct is_nan;
	template<typename T, typename F> struct lerp;
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
		return detail::Basic::is_zero<std::decay_t<T>>::run(v);
	}

	template<typename T>
	bool is_nan(T v) noexcept {
		return detail::Basic::is_nan<std::decay_t<T>>::run(v);
	}

	template<typename T, typename F>
	T lerp(T x, T y, F t) noexcept {
		return detail::Basic::lerp<T,F>::run(x, y, t);
	}
}

namespace Ubpa::detail::Basic {
	template<typename T>
	struct rmv_epsilon {
		static const T run(T val) noexcept {
			if (std::abs(std::round(val) - val) < EPSILON<T>)
				return static_cast<T>(std::round(val + EPSILON<T>)); // + epsilon for -0 case
			else
				return val;
		}
	};

	template<typename T, size_t N, template<typename T, size_t N>class V >
	struct rmv_epsilon<V<T, N>> {
		static const V<T, N> run(const V<T, N>& val) noexcept {
			return val.rmv_epsilon();
		}
	};

	template<typename T, template<typename T>class V >
	struct rmv_epsilon<V<T>> {
		static const V<T> run(const V<T>& val) noexcept {
			return val.rmv_epsilon();
		}
	};

	template<typename T>
	struct is_zero {
		static bool run(T v) noexcept {
			return v == static_cast<T>(0);
		}
	};

	template<typename T, size_t N, template<typename T, size_t N>class V >
	struct is_zero<V<T, N>> {
		static bool run(const V<T, N>& val) noexcept {
			return val.is_all_zero();
		}
	};

	template<typename T, template<typename T>class V >
	struct is_zero<V<T>> {
		static bool run(const V<T>& val) noexcept {
			return val.is_all_zero();
		}
	};

	template<typename T>
	struct is_nan {
		static bool run(T val) noexcept {
			return std::isnan(static_cast<float>(val));
		}
	};

	template<typename T, size_t N, template<typename T, size_t N>class V >
	struct is_nan<V<T, N>> {
		static bool run(const V<T, N>& val) noexcept {
			return val.has_nan();
		}
	};

	template<typename T, template<typename T>class V >
	struct is_nan<V<T>> {
		static bool run(const V<T>& val) noexcept {
			return val.has_nan();
		}
	};

	template<typename T, typename F>
	struct lerp {
		static T run(T x, T y, F t) noexcept {
			return x * (static_cast<F>(1) - t) + y * t;
		}
	};

	template<typename T, typename F, template<typename T>class V >
	struct lerp<V<T>, F> {
		static V<T> run(V<T> x, V<T> y, F t) noexcept {
			return V<T>::lerp(x, y, static_cast<T>(t));
		}
	};

	template<typename T, size_t N, typename F, template<typename T, size_t N>class V >
	struct lerp<V<T, N>, F> {
		static V<T, N> run(V<T, N> x, V<T, N> y, F t) noexcept {
			return V<T, N>::lerp(x, y, static_cast<T>(t));
		}
	};
}
