#pragma once

#include "ILinear.h"
#include "IMetric.h"

namespace Ubpa {
	template<typename Base, typename Impl, typename ArgList>
	struct INorm : SIVT_CRTP<TemplateList<IMetric, ILinear>, Base, Impl, ArgList> {
		using T = At_t<ArgList, 0>;

		using SIVT_CRTP<TemplateList<IMetric, ILinear>, Base, Impl, ArgList>::SIVT_CRTP;

		T norm() const noexcept {
			return static_cast<const Impl*>(this)->impl_norm();
		}

		const Impl normalize() const noexcept {
			auto& x = static_cast<const Impl&>(*this);
			T n = norm();
			assert(n > static_cast<T>(0));
			return x / n; // ILinear
		}

		Impl& normalize_self() noexcept {
			auto& x = static_cast<Impl&>(*this);
			T n = norm();
			assert(n > static_cast<T>(0));
			return x /= n; // ILinear
		}
		
	private:
		template<typename Base, typename Impl, typename ArgList>
		friend struct IMetric;

		static T impl_distance(const Impl& x, const Impl& y) noexcept {
			return (x - y).norm();
		}
	};
}