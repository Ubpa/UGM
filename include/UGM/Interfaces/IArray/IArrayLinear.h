#pragma once

#include "IArrayAdd.h"
#include "IArrayScalarMul.h"
#include "../ILinear.h"

namespace Ubpa {
	CombineInterface(IArrayLinear,
		IArrayAdd,
		IArrayScalarMul,
		ILinear);
}
