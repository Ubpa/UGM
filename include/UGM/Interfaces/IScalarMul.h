#pragma once

namespace Ubpa {
	template<typename Base, typename Impl, typename T, typename N>
	struct IScalarMul : Base  {
		static_assert(std::is_floating_point_v<T>, "std::is_floating_point_v<T>");

		using Base::Base;

		template<typename U, typename = std::enable_if_t<std::is_arithmetic_v<U>>>
		const Impl operator*(U k) const noexcept {
			return static_cast<const Impl*>(this)->ImplScalarMul(k);
		}

		template<typename U, typename = std::enable_if_t<std::is_arithmetic_v<U>>>
		Impl& operator*=(U k) noexcept {
			return static_cast<Impl*>(this)->ImplScalarMulToSelf(k);
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
