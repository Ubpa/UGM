#pragma once

#include "IAffineSubspace.h"

namespace Ubpa {
	template<typename Base, typename Point>
	struct IAffine : Base {
		using Base::Base;

		using Vector = ImplTraits_V<Point>;
		
		static_assert(Vector::template Contains<ILinear>());

		using Base::operator-;

		const Vector operator-(const Point& y) const noexcept {
			return static_cast<const Point*>(this)->impl_affine_minus(y);
		}
	};

	InterfaceTraits_Register(IAffine,
		IAffineSubspace
	);
}
