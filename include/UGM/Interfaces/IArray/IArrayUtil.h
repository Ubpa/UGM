#pragma once

#include "IArrayCast.h"
#include "IArrayInOut.h"

namespace Ubpa {
	template<typename Base, typename Impl, typename ArgList>
	struct IArrayUtil : SIVT_CRTP<TemplateList<IArrayCast, IArrayInOut>, Base, Impl, ArgList> {
		using SIVT_CRTP<TemplateList<IArrayCast, IArrayInOut>, Base, Impl, ArgList>::SIVT_CRTP;
	};
}
