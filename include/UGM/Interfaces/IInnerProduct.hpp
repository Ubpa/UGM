#pragma once

#include "INorm.hpp"

namespace Ubpa {
	template<typename Base, typename Impl>
	struct IInnerProduct : Base {
		using Base::Base;

		using F = SI_ImplTraits_F<Impl>;

		static F dot(const Impl& x, const Impl& y) noexcept {
			return Impl::impl_dot(x, y);
		}

		F dot(const Impl& y) const noexcept {
			const auto& x = static_cast<const Impl&>(*this);
			return dot(x, y);
		}

		F norm2() const noexcept {
			const auto& x = static_cast<const Impl&>(*this);
			return dot(x, x);
		}

		static F distance2(const Impl& x, const Impl& y) noexcept {
			return (x - y).norm2();
		}

		F distance2(const Impl& y) const noexcept {
			const auto& x = static_cast<const Impl&>(*this);
			return distance2(x, y);
		}

		static F cos_theta(const Impl& x, const Impl& y) noexcept {
			F xN = x.norm();
			F yN = y.norm();
			F xyN = xN * yN;
			assert(xyN != Zero<F>);
			return Impl::dot(x, y) / xyN;
		}

		F cos_theta(const Impl& y) const noexcept {
			const auto& x = static_cast<const Impl&>(*this);
			return cos_theta(x, y);
		}

		static F cot_theta(const Impl& x, const Impl& y) noexcept {
			float c = cos_theta(x, y);
			F s2 = 1 - pow2(c);
			assert(s2 > Zero<F>);
			F s = std::sqrt(s2);
			return c / s;
		}

		F cot_theta(const Impl& y) const noexcept {
			const auto& x = static_cast<const Impl&>(*this);
			return cot_theta(x, y);
		}

		Impl project(const Impl& n) const noexcept {
			assert(n.is_normalized());
			return dot(n) * n;
		}

		Impl perpendicular(const Impl& n) const noexcept {
			const auto& x = static_cast<const Impl&>(*this);
			return x - x.project(n);
		}

	private:
		template<typename, typename>
		friend struct INorm;

		F impl_norm() const noexcept {
			return std::sqrt(norm2());
		}
	};
}

SI_InterfaceTraits_Register(Ubpa::IInnerProduct,
	Ubpa::INorm
);