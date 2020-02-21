#pragma once

#include "Interfaces/IArray/IEuclideanV.h"
#include "Interfaces/IArray/IArrayInOut.h"

namespace Ubpa {
	template<typename T, size_t N>
	struct vec : SIIT_CRTP<TemplateList<IArrayInOut, IEuclideanV>, vec<T, N>, TypeList<TypeList<T, Size<N>>, T>> {
		using SIIT_CRTP<TemplateList<IArrayInOut, IEuclideanV>, vec<T, N>, TypeList<TypeList<T, Size<N>>, T>>::SIIT_CRTP;
	};

	template<typename T>
	struct vec<T, 3> : SIIT_CRTP<TemplateList<IArrayInOut, IEuclideanV>, vec<T, 3>, TypeList<TypeList<T, Size<3>>, T>> {
		using SIIT_CRTP<TemplateList<IArrayInOut, IEuclideanV>, vec<T, 3>, TypeList<TypeList<T, Size<3>>, T>>::SIIT_CRTP;

		static const vec cross(const vec& x, const vec& y) noexcept {
			return vec{
				x[1] * y[2] - x[2] * y[1],
				x[2] * y[0] - x[0] * y[2],
				x[0] * y[1] - x[1] * y[0]
			};
		}

		const vec cross(const vec& y) const noexcept {
			const vec& x = *this;
			return cross(x, y);
		}

		static T sin_theta(const vec& x, const vec& y) noexcept {
			T xN = x.norm();
			T yN = y.norm();
			T xyN = xN * yN;
			assert(xyN > 0);
			return cross(x, y) / xyN;
		}

		T sin_theta(const vec& y) const noexcept {
			const vec& x = *this;
			return sin_theta(x, y);
		}
	};

	template<size_t N>
	using vecf = vec<float, N>;

	using vecf1 = vecf<1>;
	using vecf2 = vecf<2>;
	using vecf3 = vecf<3>;
	using vecf4 = vecf<4>;
}
