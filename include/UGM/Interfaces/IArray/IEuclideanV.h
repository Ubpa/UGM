#pragma once

#include "IArrayLinear.h"
#include "../IInnerProduct.h"
#include "../INorm.h"

namespace Ubpa {
	// euclidean vector space
	template<typename Base, typename Impl, typename ArgList>
	struct IEuclideanV : SIVT_CRTP<TemplateList<IInnerProduct, IArrayLinear>, Base, Impl, ArgList> {
		static constexpr size_t N = Arg_N<ArgList>;
		using T = Arg_T<ArgList>;
		using F = Arg_F<ArgList>;

		using SIVT_CRTP<TemplateList<IInnerProduct, IArrayLinear>, Base, Impl, ArgList>::SIVT_CRTP;

	private:
		template<typename Base, typename Impl, typename ArgList>
		friend struct IInnerProduct;

		inline static F impl_dot(const Impl& x, const Impl& y) noexcept {
			F rst;
#ifdef USE_XSIMD
			if constexpr (std::is_same_v<T, float> && N == 4) {
				// ref
				// https://stackoverflow.com/questions/4120681/how-to-calculate-single-vector-dot-product-using-sse-intrinsic-functions-in-c
				auto sx = xsimd::load_aligned(x.data());
				auto sy = xsimd::load_aligned(y.data());
				auto srst = sx * sy;
				__m128 shuf = _mm_shuffle_ps(srst, srst, _MM_SHUFFLE(2, 3, 0, 1));
				__m128 sums = _mm_add_ps(srst, shuf);
				shuf = _mm_movehl_ps(shuf, sums);
				sums = _mm_add_ss(sums, shuf);
				rst = _mm_cvtss_f32(sums);
			}
			else
#endif // USE_XSIMD
			{
				rst = x[0] * y[0];
				for (size_t i = 1; i < N; i++)
					rst += x[i] * y[i];
			}
			return rst;
		}
	};
}
