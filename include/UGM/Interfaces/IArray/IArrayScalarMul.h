#pragma once

#include "IArray.h"
#include "../IScalarMul.h"

namespace Ubpa {
	template<typename Base, typename Impl, typename ArgList>
	struct IArrayScalarMul : SIVT_CRTP<TemplateList<IScalarMul, IArray>, Base, Impl, ArgList>  {
		static constexpr size_t N = Arg_N<ArgList>;
		using F = Arg_F<ArgList>;
		using T = Arg_T<ArgList>;

		using SIVT_CRTP<TemplateList<IScalarMul, IArray>, Base, Impl, ArgList>::SIVT_CRTP;
		using SIVT_CRTP<TemplateList<IScalarMul, IArray>, Base, Impl, ArgList>::operator*;

	private:
		template<typename Base, typename Impl, typename ArgList>
		friend struct IScalarMul;

		template<typename U, typename = std::enable_if_t<std::is_arithmetic_v<U>>>
		inline const Impl impl_scalar_mul(U k) const noexcept {
			auto& x = static_cast<const Impl&>(*this);
			auto kF = static_cast<F>(k);
#ifdef USE_XSIMD
			if constexpr (std::is_same_v<T, float> && N == 4)
				return x.get() * kF;
			else
#endif // USE_XSIMD
			{
				Impl rst{};
				for (size_t i = 0; i < N; i++)
					rst[i] = x[i] * kF;
				return rst;
			}
		}

		template<typename U, typename = std::enable_if_t<std::is_arithmetic_v<U>>>
		inline Impl& impl_scalar_mul_to_self(U k) noexcept {
			auto& x = static_cast<Impl&>(*this);
			auto kF = static_cast<F>(k);
#ifdef USE_XSIMD
			if constexpr (std::is_same_v<T, float> && N == 4)
				return x.get() *= kF;
			else
#endif // USE_XSIMD
			{
				for (size_t i = 0; i < N; i++)
					x[i] *= kF;
				return x;
			}
		}
	};
}
