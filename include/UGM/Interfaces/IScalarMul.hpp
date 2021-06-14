#pragma once

#include "ImplTraits.hpp"
#include "IAdd.hpp"

#include <cassert>

namespace Ubpa::details::IScalarMul_ {
	template<typename Impl, typename U>
	static constexpr bool need_mul = !std::is_integral_v<U> || !SI_Contains_v<Impl, IAdd>;
}

namespace Ubpa {
	template<typename Base, typename Impl>
	struct IScalarMul : Base  {
		using F = SI_ImplTraits_F<Impl>;

		//static_assert(std::is_floating_point_v<F>);
		template<typename U>
		static constexpr bool support = (std::is_floating_point_v<F> && std::is_floating_point_v<U> ||
			!std::is_floating_point_v<F> && std::is_same_v<U, F>)
			&& details::IScalarMul_::need_mul<Impl, U>;

		using Base::Base;

		using Base::operator*;
		using Base::operator*=;
		using Base::operator/;
		using Base::operator/=;

		template<typename U, std::enable_if_t<support<U>>* = nullptr>
		Impl operator*(U k) const noexcept {
			return static_cast<const Impl*>(this)->impl_scalar_mul(static_cast<F>(k));
		}

		template<typename U, std::enable_if_t<support<U>>* = nullptr>
		Impl& operator*=(U k) noexcept {
			return static_cast<Impl*>(this)->impl_scalar_mul_to_self(static_cast<F>(k));
		}

		template<typename U, std::enable_if_t<support<U>>* = nullptr>
		friend Impl operator*(U k, const Impl& x) noexcept {
			return x * k;
		}

		template<typename U, std::enable_if_t<support<U>>* = nullptr>
		Impl operator/(U k) const noexcept {
			assert(k != static_cast<U>(0));
			F inverseK = static_cast<F>(1) / static_cast<F>(k);
			const auto& x = static_cast<const Impl&>(*this);
			return x * inverseK;
		}

		template<typename U, std::enable_if_t<support<U>>* = nullptr>
		Impl& operator/=(U k) noexcept {
			assert(k != static_cast<F>(0));
			F inverseK = static_cast<F>(1) / static_cast<F>(k);
			auto& x = static_cast<Impl&>(*this);
			return x *= inverseK;
		}
	};
}
