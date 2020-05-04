#pragma once

#include "IAffineSubspace.h"

namespace Ubpa {
	template<typename Base, typename Point, typename ArgList>
	struct IAffine : Base {
		using IList = TemplateList<IAffineSubspace>;

		using Vector = Arg_Vector<ArgList>;
		
		static_assert(Vector::template IsContain<ILinear>());

		using Base::Base;
		using Base::operator-;

		inline const Vector operator-(const Point& y) const noexcept {
			return static_cast<const Point*>(this)->impl_affine_minus(y);
		}
	};
}
