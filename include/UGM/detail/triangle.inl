#pragma once

namespace Ubpa {
	template<typename T, size_t N>
	T triangle<T, N>::area() const noexcept {
		static_assert(N == 2 || N == 3);
		static_assert(std::is_floating_point_v<T>);

		auto v01 = (*this)[1] - (*this)[0];
		auto v02 = (*this)[2] - (*this)[0];
		if constexpr (N == 3)
			return static_cast<T>(0.5)* v01.cross(v02).norm();
		else
			return static_cast<T>(0.5)* std::abs(v01[0] * v02[1] - v01[1] - v02[0]);
	}

	template<typename T, size_t N>
	template<typename Container>
	const point<T, N> triangle<T, N>::lerp(const Container& wuv) const noexcept {
		assert(wuv.size() == 3);
		return point<T, N>::combine(*this, wuv);
	}
}
