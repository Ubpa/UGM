#pragma once

#include "Interfaces/IArray/IEuclideanV.h"

namespace Ubpa {
	template<typename T, size_t N>
	struct Vec : SIIT_CRTP<TemplateList<IEuclideanV>, Vec<T, N>, TypeList<T, Size<N>>> {
		using SIIT_CRTP<TemplateList<IEuclideanV>, Vec<T, N>, TypeList<T, Size<N>>>::SIIT_CRTP;
	};

	template<typename T>
	struct Vec<T, 3> : SIIT_CRTP<TemplateList<IEuclideanV>, Vec<T, 3>, TypeList<T, Size<3>>> {
		using SIIT_CRTP<TemplateList<IEuclideanV>, Vec<T, 3>, TypeList<T, Size<3>>>::SIIT_CRTP;

		const Vec cross(const Vec& y) const noexcept {
			const Vec& x = *this;
			return Vec{
				x[1] * y[2] - x[2] * y[1],
				x[2] * y[0] - x[0] * y[2],
				x[0] * y[1] - x[1] * y[0]
			};
		}
	};

	template<size_t N>
	using Vecf = Vec<float, N>;

	using Vecf1 = Vecf<1>;
	using Vecf2 = Vecf<2>;
	using Vecf3 = Vecf<3>;
	using Vecf4 = Vecf<4>;
}
