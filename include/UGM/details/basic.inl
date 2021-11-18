#pragma once

#include <type_traits>
#include <limits>

namespace Ubpa::details {
	template<typename T>
	concept contains_rmv_epsilon = requires(const T & v) { v.rmv_epsilon(); };
	template<typename T>
	concept contains_is_all_zero = requires(const T & v) { v.is_all_zero(); };
	template<typename T>
	concept contains_is_any_nan = requires(const T & v) { v.is_any_nan(); };
	template<typename T, typename F>
	concept contains_lerp = requires(const T & a, const T& b, const F& t) { a.lerp(b, t); };
	template<typename T>
	concept contains_hadamard_product = requires(const T & a, const T & b) { a.hadamard_product(b); };
	template<typename T>
	concept contains_abs = requires(const T & v) { v.abs(); };
}

namespace Ubpa {
	template<typename T>
	T rmv_epsilon(T val) noexcept {
		if constexpr (details::contains_rmv_epsilon<T>)
			return val.rmv_epsilon();
		else {
			if (std::abs(std::round(val) - val) < Epsilon<T>)
				return static_cast<T>(std::round(val + Epsilon<T>)); // + epsilon for -0 case
			else
				return val;
		}
	}

	template<typename T>
	constexpr T to_radian(T degree) noexcept {
		static_assert(std::is_floating_point_v<T>);
		return degree * (Pi<T> / static_cast<T>(180));
	}

	template<typename T>
	constexpr T to_degree(T radian) noexcept {
		static_assert(std::is_floating_point_v<T>);
		return radian * (static_cast<T>(180) / Pi<T>);
	}

	template<typename T>
	T sgn(T v) noexcept {
		if (v > Zero<T>)
			return One<T>;
		else if (v < Zero<T>)
			return static_cast<T>(-1);
		else
			return Zero<T>;
	}

	template<typename T>
	bool is_all_zero(T v) noexcept {
		if constexpr (details::contains_is_all_zero<T>)
			return v.is_all_zero();
		else
			return v == Zero<T>;
	}

	template<typename T>
	bool is_any_nan(T v) noexcept {
		if constexpr (details::contains_is_any_nan<T>)
			return v.is_any_nan();
		else
			return std::isnan(v);
	}

	template<typename T, typename F>
	T lerp(T x, T y, F t) noexcept {
		if constexpr (details::contains_lerp<T, F>)
			return x.lerp(y, t);
		else if constexpr (std::is_floating_point_v<T> && std::is_floating_point_v<F>)
			return std::lerp(x, y, t);
		else
			return static_cast<T>(std::lerp(static_cast<long double>(x), static_cast<long double>(y), static_cast<long double>(t)));
	}

	template<typename T>
	T hadamard_product(T x, T y) noexcept {
		if constexpr (details::contains_hadamard_product<T>)
			return x.hadamard_product(y);
		else
			return x * y;
	}

	template<typename T>
	T abs(T v) noexcept {
		if constexpr (details::contains_abs<T>)
			return v.abs();
		else
			return std::abs(v);
	}

	template<typename T>
	constexpr T one_epsilon() noexcept {
		static_assert(std::is_floating_point_v<T>);
		return 1 - std::numeric_limits<T>::epsilon();
	}

	template<typename T>
	T rand01() noexcept {
		static_assert(std::is_floating_point_v<T>);
		static std::uniform_real_distribution<T> distribution(Zero<T>, one_epsilon<T>());
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

	template<typename T>
	constexpr T gamma_to_linear(T value) noexcept {
		static_assert(std::is_floating_point_v<T>);
		if (value <= static_cast<T>(0.04045))
			return value / static_cast<T>(12.92);
		else if (value < static_cast<T>(1))
			return std::pow((value + static_cast<T>(0.055)) / static_cast<T>(1.055), static_cast<T>(2.4));
		else
			return std::pow(value, static_cast<T>(2.2));
	}

	template<typename T>
	constexpr T linear_to_gamma(T value) noexcept {
		static_assert(std::is_floating_point_v<T>);
		if (value <= static_cast<T>(0))
			return static_cast<T>(0);
		else if (value <= static_cast<T>(0.0031308))
			return static_cast<T>(12.92) * value;
		else if (value < static_cast<T>(1))
			return static_cast<T>(1.055) * std::pow(value, static_cast<T>(5.0/12.0)) - static_cast<T>(0.055);
		else
			return std::pow(value, static_cast<T>(5.0/11.0));
	}
}
