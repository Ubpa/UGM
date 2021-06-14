#pragma once

#include "vec.hpp"

#include "Interfaces/IArray/IArrayAdd.hpp"
#include "Interfaces/IMatrix/IMatrixMul.hpp"
#include "Interfaces/IMatrix/IMatrixInOut.hpp"
#include "Interfaces/IRing.hpp"

namespace Ubpa {
	template<typename F, size_t N>
	struct SI_ImplTraits<mat<F, N>>
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

	template<size_t N>
	using matd = mat<double, N>;

	using matd2 = matd<2>;
	using matd3 = matd<3>;
	using matd4 = matd<4>;

	// maybe error in editor, but no compile error
	static_assert(sizeof(matf4) == 16 * sizeof(float));
}
