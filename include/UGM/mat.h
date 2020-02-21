#pragma once

#include "vec.h"
#include "Interfaces/IMatrix/IMatrix.h"
#include "Interfaces/IMatrix/IMatrixMul.h"
#include "Interfaces/IMatrix/IMatrixInOut.h"
#include "Interfaces/IRing.h"

namespace Ubpa {
	template<typename T, size_t N>
	struct mat : SIIT_CRTP<TemplateList<IMatrixInOut, IRing, IEuclideanV, IMatrixMul>, mat<T, N>, TypeList<TypeList<vec<T, N>, Size<N>>, T>> {
		using SIIT_CRTP<TemplateList<IMatrixInOut, IRing, IEuclideanV, IMatrixMul>, mat<T, N>, TypeList<TypeList<vec<T, N>, Size<N>>, T>>::SIIT_CRTP;
	};

	template<size_t N>
	using matf = mat<float, N>;

	using matf3 = matf<3>;
	using matf4 = matf<4>;
}
