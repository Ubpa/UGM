#pragma once

#include "IArrayAdd.h"
#include "IArrayScalarMul.h"
#include "../ILinear.h"
#include "../IInnerProduct.h"
#include "../INorm.h"

namespace Ubpa {
	template<typename Base, typename Impl, typename T, typename N>
	struct IEuclidean : SIVT_CRTP<TemplateList<INorm, IInnerProduct, ILinear, IArrayScalarMul, IArrayAdd>, Base, Impl, T, N> {
		using SIVT_CRTP<TemplateList<INorm, IInnerProduct, ILinear, IArrayScalarMul, IArrayAdd>, Base, Impl, T, N>::SIVT_CRTP;

	private:
		template<typename Base, typename Impl, typename T, typename N>
		friend struct IInnerProduct;

		static T impl_dot(const Impl& x, const Impl& y) noexcept {
			T rst = x[0] * y[0];
			for (size_t i = 1; i < N::value; i++)
				rst += x[i] * y[i];
			return rst;
		}
	};
}