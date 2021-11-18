#pragma once

namespace Ubpa {
	template<typename T>
	val<T, 2> uniform_in_square() noexcept {
		return { rand01<T>(), rand01<T>() };
	}

	template<typename T>
	val<T, 2> uniform_on_disk() noexcept {
		T theta = 2 * Pi<T> * rand01<T>();
		return { std::cos(theta), std::sin(theta) };
	}

	template<typename T>
	val<T, 2> uniform_in_disk() noexcept {
		T r = rand01<T>();
		return r * uniform_on_disk<T>();
	}

	template<typename T>
	val<T, 3> uniform_on_sphere() noexcept {
		T Xi1 = rand01<T>();
		T Xi2 = rand01<T>();

		T phi = 2 * Pi<T> * Xi2;
		T t = 2 * std::sqrt(Xi1 * (1 - Xi1));

		T x = std::cos(phi) * t;
		T y = std::sin(phi) * t;
		T z = 1 - 2 * Xi1;

		return { x,y,z };
	}

	template<typename T>
	val<T, 3> uniform_in_sphere() noexcept {
		T r = rand01<T>();
		return r * uniform_on_sphere<T>();
	}

	// rst[Axis] > 0
	template<typename T, Axis up>
	val<T, 3> cos_weighted_on_hemisphere() noexcept {
		val<T, 2> ab = uniform_in_disk<T>();
		T c = std::sqrt(1 - ab[0]*ab[0] - ab[1]*ab[1]);
		if constexpr (up == Axis::X)
			return { c, ab[0], ab[1] };
		else if constexpr (up == Axis::Y)
			return { ab[0], c, ab[1] };
		else // up == Axis::Z
			return { ab[0], ab[1], c };
	}

	inline svecf sample_GGX_D(float alpha) noexcept {
		auto Xi = uniform_in_square<float>();
		float cos2_sthetam = (1 - Xi[0]) / ((pow2(alpha) - 1) * Xi[0] + 1);
		float cos_sthetam = std::sqrt(cos2_sthetam);
		float sin_sthetam = std::sqrt(1 - cos2_sthetam);
		float phi = 2 * Pi<float> * Xi[1];
		return { sin_sthetam * std::cos(phi), sin_sthetam * std::sin(phi), cos_sthetam };
	}
}
