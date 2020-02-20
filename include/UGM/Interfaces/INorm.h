#pragma once

#include "ILinear.h"

namespace Ubpa {
	template<typename Base, typename Impl, typename T, typename N>
	struct INorm : SIVT_CRTP<TemplateList<ILinear>, Base, Impl, T, N> {
		using SIVT_CRTP<TemplateList<ILinear>, Base, Impl, T, N>::SIVT_CRTP;

		T norm() const noexcept {
			return static_cast<const Impl*>(this)->ImplNorm();
		}

		const Impl normalize() const noexcept {
			auto& x = static_cast<const Impl&>(*this);
			T n = norm();
			assert(n > static_cast<T>(0));
			return x / n; // ILinear
		}

		Impl& normalize_self() noexcept {
			auto& x = static_cast<Impl&>(*this);
			T n = norm();
			assert(n > static_cast<T>(0));
			return x /= n; // ILinear
		}
	};
}