#pragma once

namespace Ubpa {
	template<typename T>
	quat<T> euler<T>::to_quat() const noexcept {
		// slow
		/*
		auto qX = quat<T>::rotate_with<Axis::X>((*this)[0]);
		auto qY = quat<T>::rotate_with<Axis::Y>((*this)[1]);
		auto qZ = quat<T>::rotate_with<Axis::Z>((*this)[2]);
		return qY * qX * qZ;
		*/

		// fast
		T x = to_radian((*this)[0]) / 2;
		T y = to_radian((*this)[1]) / 2;
		T z = to_radian((*this)[2]) / 2;

		T cX = std::cos(x);
		T cY = std::cos(y);
		T cZ = std::cos(z);

		T sX = std::sin(x);
		T sY = std::sin(y);
		T sZ = std::sin(z);

		T real = cX * cY * cZ + sX * sY * sZ;

		T imagX = sX * cY * cZ + cX * sY * sZ;
		T imagY = cX * sY * cZ - sX * cY * sZ;
		T imagZ = cX * cY * sZ - sX * sY * cZ;

		return { imagX, imagY, imagZ, real };
	}
}
