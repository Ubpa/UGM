#pragma once

#include "point.h"
#include "val.h"

namespace Ubpa {
	template<typename T, size_t N>
	struct ImplTraits<triangle<T, N>>
		: ArrayTraits<point<T, N>, 3, T>,
		IListTraits<IArrayInOut> {};

	template<typename T, size_t N>
	struct triangle : SI<triangle<T, N>> {
		using SI<triangle<T, N>>::SI;

		T area() const noexcept;

		template<typename Container>
		const point<T, N> lerp(const Container& wuv) const noexcept;
	};

	template<size_t N>
	using trianglef = triangle<float, N>;

	using trianglef2 = trianglef<2>;
	using trianglef3 = trianglef<3>;

	// maybe error in editor, but no compile error
	static_assert(sizeof(trianglef3) == 9 * sizeof(float));
}

#include "details/triangle.inl"
