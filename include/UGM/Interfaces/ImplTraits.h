#pragma once

#include <UTemplate/SI.h>

namespace Ubpa {
	// element type
	template<typename Impl>
	using ImplTraits_T = typename ImplTraits<Impl>::T;

	// element num
	template<typename Impl>
	constexpr size_t ImplTraits_N = ImplTraits<Impl>::N;

	// number field
	template<typename Impl>
	using ImplTraits_F = typename ImplTraits<Impl>::F;

	// vector
	template<typename Impl>
	using ImplTraits_V = typename ImplTraits<Impl>::V;

	// point
	template<typename Impl>
	using ImplTraits_P = typename ImplTraits<Impl>::P;
}
