#pragma once

#include <UTemplate/SI.h>
#include "IArray.h"

namespace Ubpa {
	template<typename Base, typename Impl, typename T, typename N>
	struct IScalarMul : SIVT_CRTP<TemplateList<IArray>, Base, Impl, T, N>  {
		static_assert(std::is_floating_point_v<T>, "std::is_floating_point_v<T>");

		using SIVT_CRTP<TemplateList<IArray>, Base, Impl, T, N>::SIVT_CRTP;

		template<typename U, typename = std::enable_if_t<std::is_arithmetic_v<U>>>
		const Impl operator*(U k) const noexcept {
			auto& x = static_cast<const Impl&>(*this);
			auto kT = static_cast<T>(k);
			Impl rst{};
			for (size_t i = 0; i < N::value; i++)
				rst[i] = x[i] * kT;
			return rst;
		}

		template<typename U, typename = std::enable_if_t<std::is_arithmetic_v<U>>>
		Impl& operator*=(U k) noexcept {
			auto& x = static_cast<Impl&>(*this);
			auto kT = static_cast<T>(k);
			for (size_t i = 0; i < N::value; i++)
				x[i] *= kT;
			return x;
		}

		template<typename U, typename = std::enable_if_t<std::is_arithmetic_v<U>>>
		friend const Impl operator*(U k, const Impl& x) noexcept {
			return x * k;
		}

		const Impl operator/(T k) const noexcept {
			assert(k != static_cast<T>(0));
			T inverseK = static_cast<T>(1) / k;
			auto& x = static_cast<const Impl&>(*this);
			return x * inverseK;
		}

		Impl& operator/=(T k) noexcept {
			assert(k != static_cast<T>(0));
			T inverseK = static_cast<T>(1) / k;
			auto& x = static_cast<Impl&>(*this);
			return x *= inverseK;
		}
	};
}
