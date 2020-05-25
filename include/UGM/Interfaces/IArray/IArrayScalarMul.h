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
			auto kF = static_cast<F>(k);
#ifdef UBPA_USE_XSIMD
			if constexpr (std::is_same_v<T, float> && N == 4)
				return x.get() * kF;
			else
#endif // UBPA_USE_XSIMD
			{
				Impl rst;
				for (size_t i = 0; i < N; i++)
					rst[i] = x[i] * kF;
				return rst;
			}
		}

		inline Impl& impl_scalar_mul_to_self(F k) noexcept {
			auto& x = static_cast<Impl&>(*this);
			auto kF = static_cast<F>(k);
#ifdef UBPA_USE_XSIMD
			if constexpr (std::is_same_v<T, float> && N == 4)
				return x.get() *= kF;
			else
#endif // UBPA_USE_XSIMD
			{
				for (size_t i = 0; i < N; i++)
					x[i] *= kF;
				return x;
			}
		}
	};

	InterfaceTraits_Regist(IArrayScalarMul,
		IScalarMul, IArray);
}
