#pragma once

#include "IAdd.h"
#include "IScalarMul.h"

#include <UTemplate/SI.h>

namespace Ubpa {
	template<typename Base, typename Impl> struct ILinear;
}

SI_CombineInterface(Ubpa::ILinear,
	Ubpa::IAdd,
	Ubpa::IScalarMul
);
