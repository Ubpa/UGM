#pragma once

#include "../IInOut.h"
#include "../../basic.h"
#include "IMatrix.h"
#include <UTemplate/SI.h>

namespace Ubpa {
	template<typename Base, typename Impl, typename ArgList>
	struct IMatrixInOut : SIVT_CRTP<TemplateList<IArrayInOut, IMatrix>, Base, Impl, ArgList> {
		static constexpr size_t N = Arg_N<ArgList>;

		using SIVT_CRTP<TemplateList<IArrayInOut, IMatrix>, Base, Impl, ArgList>::SIVT_CRTP;

		void print(std::ostream& os = std::cout) const noexcept {
			auto& x = static_cast<const Impl&>(*this);

			for (size_t row = 0; row < N; row++) {
				for (size_t col = 0; col < N-1; col++)
					os << rmv_epsilon(x[col][row]) << " ";
				os << rmv_epsilon(x[N - 1][row]) << std::endl;
			}
		}
	};
}