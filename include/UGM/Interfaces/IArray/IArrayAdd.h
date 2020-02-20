#pragma once

#include <UTemplate/SI.h>
#include "IArray.h"
#include "../IAdd.h"

namespace Ubpa {
	template<typename Base, typename Impl, typename T, typename N>
	struct IArrayAdd : SIVT_CRTP<TemplateList<IAdd, IArray>, Base, Impl, T, N>  {
		using SIVT_CRTP<TemplateList<IAdd, IArray>, Base, Impl, T, N>::SIVT_CRTP;

	private:
		template<typename Base, typename Impl, typename T, typename N>
		friend struct IAdd;

		const Impl ImplAdd(const Impl& y) const noexcept {
			auto& x = static_cast<const Impl&>(*this);
			Impl rst{};
			for (size_t i = 0; i < N::value; i++)
				rst[i] = x[i] + y[i];
			return rst;
		}

		Impl& ImplAddToSelf(const Impl& y) noexcept {
			auto& x = static_cast<Impl&>(*this);
			for (size_t i = 0; i < N::value; i++)
				x[i] += y[i];
			return x;
		}

		const Impl ImplAddInverse() const noexcept {
			auto& x = static_cast<const Impl&>(*this);
			Impl rst{};
			for (size_t i = 0; i < N::value; i++)
				rst[i] = -x[i];
			return rst;
		}

		//const Impl Minus(const Impl& y) const noexcept {
		//	auto& x = static_cast<const Impl&>(*this);
		//	Impl rst{};
		//	for (size_t i = 0; i < N::value; i++)
		//		rst[i] = x[i] - y[i];
		//	return rst;
		//}

		//Impl& MinusToSelf(const Impl& y) noexcept {
		//	auto& x = static_cast<Impl&>(*this);
		//	for (size_t i = 0; i < N::value; i++)
		//		x[i] -= y[i];
		//	return x;
		//}
	};
}
