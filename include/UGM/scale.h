#pragma once

#include "Interfaces/ILinear.h"
#include "Interfaces/IArray/IArrayAdd.h"
#include "Interfaces/IArray/IArrayScalarMul.h"
#include "Interfaces/IArray/IArrayInOut.h"
#include "Interfaces/IArray/IArrayHadamardProduct.h"

namespace Ubpa {
	template<typename T>
	struct scale : SIIT_CRTP<TemplateList<IArrayInOut, IArrayHadamardProduct, ILinear, IArrayScalarMul, IArrayAdd>, scale<T>,
		TypeList<TypeList<T, Size<3>>, T>>
	{
		using SIIT_CRTP<TemplateList<IArrayInOut, IArrayHadamardProduct, ILinear, IArrayScalarMul, IArrayAdd>, scale<T>,
			TypeList<TypeList<T, Size<3>>, T>>::SIIT_CRTP;
	};

	using scalef = scale<float>;
}
