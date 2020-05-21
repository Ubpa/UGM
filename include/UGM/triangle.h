#pragma once

#include "point.h"
#include "val.h"

namespace Ubpa {
	template<typename T, size_t N>
	struct triangle;

	template<typename T_, size_t N_>
	struct ImplTraits<triangle<T_, N_>> {
		using IList = TemplateList<IArrayInOut>;
		using T = point<T_, N_>;
		static constexpr size_t N = 3;
		using F = T_;
	};

	template<typename T, size_t N>
	struct triangle : SI<triangle<T, N>> {
		using SI<triangle<T, N>>::SI;

		inline T area() const noexcept;

		template<typename Container>
		inline const point<T, N> lerp(const Container& wuv) const noexcept;
	};

	template<size_t N>
	using trianglef = triangle<float, N>;

	using trianglef2 = trianglef<2>;
	using trianglef3 = trianglef<3>;

	// maybe error in editor, but no compile error
	static_assert(sizeof(trianglef3) == 9 * sizeof(float));
}

#include "detail/triangle.inl"
