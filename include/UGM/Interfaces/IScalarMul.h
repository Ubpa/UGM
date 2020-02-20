#pragma once

#include <UTemplate/SI.h>
#include "Arg.h"

namespace Ubpa {
	template<typename Base, typename Impl, typename ArgList>
	struct IScalarMul : Base  {
		using F = Arg_F<ArgList>;

		static_assert(std::is_floating_point_v<F>, "std::is_floating_point_v<T>");

		using Base::Base;

		template<typename U, typename = std::enable_if_t<std::is_arithmetic_v<U>>>
		const Impl operator*(U k) const noexcept {
			return static_cast<const Impl*>(this)->impl_scalar_mul(k);
		}

		template<typename U, typename = std::enable_if_t<std::is_arithmetic_v<U>>>
		Impl& operator*=(U k) noexcept {
			return static_cast<Impl*>(this)->impl_scalar_mul_to_self(k);
		}

		template<typename U, typename = std::enable_if_t<std::is_arithmetic_v<U>>>
		friend const Impl operator*(U k, const Impl& x) noexcept {
			return x * k;
		}

		const Impl operator/(F k) const noexcept {
			assert(k != static_cast<F>(0));
			F inverseK = static_cast<F>(1) / k;
			auto& x = static_cast<const Impl&>(*this);
			return x * inverseK;
		}

		Impl& operator/=(F k) noexcept {
			assert(k != static_cast<F>(0));
			F inverseK = static_cast<F>(1) / k;
			auto& x = static_cast<Impl&>(*this);
			return x *= inverseK;
		}
	};
}
