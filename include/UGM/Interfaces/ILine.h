#pragma once

#include "IOLine.h"
#include "IAffineRealSubspace.h"

namespace Ubpa {
	// line in real affine subspace
	template<typename Base, typename Impl, typename ArgList>
	struct ILine : SIVT_CRTP<TemplateList<IAffineRealSubspace, IOLine>, Base, Impl, ArgList> {
		using Point = Arg_Point<ArgList>;
		using Vector = Arg_Vector<ArgList>;
		using F = Arg_F<ArgList>;

		using SIVT_CRTP<TemplateList<IAffineRealSubspace, IOLine>, Base, Impl, ArgList>::SIVT_CRTP;

		const Point at(F t) const noexcept { return this->point() + t * this->dir(); }

		void init_ILine(const Point& p, const Vector& dir) noexcept {
			this->init_IAffineRealSubspace(p);
			this->init_IOLine(dir);
		}
	};
}
