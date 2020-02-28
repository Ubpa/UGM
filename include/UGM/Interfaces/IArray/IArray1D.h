#pragma once

#include "IArray.h"

namespace Ubpa {
	template<typename Base, typename Impl, typename ArgList>
	struct IArray1D : SIVT_CRTP<TemplateList<IArray>, Base, Impl, ArgList> {
		using SIVT_CRTP<TemplateList<IArray>, Base, Impl, ArgList>::SIVT_CRTP;
		static_assert(std::is_same_v<Arg_T<ArgList>, Arg_F<ArgList>>);
	};
}
