#pragma once

#include "IArray.h"
#include "../IScalarMul.h"

namespace Ubpa {
	template<typename Base, typename Impl, typename ArgList>
	struct IArrayScalarMul : SIVT_CRTP<TemplateList<IScalarMul, IArray>, Base, Impl, ArgList>  {
		using T = At_t<ArgList, 0>;
		using N = At_t<ArgList, 1>;

		using SIVT_CRTP<TemplateList<IScalarMul, IArray>, Base, Impl, ArgList>::SIVT_CRTP;

	private:
		template<typename Base, typename Impl, typename ArgList>
		friend struct IScalarMul;

		template<typename U, typename = std::enable_if_t<std::is_arithmetic_v<U>>>
		const Impl ImplScalarMul(U k) const noexcept {
			auto& x = static_cast<const Impl&>(*this);
			auto kT = static_cast<T>(k);
			Impl rst{};
			for (size_t i = 0; i < N::value; i++)
				rst[i] = x[i] * kT;
			return rst;
		}

		template<typename U, typename = std::enable_if_t<std::is_arithmetic_v<U>>>
		Impl& ImplScalarMulToSelf(U k) noexcept {
			auto& x = static_cast<Impl&>(*this);
			auto kT = static_cast<T>(k);
			for (size_t i = 0; i < N::value; i++)
				x[i] *= kT;
			return x;
		}
	};
}
