#pragma once

#include "ILinear.h"
#include "IMetric.h"

#include "../basic.h"

namespace Ubpa {
	template<typename Base, typename Impl, typename ArgList>
	struct INorm : Base {
		using IList = TemplateList<IMetric, ILinear>;

		using F = Arg_F<ArgList>;

		using Base::Base;

		inline F norm() const noexcept {
			return static_cast<const Impl*>(this)->impl_norm();
		}

		inline const Impl normalize() const noexcept {
			auto& x = static_cast<const Impl&>(*this);
			F n = norm();
			assert(n > static_cast<F>(0));
			return x / n; // ILinear
		}

		inline bool is_normalized() const noexcept {
			auto& x = static_cast<const Impl&>(*this);
			return std::abs(x.norm() - 1) < EPSILON<F>;
		}

		inline Impl& normalize_self() noexcept {
			auto& x = static_cast<Impl&>(*this);
			F n = norm();
			assert(n > static_cast<F>(0));
			return x /= n; // ILinear
		}
		
	private:
		template<typename Base, typename Impl, typename ArgList>
		friend struct IMetric;

		inline static F impl_distance(const Impl& x, const Impl& y) noexcept {
			return (x - y).norm();
		}
	};
}