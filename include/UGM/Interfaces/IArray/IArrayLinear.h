#pragma once

#include "IArrayAdd.h"
#include "IArrayScalarMul.h"
#include "../ILinear.h"

namespace Ubpa {
	template<typename Base, typename Impl, typename ArgList>
	struct IArrayLinear : SIVT_CRTP<TemplateList<IArrayAdd, IArrayScalarMul, ILinear>, Base, Impl, ArgList> {
		using SIVT_CRTP<TemplateList<IArrayAdd, IArrayScalarMul, ILinear>, Base, Impl, ArgList>::SIVT_CRTP;
	};
}
