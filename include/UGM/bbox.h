#pragma once

#include "val.h"
#include "point.h"

namespace Ubpa {
	template<typename F, size_t N>
	struct ImplTraits<bbox<F, N>>
		: ArrayTraits<point<F, N>, 2, F>,
		IListTraits<IArrayUtil> {};

	// axis-align bounding box
	template<typename F, size_t N>
	struct bbox : SI<bbox<F, N>> {
		using Base = SI<bbox<F, N>>;
		using Base::Base;
		
		inline bbox() noexcept;
		inline bbox(const point<F, N>& minP, const point<F, N>& maxP) noexcept;
		inline static const bbox minmax(const point<F, N>& p0, const point<F, N>& p1) noexcept;

		inline point<F, N>& minP() noexcept;
		inline const point<F, N>& minP() const noexcept;
		inline point<F, N>& maxP() noexcept;
		inline const point<F, N>& maxP() const noexcept;

		inline bool is_valid() const noexcept;

		inline const point<F, N> corner(size_t idx) const noexcept;
		inline const point<F, N> center() const noexcept;
		inline const vec<F, N> diagonal() const noexcept;
		inline const F area() const noexcept;
		inline F volume() const noexcept;
		inline size_t max_extent() const noexcept;

		using Base::lerp;
		inline const point<F, N> lerp(const val<F, N>& t) const noexcept;

		inline const vec<F, N> offset(const point<F, N>& p) const noexcept;

		inline const bbox combine(const bbox& rhs) const noexcept;
		inline bbox& combine_to_self(const bbox& rhs) noexcept;

		inline const bbox combine(const point<F, N>& p) const noexcept;
		inline bbox& combine_to_self(const point<F, N>& p) noexcept;

		static const bbox intersect(const bbox& lhs, const bbox& rhs) noexcept;
		inline bbox& intersect_to_self(const bbox& rhs) noexcept;
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

	// maybe error in editor, but no compile error
	static_assert(sizeof(bboxf3) == 6 * sizeof(float));
}

#include "details/bbox.inl"
