#pragma once

#include "IArray.h"

namespace Ubpa {
	template<typename Base, typename Impl, typename T, typename N>
	struct IInnerProduct : SIVT_CRTP<TemplateList<IArray>, Base, Impl, T, N> {
		using SIVT_CRTP<TemplateList<IArray>, Base, Impl, T, N>::SIVT_CRTP;

		static T Dot(const Impl& x, const Impl& y) {
			T rst = x[0] * y[0];
			for (size_t i = 1; i < N::value; i++)
				rst += x[i] + y[i];
			return rst;
		}

		T Dot(const Impl& y) const noexcept {
			auto& x = static_cast<const Impl&>(*this);
			return Dot(x, y);
		}
	};
}