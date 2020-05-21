#pragma once

#include "IArray.h"

namespace Ubpa {
	template<typename Base, typename Impl>
	struct IArray1D : Base {
		using Base::Base;
		static_assert(!SI_IsContain_v<ImplTraits_T<Impl>, IArray>);
	};

	InterfaceTraits_Regist(IArray1D,
		IArray);
}
