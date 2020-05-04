#pragma once

#include "IArray.h"

namespace Ubpa {
	template<typename Base, typename Impl, typename ArgList>
	struct IArray1D : Base {
		using IList = TemplateList<IArray>;
		using Base::Base;
		static_assert(std::is_same_v<Arg_T<ArgList>, Arg_F<ArgList>>);
	};
}
