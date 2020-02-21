#pragma once

#include "../IInOut.h"
#include <UTemplate/SI.h>

namespace Ubpa {
	template<typename Base, typename Impl, typename ArgList>
	struct IQuatInOut : SIVT_CRTP<TemplateList<IInOut>, Base, Impl, ArgList> {
		using SIVT_CRTP<TemplateList<IInOut>, Base, Impl, ArgList>::SIVT_CRTP;

		std::ostream& impl_out(std::ostream& os) const noexcept {
			auto& x = static_cast<const Impl&>(*this);

			os << x.real << " " << x.imag[0] << " " << x.imag[1] << " " << x.imag[2];

			return os;
		}

		std::istream& impl_in(std::istream& is) noexcept {
			auto& x = static_cast<Impl&>(*this);

			is >> x.real;
			for (size_t i = 0; i < 3; i++)
				is >> x.imag[i];

			assert(x.is_unit());

			return is;
		}
	};
}