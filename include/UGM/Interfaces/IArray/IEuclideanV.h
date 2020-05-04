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
	struct IEuclideanV : Base {
		using IList = TemplateList<IInnerProduct, IArrayLinear>;
		using Base::Base;

		static constexpr size_t N = Arg_N<ArgList>;
		using T = Arg_T<ArgList>;
		using F = Arg_F<ArgList>;


#ifdef UBPA_USE_XSIMD
		inline static F dot3(const Impl& x, const Impl& y) noexcept {
			static_assert(N == 4);
			auto srst = _mm_dp_ps(x, y, 0x71); // 0x71 : 01110001
			return srst.m128_f32[0];
		}

		inline F dot3(const Impl& y) const noexcept {
			const auto& x = static_cast<const Impl&>(*this);
			return dot3(x, y);
		}

		inline static const Impl cross3(const Impl& a, const Impl& b) noexcept {
			static_assert(N == 4);
			/*
			|a.x|   |b.x|   | a.y * b.z - a.z * b.y |
			|a.y| X |b.y| = | a.z * b.x - a.x * b.z |
			|a.z|   |b.z|   | a.x * b.y - a.y * b.x |
			*/
			return VecSwizzle(_mm_sub_ps(_mm_mul_ps(VecSwizzle(a, 2, 0, 1, 3), b),
				_mm_mul_ps(VecSwizzle(b, 2, 0, 1, 3), a)),
				2, 0, 1, 3);
		}

		inline const Impl cross3(const Impl& b) const noexcept {
			const auto& a = static_cast<const Impl&>(*this);
			return cross3(a, b);
		}
#endif

	private:
		template<typename Base, typename Impl, typename ArgList>
		friend struct IInnerProduct;

		inline static F impl_dot(const Impl& x, const Impl& y) noexcept {
#ifdef UBPA_USE_XSIMD
			if constexpr (std::is_same_v<T, float> && N == 4) {
				// ref
				// https://stackoverflow.com/questions/4120681/how-to-calculate-single-vector-dot-product-using-sse-intrinsic-functions-in-c
#ifdef USE_SSE_4_1
				auto srst = _mm_dp_ps(x, y, 0xf1); // 0xf1 : 11110001
				return _mm_cvtss_f32(srst);
#else
				auto srst = x * y;
				__m128 shuf = _mm_shuffle_ps(srst, srst, _MM_SHUFFLE(2, 3, 0, 1));
				__m128 sums = _mm_add_ps(srst, shuf);
				shuf = _mm_movehl_ps(shuf, sums);
				sums = _mm_add_ss(sums, shuf);
				return _mm_cvtss_f32(sums);
#endif // USE_SSE_4_1
			}
			else
#endif // UBPA_USE_XSIMD
			{
				F rst;
				rst = x[0] * y[0];
				for (size_t i = 1; i < N; i++)
					rst += x[i] * y[i];
				return rst;
			}
		}
	};
}
