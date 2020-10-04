#pragma once

#include "INorm.h"

namespace Ubpa {
	template<typename Base, typename Impl>
	struct IInnerProduct : Base {
		using Base::Base;

		using F = ImplTraits_F<Impl>;

		inline static F dot(const Impl& x, const Impl& y) noexcept {
			return Impl::impl_dot(x, y);
		}

		inline F dot(const Impl& y) const noexcept {
			const auto& x = static_cast<const Impl&>(*this);
			return dot(x, y);
		}

		inline F norm2() const noexcept {
			const auto& x = static_cast<const Impl&>(*this);
			return dot(x, x);
		}

		inline static F distance2(const Impl& x, const Impl& y) noexcept {
			return (x - y).norm2();
		}

		inline F distance2(const Impl& y) const noexcept {
			const auto& x = static_cast<const Impl&>(*this);
			return distance2(x, y);
		}

		inline static F cos_theta(const Impl& x, const Impl& y) noexcept {
			F xN = x.norm();
			F yN = y.norm();
			F xyN = xN * yN;
			assert(xyN != ZERO<F>);
			return Impl::dot(x, y) / xyN;
		}

		inline F cos_theta(const Impl& y) const noexcept {
			const auto& x = static_cast<const Impl&>(*this);
			return cos_theta(x, y);
		}

		inline static F cot_theta(const Impl& x, const Impl& y) noexcept {
			float c = cos_theta(x, y);
			F s2 = 1 - pow2(c);
			assert(s2 > ZERO<F>);
			F s = std::sqrt(s2);
			return c / s;
		}

		inline F cot_theta(const Impl& y) const noexcept {
			const auto& x = static_cast<const Impl&>(*this);
			return cot_theta(x, y);
		}

		const Impl project(const Impl& n) const noexcept {
			assert(n.is_normalized());
			return dot(n) * n;
		}

		const Impl perpendicular(const Impl& n) const noexcept {
			const auto& x = static_cast<const Impl&>(*this);
			return x - x.project(n);
		}

	private:
		template<typename Base, typename Impl>
		friend struct INorm;

		inline F impl_norm() const noexcept {
			return std::sqrt(norm2());
		}
	};

	InterfaceTraits_Register(IInnerProduct,
		INorm);
}