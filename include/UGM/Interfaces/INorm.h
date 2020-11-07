#pragma once

#include "ILinear.h"
#include "IMetric.h"

#include "../basic.h"

namespace Ubpa {
	template<typename Base, typename Impl>
	struct INorm : Base {
		using F = ImplTraits_F<Impl>;

		using Base::Base;

		F norm() const noexcept {
			return static_cast<const Impl*>(this)->impl_norm();
		}

		Impl normalize() const noexcept {
			const auto& x = static_cast<const Impl&>(*this);
			F n = norm();
			assert(n > static_cast<F>(0));
			return x / n; // ILinear
		}

		Impl safe_normalize() const noexcept {
			const auto& x = static_cast<const Impl&>(*this);
			F n = norm();
			if (n == ZERO<F>)
				return { ZERO<F> };
			else
				return x / n; // ILinear
		}

		bool is_normalized() const noexcept {
			const auto& x = static_cast<const Impl&>(*this);
			return std::abs(x.norm() - 1) < EPSILON<F>;
		}

		Impl& normalize_self() noexcept {
			auto& x = static_cast<Impl&>(*this);
			F n = norm();
			assert(n > static_cast<F>(0));
			return x /= n; // ILinear
		}

		Impl& safe_normalize_self() noexcept {
			auto& x = static_cast<Impl&>(*this);
			F n = norm();
			if (n == ZERO<F>)
				return x;
			else
				return x /= n; // ILinear
		}
		
	private:
		template<typename Base, typename Impl>
		friend struct IMetric;

		static F impl_distance(const Impl& x, const Impl& y) noexcept {
			return (x - y).norm();
		}
	};

	InterfaceTraits_Register(INorm,
		IMetric, ILinear);
}
