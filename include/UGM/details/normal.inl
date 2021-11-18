#pragma once

namespace Ubpa {
	template<typename T>
	point<T, 2> normal<T>::to_sphere_coordinate() const noexcept {
		assert(this->is_normalized());
		// atan2: https://zh.wikipedia.org/wiki/Atan2
		T theta = std::acos((*this)[1]);
		T phi = std::atan2(-(*this)[0], -(*this)[2]) + Pi<float>;
		return { theta,phi };
	}

	template<typename T>
	vec<T, 3> normal<T>::to_sphere_tangent() const noexcept {
		assert(this->is_normalized());
		if (*this == normal{ 0, 1, 0 })
			return { 1,0,0 };
		else
			return { (*this)[2], 0, -(*this)[0] };
	}
}
