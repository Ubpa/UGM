#pragma once

#include "IArray.h"
#include "../IScalarMul.h"

namespace Ubpa {
	template<typename Base, typename Impl>
	struct IArrayScalarMul : Base {
		using Base::Base;

		using T = SI_ImplTraits_T<Impl>;
		static constexpr size_t N = SI_ImplTraits_N<Impl>;
		using F = SI_ImplTraits_F<Impl>;
		
#ifdef UBPA_UGM_USE_SIMD
		using Base::operator*;
		using Base::operator*=;
		using Base::operator/;
		using Base::operator/=;

		bool is_scalar() const noexcept {
			static_assert(SI_ImplTraits_SupportSIMD<Impl>);
			const auto& x = static_cast<const Impl&>(*this);
			return x == x.replicate<0>();
		}

		Impl operator*(const __m128& k) const noexcept {
			static_assert(SI_ImplTraits_SupportSIMD<Impl>);
			const auto& x = static_cast<const Impl&>(*this);
			assert(Impl{ k }.is_scalar() || x.is_scalar());
			return _mm_mul_ps(x, k);
		}

		Impl& operator*=(const __m128& k) noexcept {
			const auto& x = static_cast<const Impl&>(*this);
			return x = x * k;
		}

		Impl operator/(const __m128& k) const noexcept {
			static_assert(SI_ImplTraits_SupportSIMD<Impl>);
			const auto& x = static_cast<const Impl&>(*this);
			assert(Impl{ k }.is_scalar());
			return _mm_div_ps(x, k);
		}

		Impl& operator/=(const __m128& k) noexcept {
			const auto& x = static_cast<const Impl&>(*this);
			return x = x / k;
		}
#endif // UBPA_UGM_USE_SIMD

	private:
		template<typename Base, typename Impl>
		friend struct IScalarMul;

		Impl impl_scalar_mul(F k) const noexcept {
			const auto& x = static_cast<const Impl&>(*this);
#ifdef UBPA_UGM_USE_SIMD
			if constexpr (SI_ImplTraits_SupportSIMD<Impl>)
				return _mm_mul_ps(x, Impl{ k });
			else
#endif // UBPA_UGM_USE_SIMD
			{
				Impl rst;
				for (size_t i = 0; i < N; i++)
					rst[i] = x[i] * k;
				return rst;
			}
		}

		Impl& impl_scalar_mul_to_self(F k) noexcept {
			auto& x = static_cast<Impl&>(*this);
#ifdef UBPA_UGM_USE_SIMD
			if constexpr (SI_ImplTraits_SupportSIMD<Impl>)
				return x = x * k;
			else
#endif // UBPA_UGM_USE_SIMD
			{
				for (size_t i = 0; i < N; i++)
					x[i] *= k;
				return x;
			}
		}
	};

	SI_InterfaceTraits_Register(IArrayScalarMul,
		IScalarMul,
		IArray
	);
}
