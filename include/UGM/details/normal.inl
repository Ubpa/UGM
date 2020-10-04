#pragma once

namespace Ubpa {
	template<typename T>
	const point<T, 2> normal<T>::to_sphere_texcoord() const noexcept {
		assert(this->is_normalized());
		// atan2: https://zh.wikipedia.org/wiki/Atan2
		T u = (std::atan2(-(*this)[0], -(*this)[2]) / PI<float> + 1) / 2;
		T v = 1 - std::acos((*this)[1]) / PI<float>;
		return { u,v };
	}

	template<typename T>
	const vec<T, 3> normal<T>::to_sphere_tangent() const noexcept {
		assert(this->is_normalized());
		if (*this == normal{ 0, 1, 0 })
			return { 1,0,0 };
		else
			return { (*this)[2], 0, -(*this)[0] };
	}
}
