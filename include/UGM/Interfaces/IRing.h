#pragma once

#include "IAdd.h"
#include "IMul.h"

#include <UTemplate/SI.h>

namespace Ubpa {
	template<typename Base, typename Impl, typename ArgList>
	struct IRing : SIVT_CRTP<TemplateList<IMul, IAdd>, Base, Impl, ArgList> {
		using SIVT_CRTP<TemplateList<IMul, IAdd>, Base, Impl, ArgList>::SIVT_CRTP;
	};
}
