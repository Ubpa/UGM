#pragma once

#include "Interfaces/IArray/IArrayScalarMul.h"
#include "Interfaces/IArray/IArray1D_Util.h"
#include "Interfaces/IArray/IArrayHadamardProduct.h"

namespace Ubpa {
	template<typename T, size_t N>
	struct scale;

	template<typename T_, size_t N_>
	struct ImplTraits<scale<T_, N_>> {
		using IList = TemplateList<IArray1D_Util, IArrayHadamardProduct, IArrayScalarMul>;
		using T = T_;
		static constexpr size_t N = N_;
		using F = T;
	};

	template<typename T, size_t N>
	struct scale : SI<scale<T, N>> {
		using SI<scale<T, N>>::SI;
	};

	template<size_t N>
	using scalef = scale<float, N>;

	using scalef1 = scalef<1>;
	using scalef2 = scalef<2>;
	using scalef3 = scalef<3>;
	using scalef4 = scalef<4>;

	// maybe error in editor, but no compile error
	static_assert(sizeof(scalef4) == 4 * sizeof(float));
}
