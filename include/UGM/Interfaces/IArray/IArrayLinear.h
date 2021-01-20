#pragma once

#include "IArrayAdd.h"
#include "IArrayScalarMul.h"
#include "../ILinear.h"

namespace Ubpa {
	template<typename Base, typename Impl> struct IArrayLinear;
}

SI_CombineInterface(Ubpa::IArrayLinear,
	Ubpa::IArrayAdd,
	Ubpa::IArrayScalarMul,
	Ubpa::ILinear
);
