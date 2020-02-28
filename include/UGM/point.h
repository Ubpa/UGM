#pragma once

#include "vec.h"

#include "Interfaces/IArray/IEuclideanA.h"
#include "Interfaces/IArray/IArrayInOut.h"
#include "Interfaces/IArray/IArray1D_Util.h"

namespace Ubpa {
	template<typename T, size_t N>
	struct vec;

	template<typename T, size_t N>
	struct point : SIIT_CRTP<TemplateList<IArrayInOut, IEuclideanA>, point<T, N>, TypeList<TypeList<T, Size<N>>, T, vec<T, N>>> {
		using SIIT_CRTP<TemplateList<IArrayInOut, IEuclideanA>, point<T, N>, TypeList<TypeList<T, Size<N>>, T, vec<T, N>>>::SIIT_CRTP;

		inline explicit point(const vec<T, N>& vec) {
			for (size_t i = 0; i < N; i++)
				(*this)[i] = vec[i];
		}
	};

	template<size_t N>
	using pointf = point<float, N>;

	using pointf1 = pointf<1>;
	using pointf2 = pointf<2>;
	using pointf3 = pointf<3>;
	using pointf4 = pointf<4>;
}
