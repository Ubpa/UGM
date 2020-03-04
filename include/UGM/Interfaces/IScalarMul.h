#pragma once

#include "Arg.h"

#include <UTemplate/SI.h>

#include <assert.h>

namespace Ubpa {
	template<typename Base, typename Impl, typename ArgList>
	struct IScalarMul : Base  {
		using F = Arg_F<ArgList>;

		//static_assert(std::is_floating_point_v<F>);

		using Base::Base;
		using Base::operator*;
		using Base::operator*=;
		using Base::operator/;
		using Base::operator/=;

		template<typename U, typename = std::enable_if_t<std::is_arithmetic_v<U>>>
		inline const Impl operator*(U k) const noexcept {
			static_assert(!std::is_floating_point_v<U> || std::is_floating_point_v<F>);
			return static_cast<const Impl*>(this)->impl_scalar_mul(k);
		}

		template<typename U, typename = std::enable_if_t<std::is_arithmetic_v<U>>>
		inline Impl& operator*=(U k) noexcept {
			static_assert(!std::is_floating_point_v<U> || std::is_floating_point_v<F>);
			return static_cast<Impl*>(this)->impl_scalar_mul_to_self(k);
		}

		template<typename U, typename = std::enable_if_t<std::is_arithmetic_v<U>>>
		inline friend const Impl operator*(U k, const Impl& x) noexcept {
			static_assert(!std::is_floating_point_v<U> || std::is_floating_point_v<F>);
			return x * k;
		}

		inline const Impl operator/(F k) const noexcept {
			static_assert(std::is_floating_point_v<F>);
			assert(k != static_cast<F>(0));
			F inverseK = static_cast<F>(1) / k;
			auto& x = static_cast<const Impl&>(*this);
			return x * inverseK;
		}

		inline Impl& operator/=(F k) noexcept {
			static_assert(std::is_floating_point_v<F>);
			assert(k != static_cast<F>(0));
			F inverseK = static_cast<F>(1) / k;
			auto& x = static_cast<Impl&>(*this);
			return x *= inverseK;
		}
	};
}
