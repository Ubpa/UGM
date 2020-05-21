#pragma once

#include "ILinear.h"

#include <tuple>

namespace Ubpa {
	// line in linear subspace
	// 'O' : original point
	template<typename Base, typename Impl>
	struct IOLine : Base {
		using Base::Base;

		using Vector = ImplTraits_V<Impl>;
		using F = ImplTraits_F<Impl>;

		static_assert(Vector::template IsContain<ILinear>());

		ImplTraits_V<Impl> dir;

		ImplTraits_V<Impl> inv_dir() const noexcept {
#ifdef UBPA_USE_XSIMD
			if constexpr (std::is_same_v<F, float> && ImplTraits_V<Impl>::N == 4)
				return 1.f / (*this).get_batch();
			else
#endif
			{
				ImplTraits_V<Impl> rst;
				for (size_t i = 0; i < ImplTraits_V<Impl>::N; i++)
					rst[i] = ONE<F> / dir[i];
				return rst;
			}
		}

		void init_IOLine(const Vector& v) noexcept { dir = v; }
	};
}
