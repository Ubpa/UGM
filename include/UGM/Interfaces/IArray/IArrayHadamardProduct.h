#pragma once

#include "../IMul.h"
#include <UTemplate/SI.h>

namespace Ubpa {
	template<typename Base, typename Impl>
	struct IArrayHadamardProduct : Base {
		using Base::Base;

		using F = ImplTraits_F<Impl>;
		using T = ImplTraits_T<Impl>;
		static constexpr size_t N = ImplTraits_N<Impl>;

		using Base::operator*=;
		using Base::operator/;
		using Base::operator/=;

		Impl& operator*=(const Impl& y) noexcept {
			auto& x = static_cast<Impl&>(*this);
#ifdef UBPA_USE_SIMD
			if constexpr (ImplTraits_SupportSIMD<Impl>)
				x = x * y;
			else
#endif // UBPA_USE_SIMD
			for (size_t i = 0; i < N; i++)
				x[i] *= y[i];
			return x;
		}

		const Impl operator/(const Impl& y) const noexcept {
			const auto& x = static_cast<const Impl&>(*this);
#ifdef UBPA_USE_SIMD
			if constexpr (ImplTraits_SupportSIMD<Impl>)
				return _mm_div_ps(x, y);
			else
#endif // UBPA_USE_SIMD
			{
				Impl rst;
				for (size_t i = 0; i < N; i++)
					rst[i] = x[i] / y[i];
				return rst;
			}
		}

		Impl& operator/=(const Impl& y) noexcept {
			auto& x = static_cast<Impl&>(*this);
#ifdef UBPA_USE_SIMD
			if constexpr (ImplTraits_SupportSIMD<Impl>)
				return x = x / y;
			else
#endif // UBPA_USE_SIMD
			for (size_t i = 0; i < N; i++)
				x[i] /= y[i];
			return x;
		}

		inline const Impl inverse() const noexcept {
			const auto& x = static_cast<const Impl&>(*this);

#ifdef UBPA_USE_SIMD
			if constexpr (ImplTraits_SupportSIMD<Impl>)
				return _mm_div_ps(Impl{ 1.f }, x);
			else
#endif // UBPA_USE_SIMD
			{
				Impl rst;
				for (size_t i = 0; i < N; i++)
					rst[i] = static_cast<F>(1) / x[i];
				return rst;
			}
		}

	private:
		template<typename Base, typename Impl>
		friend struct IMul;

		inline const Impl impl_mul(const Impl& y) const noexcept {
			const auto& x = static_cast<const Impl&>(*this);

#ifdef UBPA_USE_SIMD
			if constexpr (ImplTraits_SupportSIMD<Impl>)
				return _mm_mul_ps(x, y);
			else
#endif // UBPA_USE_SIMD
			{
				Impl rst;
				for (size_t i = 0; i < N; i++)
					rst[i] = x[i] * y[i];
				return rst;
			}
		}
	};

	InterfaceTraits_Register(IArrayHadamardProduct,
		IMul, IArray);
}
