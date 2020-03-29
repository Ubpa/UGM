#pragma once

#include "vec.h"

#include "Interfaces/IArray/IArray1D_Util.h"
#include "Interfaces/IArray/IEuclideanA.h"

namespace Ubpa {
	template<typename T, size_t N>
	struct point : SIIT_CRTP<TemplateList<IArray1D_Util, IEuclideanA>, point<T, N>, TypeList<TypeList<T, Size<N>>, T, vec<T, N>, point<T, N>>> {
		using SIIT_CRTP<TemplateList<IArray1D_Util, IEuclideanA>, point<T, N>, TypeList<TypeList<T, Size<N>>, T, vec<T, N>, point<T, N>>>::SIIT_CRTP;

		template<typename Container>
		static const point combine(const Container& points, T weight) noexcept;

		template<typename PContainer, typename WContainer>
		static const point combine(PContainer points, WContainer weights) noexcept;
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
