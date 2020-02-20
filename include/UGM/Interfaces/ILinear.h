#pragma once

#include "IAdd.h"
#include "IScalarMul.h"

namespace Ubpa {
	template<typename Base, typename Impl, typename ArgList>
	struct ILinear : SIVT_CRTP<TemplateList<IAdd, IScalarMul>, Base, Impl, ArgList> {
		using SIVT_CRTP<TemplateList<IAdd, IScalarMul>, Base, Impl, ArgList>::SIVT_CRTP;
	};
}
