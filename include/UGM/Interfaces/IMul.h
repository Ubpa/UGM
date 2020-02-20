#pragma once

namespace Ubpa {
	template<typename Base, typename Impl, typename ArgList>
	struct IMul : Base {
		using Base::Base;
		using Base::operator*;

		const Impl operator*(const Impl& y) const noexcept {
			return static_cast<const Impl*>(this)->impl_mul(y);
		}

		/*Impl& operator*=(const Impl& y) noexcept {
			return static_cast<Impl*>(this)->impl_add_to_self(y);
		}*/

		/*const Impl inverse() const noexcept {
			return static_cast<Impl*>(this)->inverse(y);
		}*/
	};
}
