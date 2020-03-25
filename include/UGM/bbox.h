#pragma once

#include "point.h"
#include "vec.h"
#include "val.h"

#include <limits>

namespace Ubpa {
	template<typename T, size_t N>
	struct bbox : SIIT_CRTP<TemplateList<IArrayInOut>, bbox<T, N>, TypeList<TypeList<point<T, N>, Size<2>>, T>> {
		using Base = SIIT_CRTP<TemplateList<IArrayInOut>, bbox<T, N>, TypeList<TypeList<point<T, N>, Size<2>>, T>>;
		using Base::Base;
		
		bbox() noexcept;
		bbox(const point<T, N>& minP, const point<T, N>& maxP) noexcept;
		static const bbox minmax(const point<T, N>& p0, const point<T, N>& p1) noexcept;

		point<T, N>& minP() noexcept;
		const point<T, N>& minP() const noexcept;
		point<T, N>& maxP() noexcept;
		const point<T, N>& maxP() const noexcept;

		bool is_valid() const noexcept;

		const point<T, N> corner(size_t idx) const noexcept;
		const point<T, N> center() const noexcept;
		const vec<T, N> diagonal() const noexcept;
		const T area() const noexcept;
		T volume() const noexcept;
		size_t max_extent() const noexcept;

		using Base::lerp;
		const point<T, N> lerp(const val<T, N>& t) const noexcept;

		const vec<T, N> offset(const point<T, N>& p) const noexcept;

		const bbox combine(const bbox& rhs) const noexcept;
		bbox& combine_to_self(const bbox& rhs) noexcept;

		const bbox combine(const point<T, N>& p) const noexcept;
		bbox& combine_to_self(const point<T, N>& p) noexcept;

		static const bbox intersect(const bbox& lhs, const bbox& rhs) noexcept;
		bbox& intersect_to_self(const bbox& rhs) noexcept;
	};

	template<size_t N>
	using bboxf = bbox<float, N>;

	using bboxf1 = bboxf<1>;
	using bboxf2 = bboxf<2>;
	using bboxf3 = bboxf<3>;

	template<size_t N>
	using bboxi = bbox<int, N>;

	using bboxi1 = bboxi<1>;
	using bboxi2 = bboxi<2>;
	using bboxi3 = bboxi<3>;

	template<size_t N>
	using bboxu = bbox<unsigned, N>;

	using bboxu1 = bboxu<1>;
	using bboxu2 = bboxu<2>;
	using bboxu3 = bboxu<3>;
}

#include "detail/bbox.inl"
