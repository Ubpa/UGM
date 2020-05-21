#pragma once

#include "val.h"

namespace Ubpa {
	// axis-align bounding box
	template<typename T, size_t N>
	struct bbox;

	template<typename T_, size_t N_>
	struct ImplTraits<bbox<T_, N_>> {
		using IList = TemplateList<IArrayUtil>;
		using T = point<T_, N_>;
		static constexpr size_t N = 2;
		using F = T_;
	};

	template<typename T, size_t N>
	struct bbox : SI<bbox<T, N>> {
		using Base = SI<bbox<T, N>>;
		using Base::Base;
		
		inline bbox() noexcept;
		inline bbox(const point<T, N>& minP, const point<T, N>& maxP) noexcept;
		inline static const bbox minmax(const point<T, N>& p0, const point<T, N>& p1) noexcept;

		inline point<T, N>& minP() noexcept;
		inline const point<T, N>& minP() const noexcept;
		inline point<T, N>& maxP() noexcept;
		inline const point<T, N>& maxP() const noexcept;

		inline bool is_valid() const noexcept;

		inline const point<T, N> corner(size_t idx) const noexcept;
		inline const point<T, N> center() const noexcept;
		inline const vec<T, N> diagonal() const noexcept;
		inline const T area() const noexcept;
		inline T volume() const noexcept;
		inline size_t max_extent() const noexcept;

		using Base::lerp;
		inline const point<T, N> lerp(const val<T, N>& t) const noexcept;

		inline const vec<T, N> offset(const point<T, N>& p) const noexcept;

		inline const bbox combine(const bbox& rhs) const noexcept;
		inline bbox& combine_to_self(const bbox& rhs) noexcept;

		inline const bbox combine(const point<T, N>& p) const noexcept;
		inline bbox& combine_to_self(const point<T, N>& p) noexcept;

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

#include "detail/bbox.inl"
