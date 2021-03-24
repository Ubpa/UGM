#pragma once

#include "IOLine.hpp"
#include "IAffineRealSubspace.hpp"

namespace Ubpa {
	// line in real affine subspace
	template<typename Base, typename Impl>
	struct ILine : Base {
		using Base::Base;

		using Point = SI_ImplTraits_P<Impl>;
		using Vector = SI_ImplTraits_V<Impl>;
		using F = SI_ImplTraits_F<Impl>;

		Point at(F t) const noexcept { return this->point + t * this->dir; }
	};
}

SI_InterfaceTraits_Register(Ubpa::ILine,
	Ubpa::IAffineRealSubspace,
	Ubpa::IOLine
);