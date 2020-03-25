#pragma once

#include "point.h"
#include "val.h"

namespace Ubpa {
	template<typename T, size_t N>
	struct triangle : SIIT_CRTP<TemplateList<IArrayInOut>, triangle<T, N>, TypeList<TypeList<point<T, N>, Size<3>>, T>> {
		using SIIT_CRTP<TemplateList<IArrayInOut>, triangle<T, N>, TypeList<TypeList<point<T, N>, Size<3>>, T>>::SIIT_CRTP;

		T area() const noexcept;

		template<typename Container>
		const point<T, N> lerp(const Container& wuv) const noexcept;
	};

	template<size_t N>
	using trianglef = triangle<float, N>;

	using trianglef2 = trianglef<2>;
	using trianglef3 = trianglef<3>;
}

#include "detail/triangle.inl"
