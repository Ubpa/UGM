#pragma once

#include "IArrayCast.h"
#include "IArrayInOut.h"

namespace Ubpa {
	template<typename Base, typename Impl, typename ArgList>
	struct IArrayUtil : Base {
		using IList = TemplateList<IArrayCast, IArrayInOut>;
		using Base::Base;

		using T = Arg_T<ArgList>;
		static constexpr size_t N = Arg_N<ArgList>;
		using F = Arg_F<ArgList>;

		inline const Impl rmv_epsilon() const noexcept {
			Impl rst{};
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
#ifdef UBPA_USE_XSIMD
			if constexpr (std::is_same_v<T, float> && N == 4)
				return one_minus_t * x + t * y;
			else
#endif // UBPA_USE_XSIMD
			{
				Impl rst{};
				for (size_t i = 0; i < N; i++)
					rst[i] = Ubpa::lerp(x[i], y[i], t);
				return rst;
			}
		}

		inline const Impl lerp(const Impl& y, F t) const noexcept {
			auto& x = static_cast<const Impl&>(*this);
			return lerp(x, y, t);
		}

		static const Impl mid(const Impl& x, const Impl& y) noexcept {
			return lerp(x, y, static_cast<F>(0.5));
		}

		static const Impl mix(const std::vector<Impl>& vals, const std::vector<float>& weights) noexcept {
			assert(vals.size() > 0 && vals.size() == weights.size());
#ifdef UBPA_USE_XSIMD
			if constexpr (std::is_same_v<T, float> && N == 4) {
				auto rst = vals[0].get() * weights[0];
				for (size_t i = 1; i < vals.size(); i++)
					rst += vals[i].get() * weights[i];
				return rst;
			}
			else
#endif // UBPA_USE_XSIMD
			{
				Impl rst;
				for (size_t j = 0; j < N; j++)
					rst[j] = vals[0][j] * weights[0];
				for (size_t i = 1; i < vals.size(); i++) {
					for (size_t j = 0; j < N; j++)
						rst[j] += vals[i][j] * weights[i];
				}
				return rst;
			}
		}
	};
}
