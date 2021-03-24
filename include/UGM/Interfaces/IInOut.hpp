#pragma once

#include <iostream>

namespace Ubpa {
	// virtual : in, out
	template<typename Base, typename Impl>
	struct IInOut : Base {
		using Base::Base;

		friend std::ostream& operator<<(std::ostream& os, const Impl& x) {
			return x.out(os);
		}

		friend std::istream& operator>>(std::istream& is, Impl& x) {
			return x.in(is);
		}

	private:
		std::ostream& out(std::ostream& os) const {
			const auto& x = static_cast<const Impl&>(*this);
			return x.impl_out(os);
		}

		std::istream& in(std::istream& is) {
			auto& x = static_cast<Impl&>(*this);
			return x.impl_in(is);
		}
	};
}