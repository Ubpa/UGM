#pragma once

#include "IArrayLinear.h"
#include "../IInnerProduct.h"
#include "../INorm.h"

namespace Ubpa {
	// euclidean vector space
	template<typename Base, typename Impl>
	struct IEuclideanV : Base {
		using Base::Base;

		using T = ImplTraits_T<Impl>;
		static constexpr size_t N = ImplTraits_N<Impl>;
		using F = ImplTraits_F<Impl>;

#ifdef UBPA_USE_SIMD
		// w == 0
		inline static const Impl v3_cross(const Impl& x, const Impl& y) noexcept {
			static_assert(SupportSIMD_v<Impl>);
			/*
			|a.x|   |b.x|   | a.y * b.z - a.z * b.y |
			|a.y| X |b.y| = | a.z * b.x - a.x * b.z |
			|a.z|   |b.z|   | a.x * b.y - a.y * b.x |
			*/
			return VecSwizzle(_mm_sub_ps(_mm_mul_ps(VecSwizzle(x, 2, 0, 1, 3), y),
				_mm_mul_ps(VecSwizzle(y, 2, 0, 1, 3), x)),
				2, 0, 1, 3);
		}

		// w == 0
		inline const Impl v3_cross(const Impl& y) const noexcept {
			const auto& x = static_cast<const Impl&>(*this);
			return v3_cross(x, y);
		}

		// x = y = z = w
		inline static Impl v3_dot(const Impl& x, const Impl& y) noexcept {
			static_assert(SupportSIMD_v<Impl>);
			// 0x7f : 011111111
			return _mm_dp_ps(x, y, 0x7f);
		}

		// x = y = z = w
		inline Impl v3_dot(const Impl& y) const noexcept {
			const auto& x = static_cast<const Impl&>(*this);
			return v3_dot(x, y);
		}

		// x = y = z = w
		inline Impl v3_norm2() const noexcept {
			const auto& x = static_cast<const Impl&>(*this);
			return v3_dot(x, x);
		}

		// x = y = z = w
		inline Impl v3_norm() const noexcept {
			return _mm_sqrt_ps(v3_norm2());
		}

		inline const Impl v3_normalize() const noexcept {
			const auto& x = static_cast<const Impl&>(*this);
			auto n = x.v3_norm();
			assert(n.get<0>() > static_cast<F>(0));
			return x / n; // ILinear
		}

		inline bool v3_is_normalized() const noexcept {
			const auto& x = static_cast<const Impl&>(*this);
			return std::abs(x.v3_norm().get<0>() - 1) < EPSILON<F>;
		}

		inline Impl& v3_normalize_self() noexcept {
			auto& x = static_cast<Impl&>(*this);
			return x = v3_normalize();
		}

		// x = y = z = w
		inline static Impl v3_distance2(const Impl& x, const Impl& y) noexcept {
			return (x - y).v3_norm2();
		}

		// x = y = z = w
		inline Impl v3_distance2(const Impl& y) const noexcept {
			const auto& x = static_cast<const Impl&>(*this);
			return v3_distance2(x, y);
		}

		// x = y = z = w
		inline static Impl v3_distance(const Impl& x, const Impl& y) noexcept {
			return _mm_sqrt_ps(v3_distance2(x, y));
		}

		// x = y = z = w
		inline Impl v3_distance(const Impl& y) const noexcept {
			const auto& x = static_cast<const Impl&>(*this);
			return v3_distance(x, y);
		}

		// x = y = z = w
		inline static Impl v3_cos_theta(const Impl& x, const Impl& y) noexcept {
			auto xN = x.v3_norm();
			auto yN = y.v3_norm();
			auto xyN = _mm_mul_ps(xN, yN);
			assert(_mm_cvtss_f32(xyN) != static_cast<F>(0));
			return v3_dot(x, y) / xyN;
		}

		// x = y = z = w
		inline Impl v3_cos_theta(const Impl& y) const noexcept {
			const auto& x = static_cast<const Impl&>(*this);
			return v3_cos_theta(x, y);
		}

		const Impl v3_project(const Impl& n) const noexcept {
			assert(n.v3_is_normalized());
			return v3_dot(n) * n;
		}

		const Impl v3_perpendicular(const Impl& n) const noexcept {
			const auto& x = static_cast<const Impl&>(*this);
			return x - x.v3_project(n);
		}
#endif

	private:
		template<typename Base, typename Impl>
		friend struct IInnerProduct;

		inline static F impl_dot(const Impl& x, const Impl& y) noexcept {
#ifdef UBPA_USE_SIMD
			if constexpr (SupportSIMD_v<Impl>) {
				// ref
				// https://stackoverflow.com/questions/4120681/how-to-calculate-single-vector-dot-product-using-sse-intrinsic-functions-in-c
#ifdef UBPA_USE_SSE_4_1
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
#endif // UBPA_USE_SIMD
			{
				F rst;
				rst = x[0] * y[0];
				for (size_t i = 1; i < N; i++)
					rst += x[i] * y[i];
				return rst;
			}
		}
	};

	InterfaceTraits_Regist(IEuclideanV,
		IInnerProduct, IArrayLinear);
}
