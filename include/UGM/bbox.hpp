#pragma once

#include "val.hpp"
#include "point.hpp"

namespace Ubpa {
	template<typename F, size_t N>
	struct SI_ImplTraits<bbox<F, N>>
		: ArrayTraits<point<F, N>, 2, F>,
		IListTraits<IArrayUtil> {};

	// axis-align bounding box
	template<typename F, size_t N>
	struct bbox : SI<bbox<F, N>> {
		using Base = SI<bbox<F, N>>;
		using Base::Base;
		
		bbox() noexcept;
		bbox(const point<F, N>& minP, const point<F, N>& maxP) noexcept;
		static bbox minmax(const point<F, N>& p0, const point<F, N>& p1) noexcept;

		point<F, N>& minP() noexcept;
		const point<F, N>& minP() const noexcept;
		point<F, N>& maxP() noexcept;
		const point<F, N>& maxP() const noexcept;

		bool is_valid() const noexcept;

		point<F, N> corner(size_t idx) const noexcept;
		point<F, N> center() const noexcept;
		vec<F, N> diagonal() const noexcept;
		F area() const noexcept;
		F volume() const noexcept;
		size_t max_extent() const noexcept;

		using Base::lerp;
		point<F, N> lerp(const val<F, N>& t) const noexcept;

		vec<F, N> offset(const point<F, N>& p) const noexcept;

		bbox combine(const bbox& rhs) const noexcept;
		bbox& combine_to_self(const bbox& rhs) noexcept;

		bbox combine(const point<F, N>& p) const noexcept;
		bbox& combine_to_self(const point<F, N>& p) noexcept;

		static bbox intersect(const bbox& lhs, const bbox& rhs) noexcept;
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

	// maybe error in editor, but no compile error
	static_assert(sizeof(bboxf3) == 6 * sizeof(float));
}

#include "details/bbox.inl"
