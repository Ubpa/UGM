#pragma once

#include "IArrayCast.hpp"
#include "IArrayInOut.hpp"

namespace Ubpa {
	template<typename Base, typename Impl>
	struct IArrayUtil : Base {
		using Base::Base;

		using T = SI_ImplTraits_T<Impl>;
		static constexpr size_t N = SI_ImplTraits_N<Impl>;
		using F = SI_ImplTraits_F<Impl>;

		Impl rmv_epsilon() const noexcept {
			Impl rst;
			for (size_t i = 0; i < N; i++)
				rst[i] = Ubpa::rmv_epsilon((*this)[i]);
			return rst;
		}

		bool is_all_zero() const noexcept {
			for (size_t i = 0; i < N; i++) {
				if (!Ubpa::is_all_zero((*this)[i]))
					return false;
			}
			return true;
		}

		bool is_any_nan() const noexcept {
			for (size_t i = 0; i < N; i++) {
				if (Ubpa::is_any_nan((*this)[i]))
					return true;
			}
			return false;
		}

		static Impl lerp(const Impl& x, const Impl& y, F t) noexcept {
			F one_minus_t = static_cast<F>(1) - t;
#ifdef UBPA_UGM_USE_SIMD
			if constexpr (SI_ImplTraits_SupportSIMD<Impl>)
				return _mm_add_ps(_mm_mul_ps(x, _mm_set1_ps(one_minus_t)), _mm_mul_ps(y, _mm_set1_ps(t)));
			else
#endif // UBPA_UGM_USE_SIMD
			{
				Impl rst;
				for (size_t i = 0; i < N; i++)
					rst[i] = Ubpa::lerp(x[i], y[i], t);
				return rst;
			}
		}

		Impl lerp(const Impl& y, F t) const noexcept {
			const auto& x = static_cast<const Impl&>(*this);
			return lerp(x, y, t);
		}

		static Impl hadamard_product(const Impl& x, const Impl& y) noexcept {
#ifdef UBPA_UGM_USE_SIMD
			if constexpr (SI_ImplTraits_SupportSIMD<Impl>)
				return _mm_mul_ps(x, y);
			else
#endif // UBPA_UGM_USE_SIMD
			{
				Impl rst;
				for (size_t i = 0; i < N; i++)
					rst[i] = Ubpa::hadamard_product(x[i], y[i]);
				return rst;
			}
		}

		Impl hadamard_product(const Impl& y) const noexcept {
			const auto& x = static_cast<const Impl&>(*this);
			return hadamard_product(x, y);
		}

		static Impl mid(const Impl& x, const Impl& y) noexcept {
			return lerp(x, y, static_cast<F>(0.5));
		}

		template<typename ValContainer, typename WeightContainer>
		static Impl mix(const ValContainer& vals, const WeightContainer& weights) noexcept {
			assert(vals.size() > 0 && vals.size() == weights.size());
			auto val_iter = vals.begin();
			auto weight_iter = weights.begin();

#ifdef UBPA_UGM_USE_SIMD
			if constexpr (SI_ImplTraits_SupportSIMD<Impl>) {
				__m128 rst = _mm_mul_ps(*val_iter, *weight_iter);
				++val_iter;
				++weight_iter;
				while (val_iter != vals.end()) {
					rst = _mm_add_ps(rst, _mm_mul_ps(*val_iter, *weight_iter));
					++val_iter;
					++weight_iter;
				}
				return rst;
			}
			else
#endif // UBPA_UGM_USE_SIMD
			{
				Impl rst;
				for (size_t j = 0; j < N; j++)
					rst[j] = (*val_iter)[j] * (*weight_iter);
				while (val_iter != vals.end()) {
					for (size_t j = 0; j < N; j++)
						rst[j] += (*val_iter)[j] * (*weight_iter);
					++val_iter;
					++weight_iter;
				}
				return rst;
			}
		}

		Impl abs() const noexcept {
			const auto& x = static_cast<const Impl&>(*this);
#ifdef UBPA_UGM_USE_SIMD
			if constexpr (SI_ImplTraits_SupportSIMD<Impl>)
				return _mm_abs_ps(x);
			else
#endif // UBPA_UGM_USE_SIMD
			{
				Impl rst;
				for (size_t i = 0; i < N; i++)
					rst[i] = Ubpa::abs(x[i]);
				return rst;
			}
		}

		template<typename Other>
		Impl hadamard_mul(const Other& y) const noexcept {
			static_assert(SI_ImplTraits_N<Other> == N && std::is_same_v<SI_ImplTraits_T<Other>, T>);
			const auto& x = static_cast<const Impl&>(*this);
#ifdef UBPA_UGM_USE_SIMD
			if constexpr (SI_ImplTraits_SupportSIMD<Impl> && SI_ImplTraits_SupportSIMD<Other>)
				return _mm_mul_ps(x, y);
			else
#endif // UBPA_UGM_USE_SIMD
			{
				Impl rst;
				for (size_t i = 0; i < N; i++)
					rst[i] = x[i] * y[i];
				return rst;
			}
		}

		template<typename Other>
		Impl hadamard_div(const Other& y) const noexcept {
			static_assert(SI_ImplTraits_N<Other> == N && std::is_same_v<SI_ImplTraits_T<Other>, T>);
			const auto& x = static_cast<const Impl&>(*this);
#ifdef UBPA_UGM_USE_SIMD
			if constexpr (SI_ImplTraits_SupportSIMD<Impl> && SI_ImplTraits_SupportSIMD<Other>)
				return _mm_div_ps(x, y);
			else
#endif // UBPA_UGM_USE_SIMD
			{
				Impl rst;
				for (size_t i = 0; i < N; i++)
					rst[i] = x[i] / y[i];
				return rst;
			}
		}
	};
}

SI_InterfaceTraits_Register(Ubpa::IArrayUtil,
	Ubpa::IArrayCast,
	Ubpa::IArrayInOut
);
