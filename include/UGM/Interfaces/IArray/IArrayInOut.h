#pragma once

#include "../IInOut.h"
#include <UTemplate/SI.h>

namespace Ubpa {
	template<typename Base, typename Impl, typename ArgList>
	struct IArrayInOut : SIVT_CRTP<TemplateList<IInOut>, Base, Impl, ArgList> {
		static constexpr size_t N = Arg_N<ArgList>;

		using SIVT_CRTP<TemplateList<IInOut>, Base, Impl, ArgList>::SIVT_CRTP;

		std::ostream& impl_out(std::ostream& os) const noexcept {
			auto& x = static_cast<const Impl&>(*this);

			for (auto i = static_cast<size_t>(0); i < N - 1; i++)
				os << x[i] << ", ";
			os << x[N - 1];

			return os;
		}

		std::istream& impl_in(std::istream& is) noexcept {
			auto& x = static_cast<Impl&>(*this);

			for (auto i = static_cast<size_t>(0); i < N; i++)
				is >> x[i];

			return is;
		}
	};
}