#pragma once

namespace Ubpa {
	template<typename Base, typename Impl>
	struct IAdd : Base {
		using Base::Base;

		using Base::operator+;
		using Base::operator+=;
		using Base::operator-;
		using Base::operator-=;
		using Base::operator*;
		using Base::operator*=;

		Impl operator+(const Impl& y) const noexcept {
			return static_cast<const Impl*>(this)->impl_add(y);
		}

		Impl& operator+=(const Impl& y) noexcept {
			return static_cast<Impl*>(this)->impl_add_to_self(y);
		}

		Impl operator-() const noexcept {
			return static_cast<const Impl*>(this)->impl_add_inverse();
		}

		Impl operator-(const Impl& y) const noexcept {
			return static_cast<const Impl*>(this)->impl_minus(y);
		}

		Impl& operator-=(const Impl& y) noexcept {
			return static_cast<Impl*>(this)->impl_minus_to_self(y);
		}

		Impl operator*(int k) const noexcept {
			return static_cast<const Impl*>(this)->impl_add_mul(k);
		}

		Impl operator*=(int k) noexcept {
			return static_cast<Impl*>(this)->impl_add_mul_to_self(k);
		}

		friend Impl operator*(int k, const Impl& x) noexcept {
			return x * k;
		}
	};
}
