#pragma once

#include "ImplTraits.h"

namespace Ubpa {
	template<typename Base, typename Impl>
	struct IMetric : Base {
		using F = ImplTraits_F<Impl>;

		using Base::Base;

		inline static F distance(const Impl& x, const Impl& y) noexcept {
			return Impl::impl_distance(x, y);
		}

		inline F distance(const Impl& y) const noexcept {
			const Impl& x = static_cast<const Impl&>(*this);
			return distance(x, y);
		}
	};
}