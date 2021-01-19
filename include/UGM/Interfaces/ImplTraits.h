#pragma once

#include <UTemplate/SI.h>

namespace Ubpa::details {
	template<typename Impl> struct SI_ImplTraits_SupportSIMD;
}

namespace Ubpa {
	// element type
	template<typename Impl>
	using SI_ImplTraits_T = typename SI_ImplTraits<Impl>::T;

	// element num
	template<typename Impl>
	constexpr size_t SI_ImplTraits_N = SI_ImplTraits<Impl>::N;

	// number field
	template<typename Impl>
	using SI_ImplTraits_F = typename SI_ImplTraits<Impl>::F;

	// vector
	template<typename Impl>
	using SI_ImplTraits_V = typename SI_ImplTraits<Impl>::V;

	// point
	template<typename Impl>
	using SI_ImplTraits_P = typename SI_ImplTraits<Impl>::P;

	template<typename Impl>
	constexpr bool SI_ImplTraits_SupportSIMD = details::SI_ImplTraits_SupportSIMD<Impl>::value;
}

namespace Ubpa::details {
#ifdef UBPA_UGM_USE_SIMD
	template<typename Enabler, typename Impl>
	struct SI_ImplTraits_SupportSIMD_Helper : std::true_type {};
	template<typename Impl>
	struct SI_ImplTraits_SupportSIMD_Helper<std::enable_if_t<!SI_ImplTraits<Impl>::support_SIMD>, Impl>
		: std::false_type {};

	template<typename Impl>
	struct SI_ImplTraits_SupportSIMD : IValue<bool,
		SI_ImplTraits_SupportSIMD_Helper<void, Impl>::value
		&& std::is_same_v<SI_ImplTraits_T<Impl>, float>
		&& SI_ImplTraits_N<Impl> == 4> {};
#else
	template<typename Impl>
	struct SI_ImplTraits_SupportSIMD : std::false_type {};
#endif
}
