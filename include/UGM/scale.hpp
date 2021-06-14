#pragma once

#include "details/traits.hpp"

#include "Interfaces/IArray/IArrayScalarMul.hpp"
#include "Interfaces/IArray/IArray1D_Util.hpp"
#include "Interfaces/IArray/IArrayHadamardProduct.hpp"

namespace Ubpa {
	template<typename T, size_t N>
	struct SI_ImplTraits<scale<T, N>>
		: Array1DTraits<T, N>,
		IListTraits<IArray1D_Util, IArrayHadamardProduct, IArrayScalarMul>,
		ArrayImplNTraits<scale, T> {};

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

	template<size_t N>
	using scaled = scale<double, N>;

	using scaled1 = scaled<1>;
	using scaled2 = scaled<2>;
	using scaled3 = scaled<3>;
	using scaled4 = scaled<4>;

	// maybe error in editor, but no compile error
	static_assert(sizeof(scalef4) == 4 * sizeof(float));
}
