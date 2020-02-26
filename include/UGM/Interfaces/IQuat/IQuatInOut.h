#pragma once

#include "../IInOut.h"
#include <UTemplate/SI.h>

namespace Ubpa {
	template<typename Base, typename Impl, typename T>
	struct IQuatInOut : SIVT_CRTP<TemplateList<IInOut>, Base, Impl, T> {
		using SIVT_CRTP<TemplateList<IInOut>, Base, Impl, T>::SIVT_CRTP;

		std::ostream& impl_out(std::ostream& os) const noexcept {
			auto& x = static_cast<const Impl&>(*this);

			os << rmv_epsilon(x.imag()[0])
				<< " " << rmv_epsilon(x.imag()[1])
				<< " " << rmv_epsilon(x.imag()[2])
				<< " " << rmv_epsilon(x.real());

			return os;
		}

		std::istream& impl_in(std::istream& is) noexcept {
			auto& x = static_cast<Impl&>(*this);

			for (size_t i = 0; i < 3; i++)
				is >> x.imag()[i];

			is >> x.real();

			assert(x.is_unit());

			return is;
		}
	};
}