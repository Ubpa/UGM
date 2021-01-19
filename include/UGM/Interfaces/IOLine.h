#pragma once

#include "ILinear.h"

#include <tuple>

namespace Ubpa {
	// line in linear subspace
	// 'O' : original point
	template<typename Base, typename Impl>
	struct IOLine : Base {
		using Base::Base;

		using Vector = SI_ImplTraits_V<Impl>;
		using F = SI_ImplTraits_F<Impl>;

		static_assert(SI_Contains_v<Vector, ILinear>);

		SI_ImplTraits_V<Impl> dir;

		SI_ImplTraits_V<Impl> inv_dir() const noexcept {
			// TODO: SIMD speedup
			SI_ImplTraits_V<Impl> rst;
			for (size_t i = 0; i < SI_ImplTraits_V<Impl>::N; i++)
				rst[i] = ONE<F> / dir[i];
			return rst;
		}
	};
}
