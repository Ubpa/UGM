#pragma once

#include "IOPlane.h"
#include "IAffineRealSubspace.h"

namespace Ubpa {
	template<typename Base, typename Impl> struct IPlane;
}

// plane in real affine subspace
SI_CombineInterface(Ubpa::IPlane,
	Ubpa::IAffineRealSubspace,
	Ubpa::IOPlane
);
