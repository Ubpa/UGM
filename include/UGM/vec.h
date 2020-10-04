#pragma once

#include "details/traits.h"

#include "Interfaces/IArray/ICross.h"
#include "Interfaces/IArray/IArray1D_Util.h"

namespace Ubpa {
	template<typename T, size_t N>
	struct ImplTraits<vec<T, N>>
		: Array1DTraits<T, N>,
		IListTraits<IArray1D_Util, IEuclideanV> {};

	template<typename T>
	struct ImplTraits<vec<T, 3>>
		: Array1DTraits<T, 3>,
		IListTraits<IArray1D_Util, ICross> {};

	template<typename T, size_t N>
	struct vec : SI<vec<T, N>> {
		using SI<vec<T, N>>::SI;
	};

	template<size_t N>
	using vecf = vec<float, N>;

	using vecf1 = vecf<1>;
	using vecf2 = vecf<2>;
	using vecf3 = vecf<3>;
	using vecf4 = vecf<4>;

	template<size_t N>
	using veci = vec<int, N>;

	using veci1 = veci<1>;
	using veci2 = veci<2>;
	using veci3 = veci<3>;
	using veci4 = veci<4>;

	template<size_t N>
	using vecu = vec<unsigned, N>;

	using vecu1 = vecu<1>;
	using vecu2 = vecu<2>;
	using vecu3 = vecu<3>;
	using vecu4 = vecu<4>;

	// maybe error in editor, but no compile error
	static_assert(sizeof(vecf4) == 4 * sizeof(float));
}
