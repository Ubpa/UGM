#pragma once

namespace Ubpa {
	template<typename Base, typename Impl, typename T, typename N>
	struct IInnerProduct : Base {
		using Base::Base;

		static T dot(const Impl& x, const Impl& y) noexcept {
			return Impl::ImplDot(x, y);
		}

		T dot(const Impl& y) const noexcept {
			auto& x = static_cast<const Impl&>(*this);
			return dot(x, y);
		}
	};
}