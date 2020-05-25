#pragma once

#include "ImplTraits.h"
#include "IAdd.h"

#include <cassert>

namespace Ubpa::detail::IScalarMul_ {
	template<typename Impl, typename U>
	static constexpr bool need_mul = !std::is_integral_v<U> || !SI_IsContain_v<Impl, IAdd>;
}

namespace Ubpa {
	template<typename Base, typename Impl>
	struct IScalarMul : Base  {
		using F = ImplTraits_F<Impl>;

		//static_assert(std::is_floating_point_v<F>);
		template<typename U>
		static constexpr bool support = std::is_same_v<U, F> && detail::IScalarMul_::need_mul<Impl, U>;

		using Base::Base;

		using Base::operator*;
		using Base::operator*=;
		using Base::operator/;
		using Base::operator/=;

		template<typename U, std::enable_if_t<support<U>>* = nullptr>
		inline const Impl operator*(U k) const noexcept {
			return static_cast<const Impl*>(this)->impl_scalar_mul(k);
		}

		template<typename U, std::enable_if_t<support<U>>* = nullptr>
		inline Impl& operator*=(U k) noexcept {
			return static_cast<Impl*>(this)->impl_scalar_mul_to_self(k);
		}

		template<typename U, std::enable_if_t<support<U>>* = nullptr>
		inline friend const Impl operator*(U k, const Impl& x) noexcept {
			return x * k;
		}

		inline const Impl operator/(F k) const noexcept {
			assert(k != static_cast<F>(0));
			F inverseK = static_cast<F>(1) / k;
			auto& x = static_cast<const Impl&>(*this);
			return x * inverseK;
		}

		inline Impl& operator/=(F k) noexcept {
			assert(k != static_cast<F>(0));
			F inverseK = static_cast<F>(1) / k;
			auto& x = static_cast<Impl&>(*this);
			return x *= inverseK;
		}
	};
}
