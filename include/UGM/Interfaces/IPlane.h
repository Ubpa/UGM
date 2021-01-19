#pragma once

#include "IOPlane.h"
#include "IAffineRealSubspace.h"

namespace Ubpa {
	// plane in real affine subspace
	SI_CombineInterface(IPlane,
		IAffineRealSubspace,
		IOPlane
	);
}
