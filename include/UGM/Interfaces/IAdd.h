#pragma once

namespace Ubpa {
	template<typename Base, typename Impl, typename ArgList>
	struct IAdd : Base {
		using Base::Base;

		const Impl operator+(const Impl& y) const noexcept {
			return static_cast<const Impl*>(this)->impl_add(y);
		}

		Impl& operator+=(const Impl& y) noexcept {
			return static_cast<Impl*>(this)->impl_add_to_self(y);
		}

		const Impl operator-() const noexcept {
			return static_cast<const Impl*>(this)->impl_add_inverse();
		}

		const Impl operator-(const Impl& y) const noexcept {
			return operator+(-y);
		}

		Impl& operator-=(const Impl& y) noexcept {
			return operator+=(-y);
		}
	};
}
