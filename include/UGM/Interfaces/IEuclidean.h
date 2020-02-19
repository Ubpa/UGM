#pragma once

#include "ILinear.h"
#include "IInnerProduct.h"

namespace Ubpa {
	template<typename Base, typename Impl, typename T, typename N>
	struct IEuclidean : SIVT_CRTP<TemplateList<IInnerProduct, ILinear>, Base, Impl, T, N> {
		using SIVT_CRTP<TemplateList<IInnerProduct, ILinear>, Base, Impl, T, N>::SIVT_CRTP;
	};
}