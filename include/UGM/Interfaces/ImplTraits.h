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

#ifdef UBPA_USE_SIMD
	template<typename Impl>
	struct SupportSIMD : IValue<bool, std::is_same_v<ImplTraits_T<Impl>, float> && ImplTraits_N<Impl> == 4> {};
#else
	template<typename Impl>
	struct SupportSIMD : std::false_type {};
#endif
	template<typename Impl>
	constexpr bool SupportSIMD_v = SupportSIMD<Impl>::value;
}
