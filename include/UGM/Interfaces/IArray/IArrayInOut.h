#pragma once

#include "IArray.h"
#include "../IInOut.h"
#include "../../basic.h"

#include <UTemplate/SI.h>

namespace Ubpa {
	template<typename Base, typename Impl>
	struct IArrayInOut : Base {
		using Base::Base;

		static constexpr size_t N = SI_ImplTraits_N<Impl>;

		std::ostream& impl_out(std::ostream& os) const noexcept {
			const auto& x = static_cast<const Impl&>(*this);

			for (size_t i = 0; i < N - 1; i++)
				os << rmv_epsilon(x[i]) << " ";
			os << rmv_epsilon(x[N - 1]);

			return os;
		}

		std::istream& impl_in(std::istream& is) noexcept {
			auto& x = static_cast<Impl&>(*this);

			for (size_t i = 0; i < N; i++)
				is >> x[i];

			return is;
		}
	};

	SI_InterfaceTraits_Register(IArrayInOut,
		IArray,
		IInOut
	);
}
