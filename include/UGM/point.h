#pragma once

#include "vec.h"
#include "Interfaces/IArray/IEuclideanA.h"
#include "Interfaces/IArray/IArrayInOut.h"

namespace Ubpa {
	template<typename T, size_t N>
	struct point : SIIT_CRTP<TemplateList<IArrayInOut, IEuclideanA>, point<T, N>, TypeList<TypeList<T, Size<N>>, T, vec<T, N>>> {
		using SIIT_CRTP<TemplateList<IArrayInOut, IEuclideanA>, point<T, N>, TypeList<TypeList<T, Size<N>>, T, vec<T, N>>>::SIIT_CRTP;
	};

	template<size_t N>
	using pointf = point<float, N>;

	using pointf1 = pointf<1>;
	using pointf2 = pointf<2>;
	using pointf3 = pointf<3>;
	using pointf4 = pointf<4>;
}
