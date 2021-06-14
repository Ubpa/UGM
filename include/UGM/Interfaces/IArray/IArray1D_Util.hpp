#pragma once

#include "../../basic.hpp"
#include "IArrayUtil.hpp"
#include "IArray1D.hpp"

namespace Ubpa {
	template<typename Base, typename Impl>
	struct IArray1D_Util : Base {
		using Base::Base;

		using T = SI_ImplTraits_T<Impl>;
		using F = SI_ImplTraits_F<Impl>;
		static constexpr size_t N = SI_ImplTraits_N<Impl>;

		T& min_component() noexcept {
			return (*this)[min_dim()];
		}

		T min_component() const noexcept {
#ifdef UBPA_UGM_USE_SIMD
			if constexpr (SI_ImplTraits_SupportSIMD<Impl>) {
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
#endif // UBPA_UGM_USE_SIMD
			return (*this)[min_dim()];
		}

		T& max_component() noexcept {
			return (*this)[max_dim()];
		}

		T max_component() const noexcept {
#ifdef UBPA_UGM_USE_SIMD
			if constexpr (SI_ImplTraits_SupportSIMD<Impl>) {
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
#endif // UBPA_UGM_USE_SIMD
			return (*this)[max_dim()];
		}

		constexpr size_t min_dim() const noexcept {
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

		constexpr size_t max_dim() const noexcept {
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

		static Impl min(const Impl& x, const Impl& y) noexcept {
#ifdef UBPA_UGM_USE_SIMD
			if constexpr (SI_ImplTraits_SupportSIMD<Impl>)
				return _mm_min_ps(x, y);
			else
#endif // UBPA_UGM_USE_SIMD
			{
				Impl rst;
				for (size_t i = 0; i < N; i++)
					rst[i] = std::min(x[i], y[i]);
				return rst;

			}
		}

		static Impl max(const Impl& x, const Impl& y) noexcept {
#ifdef UBPA_UGM_USE_SIMD
			if constexpr (SI_ImplTraits_SupportSIMD<Impl>)
				return _mm_max_ps(x, y);
			else
#endif // UBPA_UGM_USE_SIMD
			{
				Impl rst;
				for (size_t i = 0; i < N; i++)
					rst[i] = std::max(x[i], y[i]);
				return rst;
			}
		}
	};
}

SI_InterfaceTraits_Register(Ubpa::IArray1D_Util,
	Ubpa::IArray1D,
	Ubpa::IArrayUtil
);
