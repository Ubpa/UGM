#pragma once

namespace Ubpa {
	template<typename Base, typename Impl, typename T, typename N>
	struct IMetric : Base {
		using Base::Base;

		static T distance(const Impl& x, const Impl& y) noexcept {
			return Impl::impl_distance(x, y);
		}

		T distance(const Impl& y) const noexcept {
			const Impl& x = static_cast<const Impl&>(*this);
			return x.distance(y);
		}
	};
}