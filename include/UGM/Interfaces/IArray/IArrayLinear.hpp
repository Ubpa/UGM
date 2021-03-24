#pragma once

#include "IArrayAdd.hpp"
#include "IArrayScalarMul.hpp"
#include "../ILinear.hpp"

namespace Ubpa {
	template<typename Base, typename Impl> struct IArrayLinear;
}

SI_CombineInterface(Ubpa::IArrayLinear,
	Ubpa::IArrayAdd,
	Ubpa::IArrayScalarMul,
	Ubpa::ILinear
);
