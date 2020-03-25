#pragma once

namespace Ubpa {
	template<typename T>
	const val<T, 2> uniform_in_square() noexcept {
		return { rand01<T>(), rand01<T>() };
	}

	template<typename T>
	const val<T, 2> uniform_on_disk() noexcept {
		T theta = 2 * PI<T> * rand01<T>();
		return { std::cos(theta), std::sin(theta) };
	}

	template<typename T>
	const val<T, 2> uniform_in_disk() noexcept {
		T r = rand01<T>();
		return r * uniform_on_disk<T>();
	}

	template<typename T>
	const val<T, 3> uniform_on_sphere() noexcept {
		T Xi1 = rand01<T>();
		T Xi2 = rand01<T>();

		T phi = 2 * PI<T> * Xi2;
		T t = 2 * std::sqrt(Xi1 * (1 - Xi1));

		T x = std::cos(phi) * t;
		T y = std::sin(phi) * t;
		T z = 1 - 2 * Xi1;

		return { x,y,z };
	}

	template<typename T>
	const val<T, 3> uniform_in_sphere() noexcept {
		T r = rand01<T>();
		return r * uniform_on_sphere<T>();
	}

	// z > 0
	template<typename T, Axis up>
	const val<T, 3> cos_weighted_on_hemisphere() noexcept {
		val<T, 2> ab = uniform_in_disk();
		T c = std::sqrt(ab[0], ab[1]);
		if constexpr (up == Axis::X)
			return { c, ab[0], ab[1] };
		else if constexpr (up == Axis::X)
			return { ab[0], c, ab[1] };
		else // up == Axis::Z
			return { ab[0], ab[1], c };
	}
}
