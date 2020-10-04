#pragma once

#include "ILinear.h"

namespace Ubpa {
	template<typename Base, typename Impl>
	struct IAffineSubspace : Base {
		using Base::Base;

		using Vector = ImplTraits_V<Impl>;
		
		static_assert(Vector::template Contains<ILinear>());

		inline const Impl operator+(const Vector& v) const noexcept {
			return static_cast<const Impl*>(this)->impl_affine_subspace_add(v);
		}

		inline Impl& operator+=(const Vector& v) noexcept {
			return static_cast<Impl*>(this)->impl_affine_subspace_add_to_self(v);
		}

		inline const Impl operator-(const Vector& v) const noexcept {
			return static_cast<const Impl*>(this)->impl_affine_subspace_minus(v);
		}

		inline Impl& operator-=(const Vector& v) noexcept {
			return static_cast<Impl*>(this)->impl_affine_subspace_minus_to_self(v);
		}
	};
}
