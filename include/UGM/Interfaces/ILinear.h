#pragma once

#include "IAdd.h"
#include "IScalarMul.h"

namespace Ubpa {
	template<typename Base, typename Impl, typename ArgList>
	struct ILinear : Base {
		using IList = TemplateList<IAdd, IScalarMul>;
		using Base::Base;
	};
}
