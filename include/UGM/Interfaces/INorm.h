#pragma once

#include "ILinear.h"
#include "IMetric.h"

#include "../basic.h"

namespace Ubpa {
	template<typename Base, typename Impl>
	struct INorm : Base {
		using F = ImplTraits_F<Impl>;

		using Base::Base;

		inline F norm() const noexcept {
			return static_cast<const Impl*>(this)->impl_norm();
		}

		inline const Impl normalize() const noexcept {
			const auto& x = static_cast<const Impl&>(*this);
			F n = norm();
			assert(n > static_cast<F>(0));
			return x / n; // ILinear
		}

		inline bool is_normalized() const noexcept {
			const auto& x = static_cast<const Impl&>(*this);
			return std::abs(x.norm() - 1) < EPSILON<F>;
		}

		inline Impl& normalize_self() noexcept {
			auto& x = static_cast<Impl&>(*this);
			F n = norm();
			assert(n > static_cast<F>(0));
			return x /= n; // ILinear
		}
		
	private:
		template<typename Base, typename Impl>
		friend struct IMetric;

		inline static F impl_distance(const Impl& x, const Impl& y) noexcept {
			return (x - y).norm();
		}
	};

	InterfaceTraits_Regist(INorm,
		IMetric, ILinear);
}
