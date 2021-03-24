#pragma once

#include "ImplTraits.hpp"

namespace Ubpa {
	template<typename Base, typename Impl>
	struct IMetric : Base {
		using F = SI_ImplTraits_F<Impl>;

		using Base::Base;

		static F distance(const Impl& x, const Impl& y) noexcept {
			return Impl::impl_distance(x, y);
		}

		F distance(const Impl& y) const noexcept {
			const auto& x = static_cast<const Impl&>(*this);
			return distance(x, y);
		}
	};
}