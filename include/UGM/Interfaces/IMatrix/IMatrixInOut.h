#pragma once

#include "../IInOut.h"
#include "IMatrix.h"
#include <UTemplate/SI.h>

namespace Ubpa {
	template<typename Base, typename Impl, typename ArgList>
	struct IMatrixInOut : SIVT_CRTP<TemplateList<IInOut, IMatrix>, Base, Impl, ArgList> {
		static constexpr size_t N = Arg_N<ArgList>;

		using SIVT_CRTP<TemplateList<IInOut, IMatrix>, Base, Impl, ArgList>::SIVT_CRTP;

		std::ostream& impl_out(std::ostream& os) const noexcept {
			auto& x = static_cast<const Impl&>(*this);

			for (size_t row = 0; row < N; row++) {
				for (size_t col = 0; col < N; col++)
					os << x[col][row] << " ";
				os << std::endl;
			}

			return os;
		}

		std::istream& impl_in(std::istream& is) noexcept {
			auto& x = static_cast<Impl&>(*this);

			for (size_t row = 0; row < N; row++) {
				for (size_t col = 0; col < N; col++)
					is >> x[col][row];
			}

			return is;
		}
	};
}