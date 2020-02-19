#pragma once

#include "Interfaces/ILinear.h"

namespace Ubpa {
	template<typename T, size_t N>
	struct Vec : SIIT_CRTP<TemplateList<ILinear>, Vec<T, N>, T, Size<N>> {
		using SIIT_CRTP<TemplateList<ILinear>, Vec<T, N>, T, Size<N>>::SIIT_CRTP;
	};

	template<size_t N>
	using Vecf = Vec<float, N>;

	using Vecf1 = Vecf<1>;
	using Vecf2 = Vecf<2>;
	using Vecf3 = Vecf<3>;

	template<size_t N>
	using Veci = Vec<int, N>;

	using Veci1 = Veci<1>;
	using Veci2 = Veci<2>;
	using Veci3 = Veci<3>;

	template<size_t N>
	using Vecu = Vec<unsigned, N>;

	using Vecu1 = Vecu<1>;
	using Vecu2 = Vecu<2>;
	using Vecu3 = Vecu<3>;
}
