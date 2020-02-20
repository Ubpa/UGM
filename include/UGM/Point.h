#pragma once

#include "Vec.h"
#include "Interfaces/IArray/IEuclideanA.h"

namespace Ubpa {
	template<typename T, size_t N>
	struct Point : SIIT_CRTP<TemplateList<IEuclideanA>, Point<T, N>, TypeList<T, Size<N>, Vec<T, N>>> {
		using SIIT_CRTP<TemplateList<IEuclideanA>, Point<T, N>, TypeList<T, Size<N>, Vec<T, N>>>::SIIT_CRTP;
	};

	template<size_t N>
	using Pointf = Point<float, N>;

	using Pointf1 = Pointf<1>;
	using Pointf2 = Pointf<2>;
	using Pointf3 = Pointf<3>;
	using Pointf4 = Pointf<4>;
}
