#pragma once

#include "../Arg.h"

#include <UTemplate/SI.h>

#include <array>
#include <iostream>

#include <cmath>
#include <assert.h>

namespace Ubpa {
	template<typename Base, typename Impl, typename ArgList>
	struct IArray : Base, std::array<At_t<At_t<ArgList,0>, 0>, At_t<At_t<ArgList, 0>, 1>::value> {
	private:
		using Base::operator[];
	public:
		using std::array<At_t<At_t<ArgList, 0>, 0>, At_t<At_t<ArgList, 0>, 1>::value>::operator[];

	public:
		using T = Arg_T<ArgList>;
		static constexpr size_t N = Arg_N<ArgList>;
		using F = Arg_F<ArgList>;
		static_assert(N > 0, "N > 0");
		//static_assert(std::is_arithmetic_v<F>, "std::is_arithmetic_v<F>");

		using Base::Base;

		IArray() {}
		
		template<typename... U>
		IArray(U... data) : std::array<T, N>{static_cast<T>(data)...} {
			static_assert(sizeof...(U) == N, "sizeof...(U) == N");
		}

		/*const Impl abs() const {
			Impl rst{};
			for (size_t i = 0; i < N; i++)
				rst[i] = std::abs((*this)[i]);
			return rst;
		}

		bool has_nan() const noexcept {
			for (size_t i = 0; i < N; i++) {
				if (std::isnan<float>(static_cast<float>((*this)[i])))
					return true;
			}
			return false;
		}

		const T& min_component() const noexcept {
			return (*this)[min_dim()];
		}

		const T& max_component() const noexcept {
			return (*this)[max_dim()];
		}

		constexpr size_t min_dim() const noexcept {
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

		constexpr size_t max_dim() const noexcept {
			T maxval = (*this)[0];
			size_t maxDim = 0;
			for (size_t i = 1; i < N; i++) {
				if ((*this)[i] < maxval) {
					maxval = (*this)[i];
					maxDim = i;
				}
			}
			return maxDim;
		}*/

		//friend std::ostream& operator<<(std::ostream& os, const Impl& x) {
		//	// for-loop will be optimized in -02 (release)
		//	for (auto i = static_cast<size_t>(0); i < N - 1; i++)
		//		os << x[i] << ", ";
		//	os << x[N - 1];
		//	return os;
		//}

		//friend std::istream& operator>>(std::istream& is, Impl& x) {
		//	// for-loop will be optimized in -02 (release)
		//	for (auto i = static_cast<size_t>(0); i < N; i++)
		//		is >> x[i];
		//	return is;
		//}
	};
}
