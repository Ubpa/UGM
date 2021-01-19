#pragma once

#include "IArrayAdd.h"
#include "IArrayScalarMul.h"
#include "../ILinear.h"

namespace Ubpa {
	SI_CombineInterface(IArrayLinear,
		IArrayAdd,
		IArrayScalarMul,
		ILinear
	);
}
