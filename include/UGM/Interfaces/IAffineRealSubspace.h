#pragma once

#include "IAffine.h"

namespace Ubpa {
	template<typename Base, typename Impl>
	struct IAffineRealSubspace : Base {
		using Base::Base;

		using Point = ImplTraits_P<Impl>;
		using Vector = ImplTraits_V<Impl>;

		static_assert(Point::template Contains<IAffine>());
		static_assert(Vector::template Contains<ILinear>());
		static_assert(Point::N == Vector::N);

		ImplTraits_P<Impl> point;

		static const Impl move(const Impl& impl, const Point& p) noexcept {
			return Impl::impl_move(impl, p);
		}

		const Impl move(const Point& p) const noexcept {
			auto& impl = static_cast<const Impl&>(*this);
			return move(impl, p);
		}

		Impl& move_self(const Point& p) noexcept {
			point = p;
			return static_cast<Impl&>(*this);
		}

	private:
		template<typename Base, typename Impl>
		friend struct IAffineSubspace;

		const Impl impl_affine_subspace_add(const Vector& v) const noexcept {
			auto moved_p = point + v;
			return move(moved_p);
		}

		Impl& impl_affine_subspace_add_to_self(const Vector& v) noexcept {
			point += v;
			return static_cast<Impl&>(*this);
		}

		const Impl impl_affine_subspace_minus(const Vector& v) const noexcept {
			auto moved_p = point - v;
			return move(moved_p);
		}

		Impl& impl_affine_subspace_minus_to_self(const Vector& v) noexcept {
			point -= v;
			return static_cast<Impl&>(*this);
		}
	};
	
	InterfaceTraits_Register(IAffineRealSubspace,
		IAffineSubspace
	);
}
