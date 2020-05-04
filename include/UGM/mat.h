#pragma once

#include "vec.h"

#include "Interfaces/IArray/IArrayAdd.h"
#include "Interfaces/IMatrix/IMatrixMul.h"
#include "Interfaces/IMatrix/IMatrixInOut.h"
#include "Interfaces/IRing.h"

namespace Ubpa {
	template<typename T, size_t N>
	struct mat : SI<TemplateList<IMatrixInOut, IMatrixMul, IArrayLinear, IRing>, mat<T, N>, TypeList<vec<T, N>, Size<N>>, T> {
		using SI<TemplateList<IMatrixInOut, IMatrixMul, IArrayLinear, IRing>, mat<T, N>, TypeList<vec<T, N>, Size<N>>, T>::SI;
	};

	template<size_t N>
	using matf = mat<float, N>;

	using matf3 = matf<3>;
	using matf4 = matf<4>;

	// maybe error in editor, but no compile error
	static_assert(sizeof(matf4) == 16 * sizeof(float));
}
