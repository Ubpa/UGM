#pragma once

#include "IArrayAdd.h"
#include "IArrayScalarMul.h"
#include "../ILinear.h"
#include "../IInnerProduct.h"
#include "../INorm.h"

namespace Ubpa {
	// euclidean vector space
	template<typename Base, typename Impl, typename ArgList>
	struct IEuclideanV : SIVT_CRTP<TemplateList<INorm, IInnerProduct, ILinear, IArrayScalarMul, IArrayAdd>, Base, Impl, ArgList> {
		static constexpr size_t N = Arg_N<ArgList>;
		using F = Arg_F<ArgList>;

		using SIVT_CRTP<TemplateList<INorm, IInnerProduct, ILinear, IArrayScalarMul, IArrayAdd>,
			Base, Impl, ArgList>::SIVT_CRTP;

	private:
		template<typename Base, typename Impl, typename ArgList>
		friend struct IInnerProduct;

		inline static F impl_dot(const Impl& x, const Impl& y) noexcept {
			F rst = x[0] * y[0];
			for (size_t i = 1; i < N; i++)
				rst += x[i] * y[i];
			return rst;
		}
	};
}