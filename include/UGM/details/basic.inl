#pragma once

#include <type_traits>
#include <limits>

namespace Ubpa::detail::Basic {
	template<typename T> struct rmv_epsilon;
	template<typename T> struct is_zero;
	template<typename T> struct is_nan;
	template<typename T, typename F> struct lerp;
}

namespace Ubpa {
	template<typename T>
	T rmv_epsilon(T val) noexcept {
		return detail::Basic::rmv_epsilon<T>::run(val);
	}

	template<typename T>
	constexpr T to_radian(T degree) noexcept {
		static_assert(std::is_floating_point_v<T>);
		return degree * (PI<T> / static_cast<T>(180));
	}

	template<typename T>
	constexpr T to_degree(T radian) noexcept {
		static_assert(std::is_floating_point_v<T>);
		return radian * (static_cast<T>(180) / PI<T>);
	}

	template<typename T>
	T sgn(T v) noexcept {
		if (v > ZERO<T>)
			return ONE<T>;
		else if (v < ZERO<T>)
			return static_cast<T>(-1);
		else
			return ZERO<T>;
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

	template<typename T>
	constexpr T one_epsilon() noexcept {
		static_assert(std::is_floating_point_v<T>);
		return 1 - std::numeric_limits<T>::epsilon();
	}

	template<typename T>
	T rand01() noexcept {
		static_assert(std::is_floating_point_v<T>);
		static std::uniform_real_distribution<T> distribution(ZERO<T>, one_epsilon<T>());
		static std::default_random_engine engine;
		return distribution(engine);
	}

	inline size_t randi() noexcept {
		static std::uniform_int_distribution<size_t> sMap;
		static std::default_random_engine engine;
		return sMap(engine);
	}

	template<typename T>
	constexpr T pow2(T x) noexcept {
		return x * x;
	}

	template<typename T>
	constexpr T pow5(T x) noexcept {
		T x2 = pow2(x);
		return x2 * x2 * x;
	}
}

namespace Ubpa::detail::Basic {
	template<typename T>
	struct rmv_epsilon {
		static T run(T val) noexcept {
			if (std::abs(std::round(val) - val) < EPSILON<T>)
				return static_cast<T>(std::round(val + EPSILON<T>)); // + epsilon for -0 case
			else
				return val;
		}
	};

	template<typename T, size_t N, template<typename, size_t>class V >
	struct rmv_epsilon<V<T, N>> {
		static V<T, N> run(const V<T, N>& val) noexcept {
			return val.rmv_epsilon();
		}
	};

	template<typename T, template<typename>class V >
	struct rmv_epsilon<V<T>> {
		static V<T> run(const V<T>& val) noexcept {
			return val.rmv_epsilon();
		}
	};

	template<typename T>
	struct is_zero {
		static bool run(T v) noexcept {
			return v == ZERO<T>;
		}
	};

	template<typename T, size_t N, template<typename, size_t>class V >
	struct is_zero<V<T, N>> {
		static bool run(const V<T, N>& val) noexcept {
			return val.is_all_zero();
		}
	};

	template<typename T, template<typename>class V >
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

	template<typename T, size_t N, template<typename, size_t>class V >
	struct is_nan<V<T, N>> {
		static bool run(const V<T, N>& val) noexcept {
			return val.has_nan();
		}
	};

	template<typename T, template<typename>class V >
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

	template<typename T, typename F, template<typename>class V >
	struct lerp<V<T>, F> {
		static V<T> run(V<T> x, V<T> y, F t) noexcept {
			return V<T>::lerp(x, y, static_cast<T>(t));
		}
	};

	template<typename T, size_t N, typename F, template<typename, size_t>class V >
	struct lerp<V<T, N>, F> {
		static V<T, N> run(V<T, N> x, V<T, N> y, F t) noexcept {
			return V<T, N>::lerp(x, y, static_cast<T>(t));
		}
	};
}
