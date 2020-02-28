#pragma once

#include "../../basic.h"
#include "IArray1D.h"

namespace Ubpa {
	template<typename Base, typename Impl, typename ArgList>
	struct IArray1D_Util : SIVT_CRTP<TemplateList<IArray1D>, Base, Impl, ArgList> {
		using SIVT_CRTP<TemplateList<IArray1D>, Base, Impl, ArgList>::SIVT_CRTP;

		using T = Arg_T<ArgList>;
		using F = Arg_F<ArgList>;
		static constexpr size_t N = Arg_N<ArgList>;

		inline const Impl abs() const {
			Impl rst{};
			for (size_t i = 0; i < N; i++)
				rst[i] = std::abs((*this)[i]);
			return rst;
		}

		inline bool has_nan() const noexcept {
			for (size_t i = 0; i < N; i++) {
				if (std::isnan<float>(static_cast<float>((*this)[i])))
					return true;
			}
			return false;
		}

		inline T& min_component() noexcept {
			return (*this)[min_dim()];
		}

		inline T min_component() const noexcept {
			return (*this)[min_dim()];
		}

		inline T& max_component() noexcept {
			return (*this)[max_dim()];
		}

		inline T max_component() const noexcept {
			return (*this)[max_dim()];
		}

		inline constexpr size_t min_dim() const noexcept {
			T minval = (*this)[0];
			size_t minDim = 0;
			for (size_t i = 1; i < N; i++) {
				if ((*this)[i] < minval) {
					minval = (*this)[i];
					minDim = i;
				}
			}
			return minDim;
		}

		inline constexpr size_t max_dim() const noexcept {
			T maxval = (*this)[0];
			size_t maxDim = 0;
			for (size_t i = 1; i < N; i++) {
				if ((*this)[i] < maxval) {
					maxval = (*this)[i];
					maxDim = i;
				}
			}
			return maxDim;
		}

		inline const Impl rmv_epsilon() const noexcept {
			Impl rst{};
			for (size_t i = 0; i < N; i++)
				rst[i] = Ubpa::rmv_epsilon((*this)[i]);
			return rst;
		}

		inline bool is_zero() const noexcept {
			for (size_t i = 0; i < N; i++) {
				if ((*this)[i] != static_cast<F>(0))
					return false;
			}
			return true;
		}
	};
}
