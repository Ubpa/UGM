#pragma once

#include "IAffineSubspace.h"

namespace Ubpa {
	template<typename Base, typename Point, typename ArgList>
	struct IAffine : SIVT_CRTP<TemplateList<IAffineSubspace>, Base, Point, ArgList> {
		using Vector = Arg_Vector<ArgList>;
		
		static_assert(ExistInstance_v<typename Vector::AllVBs, ILinear>);

		using DBase = SIVT_CRTP<TemplateList<IAffineSubspace>, Base, Point, ArgList>;
		using DBase::DBase;
		using DBase::operator-;

		inline const Vector operator-(const Point& y) const noexcept {
			return static_cast<const Point*>(this)->impl_affine_minus(y);
		}
	};
}
