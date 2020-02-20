#pragma once

#include "ILinear.h"
#include "IMetric.h"

namespace Ubpa {
	template<typename Base, typename Impl, typename ArgList>
	struct INorm : SIVT_CRTP<TemplateList<IMetric, ILinear>, Base, Impl, ArgList> {
		using F = Arg_F<ArgList>;

		using SIVT_CRTP<TemplateList<IMetric, ILinear>, Base, Impl, ArgList>::SIVT_CRTP;

		F norm() const noexcept {
			return static_cast<const Impl*>(this)->impl_norm();
		}

		const Impl normalize() const noexcept {
			auto& x = static_cast<const Impl&>(*this);
			F n = norm();
			assert(n > static_cast<F>(0));
			return x / n; // ILinear
		}

		Impl& normalize_self() noexcept {
			auto& x = static_cast<Impl&>(*this);
			F n = norm();
			assert(n > static_cast<F>(0));
			return x /= n; // ILinear
		}
		
	private:
		template<typename Base, typename Impl, typename ArgList>
		friend struct IMetric;

		static F impl_distance(const Impl& x, const Impl& y) noexcept {
			return (x - y).norm();
		}
	};
}