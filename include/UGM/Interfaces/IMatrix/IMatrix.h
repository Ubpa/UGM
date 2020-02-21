#pragma once

#include "../IArray/IArray.h"

namespace Ubpa {
	// simple [square] 2D array
	// column first
	template<typename Base, typename Impl, typename ArgList>
	struct IMatrix : SIVT_CRTP<TemplateList<IArray>, Base, Impl, ArgList> {
		static_assert(ExistInstance_v<typename Arg_T<ArgList>::AllVBs, IArray>,
			"ExistInstance_v<typename Arg_T<ArgList>::AllVBs, IArray>");

		static constexpr size_t N = Arg_N<ArgList>;

		static_assert(N == 3 || N == 4, "N == 3 || N == 4");
		static_assert(Arg_T<ArgList>::N == N, "Arg_T<ArgList>::N == N");

		using SIVT_CRTP<TemplateList<IArray>, Base, Impl, ArgList>::SIVT_CRTP;
	};
}
