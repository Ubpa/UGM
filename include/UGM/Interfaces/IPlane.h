#pragma once

#include "IOPlane.h"
#include "IAffineRealSubspace.h"

namespace Ubpa {
	// plane in real affine subspace
	template<typename Base, typename Impl, typename ArgList>
	struct IPlane : SIVT_CRTP<TemplateList<IAffineSubspace, IOPlane>, Base, Impl, ArgList> {
		using Point = Arg_Point<ArgList>;
		using Vector = Arg_Vector<ArgList>;

		void init_IPlane(const Point& p, const Vector& n) noexcept {
			this->init_IAffineRealSubspace(p);
			this->init_IOPlane(n);
		}
	};
}
