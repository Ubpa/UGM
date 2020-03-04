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
			T minVal = (*this)[0];
			size_t minDim = 0;
			for (size_t i = 1; i < N; i++) {
				if ((*this)[i] < minVal) {
					minVal = (*this)[i];
					minDim = i;
				}
			}
			return minDim;
		}

		inline constexpr size_t max_dim() const noexcept {
			T maxVal = (*this)[0];
			size_t maxDim = 0;
			for (size_t i = 1; i < N; i++) {
				if ((*this)[i] > maxVal) {
					maxVal = (*this)[i];
					maxDim = i;
				}
			}
			return maxDim;
		}

		static const Impl min(const Impl& x, const Impl& y) noexcept {
			Impl rst{};
			for (size_t i = 0; i < N; i++)
				rst[i] = std::min(x[i], y[i]);
			return rst;
		}

		static const Impl max(const Impl& x, const Impl& y) noexcept {
			Impl rst{};
			for (size_t i = 0; i < N; i++)
				rst[i] = std::max(x[i], y[i]);
			return rst;
		}
	};
}
