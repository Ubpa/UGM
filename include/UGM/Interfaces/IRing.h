#pragma once

#include "IAdd.h"
#include "IMul.h"

#include <UTemplate/SI.h>

namespace Ubpa {
	template<typename Base, typename Impl, typename ArgList>
	struct IRing : Base {
		using IList = TemplateList<IMul, IAdd>;
		using Base::Base;
	};
}
