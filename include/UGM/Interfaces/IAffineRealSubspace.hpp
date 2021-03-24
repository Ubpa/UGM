#pragma once

#include "IAffine.hpp"

namespace Ubpa {
	template<typename Base, typename Impl>
	struct IAffineRealSubspace : Base {
		using Base::Base;

		using Point = SI_ImplTraits_P<Impl>;
		using Vector = SI_ImplTraits_V<Impl>;

		static_assert(SI_Contains_v<Point, IAffine>);
		static_assert(SI_Contains_v<Vector, ILinear>);
		static_assert(SI_ImplTraits_N<Point> == SI_ImplTraits_N<Vector>);

		SI_ImplTraits_P<Impl> point;

		static Impl move(const Impl& impl, const Point& p) noexcept {
			return Impl::impl_move(impl, p);
		}

		Impl move(const Point& p) const noexcept {
			auto& impl = static_cast<const Impl&>(*this);
			return move(impl, p);
		}

		Impl& move_self(const Point& p) noexcept {
			point = p;
			return static_cast<Impl&>(*this);
		}

	private:
		template<typename, typename>
		friend struct IAffineSubspace;

		Impl impl_affine_subspace_add(const Vector& v) const noexcept {
			auto moved_p = point + v;
			return move(moved_p);
		}

		Impl& impl_affine_subspace_add_to_self(const Vector& v) noexcept {
			point += v;
			return static_cast<Impl&>(*this);
		}

		Impl impl_affine_subspace_minus(const Vector& v) const noexcept {
			auto moved_p = point - v;
			return move(moved_p);
		}

		Impl& impl_affine_subspace_minus_to_self(const Vector& v) noexcept {
			point -= v;
			return static_cast<Impl&>(*this);
		}
	};
}

SI_InterfaceTraits_Register(Ubpa::IAffineRealSubspace,
	Ubpa::IAffineSubspace
);