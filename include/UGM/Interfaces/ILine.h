#pragma once

#include "IOLine.h"
#include "IAffineRealSubspace.h"

namespace Ubpa {
	// line in real affine subspace
	template<typename Base, typename Impl>
	struct ILine : Base {
		using Base::Base;

		using Point = ImplTraits_P<Impl>;
		using Vector = ImplTraits_V<Impl>;
		using F = ImplTraits_F<Impl>;

		const Point at(F t) const noexcept { return this->point + t * this->dir; }
	};

	InterfaceTraits_Register(ILine,
		IAffineRealSubspace,
		IOLine
	);
}
