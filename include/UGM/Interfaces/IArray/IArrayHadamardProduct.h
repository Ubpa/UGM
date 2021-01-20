#pragma once

#include "../IMul.h"
#include <UTemplate/SI.h>

namespace Ubpa {
	template<typename Base, typename Impl>
	struct IArrayHadamardProduct : Base {
		using Base::Base;

		using F = SI_ImplTraits_F<Impl>;
		using T = SI_ImplTraits_T<Impl>;
		static constexpr size_t N = SI_ImplTraits_N<Impl>;

		using Base::operator*=;
		using Base::operator/;
		using Base::operator/=;

		Impl& operator*=(const Impl& y) noexcept {
			auto& x = static_cast<Impl&>(*this);
#ifdef UBPA_UGM_USE_SIMD
			if constexpr (SI_ImplTraits_SupportSIMD<Impl>)
				x = x * y;
			else
#endif // UBPA_UGM_USE_SIMD
			for (size_t i = 0; i < N; i++)
				x[i] *= y[i];
			return x;
		}

		Impl operator/(const Impl& y) const noexcept {
			const auto& x = static_cast<const Impl&>(*this);
#ifdef UBPA_UGM_USE_SIMD
			if constexpr (SI_ImplTraits_SupportSIMD<Impl>)
				return _mm_div_ps(x, y);
			else
#endif // UBPA_UGM_USE_SIMD
			{
				Impl rst;
				for (size_t i = 0; i < N; i++)
					rst[i] = x[i] / y[i];
				return rst;
			}
		}

		Impl& operator/=(const Impl& y) noexcept {
			auto& x = static_cast<Impl&>(*this);
#ifdef UBPA_UGM_USE_SIMD
			if constexpr (SI_ImplTraits_SupportSIMD<Impl>)
				return x = x / y;
			else
#endif // UBPA_UGM_USE_SIMD
			for (size_t i = 0; i < N; i++)
				x[i] /= y[i];
			return x;
		}

		Impl inverse() const noexcept {
			const auto& x = static_cast<const Impl&>(*this);

#ifdef UBPA_UGM_USE_SIMD
			if constexpr (SI_ImplTraits_SupportSIMD<Impl>)
				return _mm_div_ps(Impl{ 1.f }, x);
			else
#endif // UBPA_UGM_USE_SIMD
			{
				Impl rst;
				for (size_t i = 0; i < N; i++)
					rst[i] = static_cast<F>(1) / x[i];
				return rst;
			}
		}

	private:
		template<typename, typename>
		friend struct IMul;

		Impl impl_mul(const Impl& y) const noexcept {
			const auto& x = static_cast<const Impl&>(*this);

#ifdef UBPA_UGM_USE_SIMD
			if constexpr (SI_ImplTraits_SupportSIMD<Impl>)
				return _mm_mul_ps(x, y);
			else
#endif // UBPA_UGM_USE_SIMD
			{
				Impl rst;
				for (size_t i = 0; i < N; i++)
					rst[i] = x[i] * y[i];
				return rst;
			}
		}
	};
}

SI_InterfaceTraits_Register(Ubpa::IArrayHadamardProduct,
	Ubpa::IMul,
	Ubpa::IArray
);
