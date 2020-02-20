#pragma once

namespace Ubpa {
	template<typename Base, typename Impl, typename T, typename N>
	struct IAdd : Base {
		using Base::Base;

		const Impl operator+(const Impl& y) const noexcept {
			return static_cast<const Impl*>(this)->ImplAdd(y);
		}

		Impl& operator+=(const Impl& y) noexcept {
			return static_cast<Impl*>(this)->ImplAddToSelf(y);
		}

		const Impl operator-() const noexcept {
			return static_cast<const Impl*>(this)->ImplAddInverse();
		}

		const Impl operator-(const Impl& y) const noexcept {
			return operator+(-y);
		}

		Impl& operator-=(const Impl& y) noexcept {
			return operator+=(-y);
		}
	};
}
