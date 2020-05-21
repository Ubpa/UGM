#pragma once

#include "vec.h"

#include "Interfaces/IArray/IArray1D_Util.h"
#include "Interfaces/IArray/IEuclideanA.h"

namespace Ubpa {
	template<typename T, size_t N>
	struct point;

	template<typename T_, size_t N_>
	struct ImplTraits<point<T_, N_>> {
		using IList = TemplateList<IArray1D_Util, IEuclideanA>;
		using T = T_;
		static constexpr size_t N = N_;
		using F = T;
		using V = vec<T, N>;
	};

	template<typename T, size_t N>
	struct point : SI<point<T, N>> {
		using SI<point<T, N>>::SI;

		template<typename Container>
		inline static const point combine(const Container& points, T weight) noexcept;

		template<typename PContainer, typename WContainer>
		inline static const point combine(PContainer points, WContainer weights) noexcept;
	};

	template<size_t N>
	using pointf = point<float, N>;

	using pointf1 = pointf<1>;
	using pointf2 = pointf<2>;
	using pointf3 = pointf<3>;
	using pointf4 = pointf<4>;

	template<size_t N>
	using pointi = point<int, N>;

	using pointi1 = pointi<1>;
	using pointi2 = pointi<2>;
	using pointi3 = pointi<3>;
	using pointi4 = pointi<4>;

	template<size_t N>
	using pointu = point<unsigned, N>;

	using pointu1 = pointu<1>;
	using pointu2 = pointu<2>;
	using pointu3 = pointu<3>;
	using pointu4 = pointu<4>;

	// maybe error in editor, but no compile error
	static_assert(sizeof(pointf3) == sizeof(float) * 3);
}

#include "detail/point.inl"
