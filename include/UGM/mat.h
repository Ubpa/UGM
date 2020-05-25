#pragma once

#include "vec.h"

#include "Interfaces/IArray/IArrayAdd.h"
#include "Interfaces/IMatrix/IMatrixMul.h"
#include "Interfaces/IMatrix/IMatrixInOut.h"
#include "Interfaces/IRing.h"

namespace Ubpa {
	template<typename T, size_t N>
	struct ImplTraits<mat<T, N>>
		: ArrayTraits<vec<T, N>, N, T>,
		IListTraits<IMatrixInOut, IMatrixMul, IArrayLinear, IRing>{};

	template<typename T, size_t N>
	struct mat : SI<mat<T, N>> {
		using SI<mat<T, N>>::SI;
	};

	template<size_t N>
	using matf = mat<float, N>;

	using matf3 = matf<3>;
	using matf4 = matf<4>;

	// maybe error in editor, but no compile error
	static_assert(sizeof(matf4) == 16 * sizeof(float));
}
