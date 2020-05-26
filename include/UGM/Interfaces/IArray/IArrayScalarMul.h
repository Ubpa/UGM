#pragma once

#include "IArray.h"
#include "../IScalarMul.h"

namespace Ubpa {
	template<typename Base, typename Impl>
	struct IArrayScalarMul : Base {
		using Base::Base;

		using T = ImplTraits_T<Impl>;
		static constexpr size_t N = ImplTraits_N<Impl>;
		using F = ImplTraits_F<Impl>;

		using Base::operator*;

	private:
		template<typename Base, typename Impl>
		friend struct IScalarMul;

		inline const Impl impl_scalar_mul(F k) const noexcept {
			auto& x = static_cast<const Impl&>(*this);
#ifdef UBPA_USE_SIMD
			if constexpr (SupportSIMD_v<Impl>)
				return _mm_mul_ps(x, Impl{ k });
			else
#endif // UBPA_USE_SIMD
			{
				Impl rst;
				for (size_t i = 0; i < N; i++)
					rst[i] = x[i] * k;
				return rst;
			}
		}

		inline Impl& impl_scalar_mul_to_self(F k) noexcept {
			auto& x = static_cast<Impl&>(*this);
#ifdef UBPA_USE_SIMD
			if constexpr (SupportSIMD_v<Impl>)
				return x = x * k;
			else
#endif // UBPA_USE_SIMD
			{
				for (size_t i = 0; i < N; i++)
					x[i] *= k;
				return x;
			}
		}
	};

	InterfaceTraits_Regist(IArrayScalarMul,
		IScalarMul, IArray);
}
