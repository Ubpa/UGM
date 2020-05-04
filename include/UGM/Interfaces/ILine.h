#pragma once

#include "IOLine.h"
#include "IAffineRealSubspace.h"

namespace Ubpa {
	// line in real affine subspace
	template<typename Base, typename Impl, typename ArgList>
	struct ILine : Base {
		using IList = TemplateList<IAffineRealSubspace, IOLine>;
		using Base::Base;

		using Point = Arg_Point<ArgList>;
		using Vector = Arg_Vector<ArgList>;
		using F = Arg_F<ArgList>;

		void init_ILine(const Point& p, const Vector& dir) noexcept {
			this->init_IAffineRealSubspace(p);
			this->init_IOLine(dir);
		}

		const Point at(F t) const noexcept { return this->point + t * this->dir; }
	};
}
