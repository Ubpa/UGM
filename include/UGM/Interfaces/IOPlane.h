#pragma once

#include "ILinear.h"

#include <tuple>

namespace Ubpa {
	// plane in linear subspace
	// 'O' : original point
	template<typename Base, typename Impl, typename ArgList>
	struct IOPlane : Base {
		using Vector = Arg_Vector<ArgList>;
		using F = Arg_F<ArgList>;

		static_assert(ExistInstance_v<typename Vector::AllVBs, ILinear>);

		Arg_Vector<ArgList> normal;

		void init_IOPlane(const Vector& n) noexcept { normal = n; }
	};
}
