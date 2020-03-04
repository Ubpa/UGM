#pragma once

#include "point.h"
#include "vec.h"

#include <limits>

namespace Ubpa {
	template<typename T, size_t N>
	struct bbox : SIIT_CRTP<TemplateList<IArrayUtil>, bbox<T, N>, TypeList<TypeList<point<T, N>, Size<2>>, T>> {
		using Base = SIIT_CRTP<TemplateList<IArrayUtil>, bbox<T, N>, TypeList<TypeList<point<T, N>, Size<2>>, T>>;
		using Base::Base;
		
		inline bbox();
		inline bbox(const point<T, N>& minP, const point<T, N>& maxP);
		static const bbox minmax(const point<T, N>& p0, const point<T, N>& p1);

		point<T, N>& minP() noexcept { return (*this)[0]; }
		const point<T, N>& minP() const noexcept { return (*this)[0]; }
		point<T, N>& maxP() noexcept { return (*this)[1]; }
		const point<T, N>& maxP() const noexcept { return (*this)[1]; }

		bool is_valid() const;

		const point<T, N> corner(size_t idx) const;
		const point<T, N> center() const;
		const vec<T, N> diagonal() const { return maxP() - minP(); }
		const T area() const;
		T volume() const;
		size_t max_extent() const;

		using Base::lerp;
		const point<T, N> lerp(const val<T, N>& t) const;

		const vec<T, N> offset(const point<T, N>& p) const;

		const bbox combine(const bbox& rhs) const;
		bbox& combine_with(const bbox& rhs);

		const bbox combine(const point<T, N>& p) const;
		bbox& combine_with(const point<T, N>& p);

		static const bbox intersect(const bbox& lhs, const bbox& rhs);
		bbox& intersect_with(const bbox& rhs);
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
