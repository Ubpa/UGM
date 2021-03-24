#pragma once

#include "ILinear.hpp"

namespace Ubpa {
	template<typename Base, typename Impl>
	struct IAffineSubspace : Base {
		using Base::Base;

		using Base::operator+;
		using Base::operator+=;
		using Base::operator-;
		using Base::operator-=;

		using Vector = SI_ImplTraits_V<Impl>;

		static_assert(SI_Contains_v<Vector, ILinear>);

		Impl operator+(const Vector& v) const noexcept {
			return static_cast<const Impl*>(this)->impl_affine_subspace_add(v);
		}

		Impl& operator+=(const Vector& v) noexcept {
			return static_cast<Impl*>(this)->impl_affine_subspace_add_to_self(v);
		}

		Impl operator-(const Vector& v) const noexcept {
			return static_cast<const Impl*>(this)->impl_affine_subspace_minus(v);
		}

		Impl& operator-=(const Vector& v) noexcept {
			return static_cast<Impl*>(this)->impl_affine_subspace_minus_to_self(v);
		}
	};
}
