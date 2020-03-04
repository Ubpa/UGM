#pragma once

#include "IArray.h"
#include "../IInOut.h"
#include "../../basic.h"

#include <UTemplate/SI.h>

namespace Ubpa {
	template<typename Base, typename Impl, typename ArgList>
	struct IArrayInOut : SIVT_CRTP<TemplateList<IArray, IInOut>, Base, Impl, ArgList> {
		static constexpr size_t N = Arg_N<ArgList>;

		using SIVT_CRTP<TemplateList<IArray, IInOut>, Base, Impl, ArgList>::SIVT_CRTP;

		std::ostream& impl_out(std::ostream& os) const noexcept {
			auto& x = static_cast<const Impl&>(*this);

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
}