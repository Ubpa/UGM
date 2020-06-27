#pragma once

#include "vec.h"

#include "Interfaces/IArray/IArrayAdd.h"
#include "Interfaces/IMatrix/IMatrixMul.h"
#include "Interfaces/IMatrix/IMatrixInOut.h"
#include "Interfaces/IRing.h"

namespace Ubpa {
	template<typename F, size_t N>
	struct ImplTraits<mat<F, N>>
		: ArrayTraits<vec<F, N>, N, F>,
		IListTraits<IMatrixInOut, IMatrixMul, IArrayLinear, IRing>{};

	template<typename F, size_t N>
	struct mat : SI<mat<F, N>> {
		using SI<mat<F, N>>::SI;
	};

	template<size_t N>
	using matf = mat<float, N>;

	using matf2 = matf<2>;
	using matf3 = matf<3>;
	using matf4 = matf<4>;

	// maybe error in editor, but no compile error
	static_assert(sizeof(matf4) == 16 * sizeof(float));
}
