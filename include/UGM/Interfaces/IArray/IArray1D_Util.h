#pragma once

#include "../../basic.h"
#include "IArrayUtil.h"
#include "IArray1D.h"

namespace Ubpa {
	template<typename Base, typename Impl>
	struct IArray1D_Util : Base {
		using Base::Base;

		using T = ImplTraits_T<Impl>;
		using F = ImplTraits_F<Impl>;
		static constexpr size_t N = ImplTraits_N<Impl>;

		inline const Impl abs() const noexcept {
			const auto& x = static_cast<const Impl&>(*this);
#ifdef UBPA_USE_SIMD
			if constexpr (ImplTraits_SupportSIMD<Impl>)
				return _mm_abs_ps(x);
			else
#endif // UBPA_USE_SIMD
			{
				Impl rst;
				for (size_t i = 0; i < N; i++)
					rst[i] = std::abs(x[i]);
				return rst;
			}
		}

		inline T& min_component() noexcept {
			return (*this)[min_dim()];
		}

		inline T min_component() const noexcept {
#ifdef UBPA_USE_SIMD
			if constexpr (ImplTraits_SupportSIMD<Impl>) {
				// 5 instructions
				const auto& s0 = *this;
				auto s1 = VecSwizzle(s0, 1, 0, 3, 2);
				auto s2 = _mm_min_ps(s0, s1);
				auto s3 = VecSwizzle(s2, 2, 3, 0, 1);
				auto s4 = _mm_min_ps(s2, s3);
				return _mm_cvtss_f32(s4);
				// slow
				//return std::min(std::min((*this)[0], (*this)[1]), std::min((*this)[2], (*this)[3]));
			}
			else
#endif // UBPA_USE_SIMD
			return (*this)[min_dim()];
		}

		inline T& max_component() noexcept {
			return (*this)[max_dim()];
		}

		inline T max_component() const noexcept {
#ifdef UBPA_USE_SIMD
			if constexpr (ImplTraits_SupportSIMD<Impl>) {
				// 5 instructions
				const auto& s0 = *this;
				auto s1 = VecSwizzle(s0, 1, 0, 3, 2);
				auto s2 = _mm_max_ps(s0, s1);
				auto s3 = VecSwizzle(s2, 2, 3, 0, 1);
				auto s4 = _mm_max_ps(s2, s3);
				return _mm_cvtss_f32(s4);
				// slow
				//return std::max(std::max((*this)[0], (*this)[1]), std::max((*this)[2], (*this)[3]));
			}
			else
#endif // UBPA_USE_SIMD
			return (*this)[max_dim()];
		}

		inline constexpr size_t min_dim() const noexcept {
			T minVal = (*this)[0];
			size_t minDim = 0;
			for (size_t i = 1; i < N; i++) {
				if ((*this)[i] < minVal) {
					minVal = (*this)[i];
					minDim = i;
				}
			}
			return minDim;
		}

		inline constexpr size_t max_dim() const noexcept {
			T maxVal = (*this)[0];
			size_t maxDim = 0;
			for (size_t i = 1; i < N; i++) {
				if ((*this)[i] > maxVal) {
					maxVal = (*this)[i];
					maxDim = i;
				}
			}
			return maxDim;
		}

		static const Impl min(const Impl& x, const Impl& y) noexcept {
#ifdef UBPA_USE_SIMD
			if constexpr (ImplTraits_SupportSIMD<Impl>)
				return _mm_min_ps(x, y);
			else
#endif // UBPA_USE_SIMD
			{
				Impl rst;
				for (size_t i = 0; i < N; i++)
					rst[i] = std::min(x[i], y[i]);
				return rst;

			}
		}

		static const Impl max(const Impl& x, const Impl& y) noexcept {
#ifdef UBPA_USE_SIMD
			if constexpr (ImplTraits_SupportSIMD<Impl>)
				return _mm_max_ps(x, y);
			else
#endif // UBPA_USE_SIMD
			{
				Impl rst;
				for (size_t i = 0; i < N; i++)
					rst[i] = std::max(x[i], y[i]);
				return rst;
			}
		}
	};

	InterfaceTraits_Register(IArray1D_Util,
		IArray1D,
		IArrayUtil
	);
}
