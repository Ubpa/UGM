#pragma once

#include "IArray.h"
#include "../IAdd.h"

namespace Ubpa {
	template<typename Base, typename Impl>
	struct IArrayAdd : Base {
		using Base::Base;

		using T = ImplTraits_T<Impl>;
		static constexpr size_t N = ImplTraits_N<Impl>;


	private:
		template<typename Base, typename Impl>
		friend struct IAdd;

		Impl impl_add(const Impl& y) const noexcept {
			const auto& x = static_cast<const Impl&>(*this);
#ifdef UBPA_UGM_USE_SIMD
			if constexpr (ImplTraits_SupportSIMD<Impl>)
				return _mm_add_ps(x, y);
			else
#endif // UBPA_UGM_USE_SIMD
			{
				Impl rst;
				for (size_t i = 0; i < N; i++)
					rst[i] = x[i] + y[i];
				return rst;
			}
		}

		Impl& impl_add_to_self(const Impl& y) noexcept {
			auto& x = static_cast<Impl&>(*this);
#ifdef UBPA_UGM_USE_SIMD
			if constexpr (ImplTraits_SupportSIMD<Impl>)
				return x = x + y;
			else
#endif // UBPA_UGM_USE_SIMD
			{
				for (size_t i = 0; i < N; i++)
					x[i] += y[i];
				return x;
			}
		}

		Impl impl_add_inverse() const noexcept {
			const auto& x = static_cast<const Impl&>(*this);
#ifdef UBPA_UGM_USE_SIMD
			if constexpr (ImplTraits_SupportSIMD<Impl>)
				// ref: https://stackoverflow.com/questions/20083997/how-to-negate-change-sign-of-the-floating-point-elements-in-a-m128-type-vari
				return _mm_sub_ps(Impl{ 0.f }, x);
			else
#endif // UBPA_UGM_USE_SIMD
			{
				Impl rst;
				for (size_t i = 0; i < N; i++) {
					if constexpr(std::is_unsigned_v<T>)
						rst[i] = static_cast<T>(-static_cast<std::make_signed_t<T>>(x[i]));
					else
						rst[i] = -x[i];
				}
				return rst;
			}
		}

		Impl impl_minus(const Impl& y) const noexcept {
			const auto& x = static_cast<const Impl&>(*this);
#ifdef UBPA_UGM_USE_SIMD
			if constexpr (ImplTraits_SupportSIMD<Impl>) {
				return _mm_sub_ps(x, y);
			}
			else
#endif // UBPA_UGM_USE_SIMD
			{
				Impl rst;
				for (size_t i = 0; i < N; i++)
					rst[i] = x[i] - y[i];
				return rst;
			}
		}

		Impl& impl_minus_to_self(const Impl& y) noexcept {
			auto& x = static_cast<Impl&>(*this);
#ifdef UBPA_UGM_USE_SIMD
			if constexpr (ImplTraits_SupportSIMD<Impl>) {
				return x = x - y;
			}
			else
#endif // UBPA_UGM_USE_SIMD
			{
				for (size_t i = 0; i < N; i++)
					x[i] -= y[i];
				return x;
			}
		}

		template<typename U, std::enable_if_t<std::is_integral_v<U>>* = nullptr>
		Impl impl_add_mul(U v) const noexcept {
			const auto& x = static_cast<const Impl&>(*this);
#ifdef UBPA_UGM_USE_SIMD
			if constexpr (ImplTraits_SupportSIMD<Impl>)
				return _mm_mul_ps(x, Impl{ v });
			else
#endif // UBPA_UGM_USE_SIMD
			{
				Impl rst;
				for (size_t i = 0; i < N; i++)
					rst[i] = x[i] * v;
				return rst;
			}
		}

		template<typename U, std::enable_if_t<std::is_integral_v<U>>* = nullptr>
		Impl& impl_add_mul_to_self(U v) noexcept {
			auto& x = static_cast<Impl&>(*this);
#ifdef UBPA_UGM_USE_SIMD
			if constexpr (ImplTraits_SupportSIMD<Impl>)
				return x = x * v;
			else
#endif // UBPA_UGM_USE_SIMD
			{
				for (size_t i = 0; i < N; i++)
					x[i] *= v;
				return x;
			}
		}
	};

	InterfaceTraits_Register(IArrayAdd,
		IAdd,
		IArray
	);
}
