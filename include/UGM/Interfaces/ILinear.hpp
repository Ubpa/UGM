#pragma once

#include "IAdd.hpp"
#include "IScalarMul.hpp"

#include <UTemplate/SI.hpp>

namespace Ubpa {
	template<typename Base, typename Impl> struct ILinear;
}

SI_CombineInterface(Ubpa::ILinear,
	Ubpa::IAdd,
	Ubpa::IScalarMul
);
