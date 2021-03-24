#pragma once

#include "IOPlane.hpp"
#include "IAffineRealSubspace.hpp"

namespace Ubpa {
	template<typename Base, typename Impl> struct IPlane;
}

// plane in real affine subspace
SI_CombineInterface(Ubpa::IPlane,
	Ubpa::IAffineRealSubspace,
	Ubpa::IOPlane
);
