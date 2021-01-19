#pragma once

#include "IArray.h"

namespace Ubpa {
	template<typename Base, typename Impl>
	struct IArray1D : Base {
		using Base::Base;
		static_assert(!SI_Contains_v<SI_ImplTraits_T<Impl>, IArray>);
	};

	SI_InterfaceTraits_Register(IArray1D,
		IArray
	);
}
