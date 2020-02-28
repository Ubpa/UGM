#pragma once

#include "ILinear.h"

namespace Ubpa {
	template<typename Base, typename ImplP, typename ArgList>
	struct IAffine : Base {
		using ImplV = At_t<ArgList, 2>;
		
		static_assert(ExistInstance_v<typename ImplV::AllVBs, ILinear>);

		using Base::Base;

		inline const ImplP operator+(const ImplV& v) const noexcept {
			return static_cast<const ImplP*>(this)->impl_affine_add(v);
		}

		inline ImplP& operator+=(const ImplV& v) noexcept {
			return static_cast<ImplP*>(this)->impl_affine_add_to_self(v);
		}

		inline const ImplP operator-(const ImplV& v) const noexcept {
			return operator+(-v);
		}

		inline ImplP& operator-=(const ImplV& v) noexcept {
			return operator+=(-v);
		}

		inline const ImplV operator-(const ImplP& v) const noexcept {
			return static_cast<const ImplP*>(this)->impl_affine_minus(v);
		}
	};
}
