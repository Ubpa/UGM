#pragma once

#include "ILinear.h"

#include <tuple>

namespace Ubpa {
	// plane in linear subspace
	// 'O' : original point
	template<typename Base, typename Impl, typename ArgList>
	struct IOPlane : Base {
		using Base::Base;
		using Vector = Arg_Vector<ArgList>;
		using F = Arg_F<ArgList>;

		static_assert(Vector::template IsContain<ILinear>());

		Arg_Vector<ArgList> normal;

		void init_IOPlane(const Vector& n) noexcept { normal = n; }
	};
}
