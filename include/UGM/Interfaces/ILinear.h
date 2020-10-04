#pragma once

#include "IAdd.h"
#include "IScalarMul.h"

#include <UTemplate/SI.h>

namespace Ubpa {
	CombineInterface(ILinear,
		IAdd,
		IScalarMul
	);
}
