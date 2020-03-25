#pragma once

#include "point.h"
#include "val.h"

namespace Ubpa {
	template<typename T, size_t N>
	struct triangle : SIIT_CRTP<TemplateList<IArrayInOut>, triangle<T, N>, TypeList<TypeList<point<T, N>, Size<3>>, T>> {
		using SIIT_CRTP<TemplateList<IArrayInOut>, triangle<T, N>, TypeList<TypeList<point<T, N>, Size<3>>, T>>::SIIT_CRTP;

		T area() const noexcept {
			static_assert(N == 2 || N == 3);
			static_assert(std::is_floating_point_v<T>);

			auto v01 = (*this)[1] - (*this)[0];
			auto v02 = (*this)[2] - (*this)[0];
			if constexpr (N == 3)
				return static_cast<T>(0.5)* v01.cross(v02).norm();
			else
				return static_cast<T>(0.5)* std::abs(v01[0] * v02[1] - v01[1] - v02[0]);
		}

		template<typename Container>
		const point<T, N> lerp(const Container& wuv) const noexcept {
			assert(wuv.size() == 3);
			return point<T, N>::combine(*this, wuv);
		}
	};

	template<size_t N>
	using trianglef = triangle<float, N>;

	using trianglef2 = trianglef<2>;
	using trianglef3 = trianglef<3>;
}
