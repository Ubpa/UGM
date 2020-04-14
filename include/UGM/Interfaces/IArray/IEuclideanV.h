#pragma once

#include "IArrayLinear.h"
#include "../IInnerProduct.h"
#include "../INorm.h"

/*
<mmintrin.h>  MMX
<xmmintrin.h> SSE
<emmintrin.h> SSE2
<pmmintrin.h> SSE3
<tmmintrin.h> SSSE3
<smmintrin.h> SSE4.1
<nmmintrin.h> SSE4.2
<ammintrin.h> SSE4A
<wmmintrin.h> AES
<immintrin.h> AVX, AVX2, FMA
*/
#include <smmintrin.h>
#define USE_SSE_4_1

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

#ifdef USE_SSE_4_1
				auto srst = _mm_dp_ps(x, y, 0xf1); // 0xf1 : 11110001
				rst = srst.m128_f32[0];
#else
				auto srst = x * y;
				__m128 shuf = _mm_shuffle_ps(srst, srst, _MM_SHUFFLE(2, 3, 0, 1));
				__m128 sums = _mm_add_ps(srst, shuf);
				shuf = _mm_movehl_ps(shuf, sums);
				sums = _mm_add_ss(sums, shuf);
				rst = _mm_cvtss_f32(sums);
#endif // USE_SSE_4_1
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
