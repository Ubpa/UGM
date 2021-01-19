#pragma once

#include "IAdd.h"
#include "IScalarMul.h"

#include <UTemplate/SI.h>

namespace Ubpa {
	SI_CombineInterface(ILinear,
		IAdd,
		IScalarMul
	);
}
