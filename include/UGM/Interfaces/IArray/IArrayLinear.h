#pragma once

#include "IArrayAdd.h"
#include "IArrayScalarMul.h"
#include "../ILinear.h"

namespace Ubpa {
	template<typename Base, typename Impl, typename ArgList>
	struct IArrayLinear : Base {
		using IList = TemplateList<IArrayAdd, IArrayScalarMul, ILinear>;
		using Base::Base;
	};
}
