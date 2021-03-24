#pragma once

#include <UTemplate/SI.hpp>

namespace Ubpa::details {
	template<typename Impl> struct SI_ImplTraits_SupportSIMD;
	template<typename Impl, std::size_t N> struct SI_ImplTraits_SupportImplN;
}

namespace Ubpa {
	// element type
	template<typename Impl>
	using SI_ImplTraits_T = typename SI_ImplTraits<Impl>::T;

	// element num
	template<typename Impl>
	constexpr std::size_t SI_ImplTraits_N = SI_ImplTraits<Impl>::N;

	template<typename Impl, std::size_t N>
	using SI_ImplTraits_ImplN = typename SI_ImplTraits<Impl>::template ImplN<N>;

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

	template<typename Impl, std::size_t N>
	constexpr bool SI_ImplTraits_SupportImplN = details::SI_ImplTraits_SupportImplN<Impl, N>::value;
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

	template<typename Enabler, typename Impl, std::size_t N>
	struct SI_ImplTraits_SupportImplN_Helper : std::false_type {};
	template<typename Impl, std::size_t N>
	struct SI_ImplTraits_SupportImplN_Helper<std::void_t<typename SI_ImplTraits<Impl>::template ImplN<N>>, Impl, N>
		: std::true_type {};

	template<typename Impl, std::size_t N>
	struct SI_ImplTraits_SupportImplN : SI_ImplTraits_SupportImplN_Helper<void, Impl, N> {};
}
