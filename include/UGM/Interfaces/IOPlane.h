#pragma once

#include "ILinear.h"

#include <tuple>

namespace Ubpa {
	// plane in linear subspace
	// 'O' : original point
	template<typename Base, typename Impl>
	struct IOPlane : Base {
		using Base::Base;

		using Normal = ImplTraits_V<Impl>;
		using F = ImplTraits_F<Impl>;

		static_assert(Normal::template Contains<ILinear>());

		ImplTraits_V<Impl> normal;
	};
}
