#pragma once

#include "IArrayCast.h"
#include "IArrayInOut.h"

namespace Ubpa {
	template<typename Base, typename Impl>
	struct IArrayUtil : Base {
		using Base::Base;

		using T = ImplTraits_T<Impl>;
		static constexpr size_t N = ImplTraits_N<Impl>;
		using F = ImplTraits_F<Impl>;

		inline const Impl rmv_epsilon() const noexcept {
			Impl rst;
			for (size_t i = 0; i < N; i++)
				rst[i] = Ubpa::rmv_epsilon((*this)[i]);
			return rst;
		}

		inline bool is_all_zero() const noexcept {
			for (size_t i = 0; i < N; i++) {
				if (is_zero((*this)[i]))
					return false;
			}
			return true;
		}

		inline bool has_nan() const noexcept {
			for (size_t i = 0; i < N; i++) {
				if (is_nan((*this)[i]))
					return true;
			}
			return false;
		}

		static const Impl lerp(const Impl& x, const Impl& y, F t) noexcept {
			F one_minus_t = static_cast<F>(1) - t;
#ifdef UBPA_USE_SIMD
			if constexpr (SupportSIMD_v<Impl>)
				return _mm_add_ps(_mm_mul_ps(x, _mm_set1_ps(one_minus_t)), _mm_mul_ps(y, _mm_set1_ps(t)));
			else
#endif // UBPA_USE_SIMD
			{
				Impl rst;
				for (size_t i = 0; i < N; i++)
					rst[i] = Ubpa::lerp(x[i], y[i], t);
				return rst;
			}
		}

		inline const Impl lerp(const Impl& y, F t) const noexcept {
			const auto& x = static_cast<const Impl&>(*this);
			return lerp(x, y, t);
		}

		static const Impl mid(const Impl& x, const Impl& y) noexcept {
			return lerp(x, y, static_cast<F>(0.5));
		}

		template<typename ValContainer, typename WeightContainer>
		static const Impl mix(const ValContainer& vals, const WeightContainer& weights) noexcept {
			assert(vals.size() > 0 && vals.size() == weights.size());
			auto val_iter = vals.begin();
			auto weight_iter = weights.begin();

#ifdef UBPA_USE_SIMD
			if constexpr (SupportSIMD_v<Impl>) {
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
#endif // UBPA_USE_SIMD
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
	};

	InterfaceTraits_Regist(IArrayUtil,
		IArrayCast, IArrayInOut);
}
