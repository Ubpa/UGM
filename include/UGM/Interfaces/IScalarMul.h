#pragma once

#include <UTemplate/SI.h>
#include "IVal.h"

namespace Ubpa {
	template<typename Base, typename Impl, typename T, typename N>
	struct IScalarMul : SIVT_CRTP<TemplateList<IVal>, Base, Impl, T, N>  {
		static_assert(std::is_floating_point_v<T>, "std::is_floating_point_v<T>");

		using SIVT_CRTP<TemplateList<IVal>, Base, Impl, T, N>::SI_CRTP;

		const Impl operator*(T k) const noexcept {
			auto& x = static_cast<const Impl&>(*this);
			Impl rst{};
			for (size_t i = 0; i < N::value; i++)
				rst[i] = x[i] * k;
			return rst;
		}

		Impl& operator*=(T k) noexcept {
			auto& x = static_cast<Impl&>(*this);
			for (size_t i = 0; i < N::value; i++)
				x[i] *= k;
			return x;
		}

		friend const Impl operator*(T k, const Impl& x) noexcept {
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
