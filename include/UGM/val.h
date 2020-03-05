#pragma once

#include "vec.h"
#include "normal.h"
#include "point.h"
#include "quat.h"
#include "euler.h"
#include "rgb.h"
#include "rgba.h"

#include "Interfaces/IArray/IArrayUtil.h"
#include "Interfaces/IArray/IArray1D_Util.h"
#include "Interfaces/IArray/IArrayHadamardProduct.h"
#include "Interfaces/IArray/IArrayAdd.h"
#include "Interfaces/IArray/IArrayScalarMul.h"

#include <UTemplate/SI.h>

namespace Ubpa {
	template<typename T, size_t N>
	struct val : SIIT_CRTP<TemplateList<IArrayAdd, IArrayScalarMul, IArrayHadamardProduct, IArray1D_Util, IArrayUtil>, val<T, N>, TypeList<TypeList<T, Size<N>>, T>> {
		using SIIT_CRTP<TemplateList<IArrayAdd, IArrayScalarMul, IArrayHadamardProduct, IArray1D_Util, IArrayUtil>, val<T, N>, TypeList<TypeList<T, Size<N>>, T>>::SIIT_CRTP;

		val(const vec<T, N>& v) noexcept {
			for (size_t i = 0; i < N; i++)
				(*this)[i] = v[i];
		}
		val(const normal<T>& v) noexcept {
			static_assert(N == 3);
			for (size_t i = 0; i < 3; i++)
				(*this)[i] = v[i];
		}
		val(const point<T, N>& v) noexcept {
			for (size_t i = 0; i < N; i++)
				(*this)[i] = v[i];
		}
		val(const rgb<T>& v) noexcept {
			static_assert(N == 3);
			for (size_t i = 0; i < 3; i++)
				(*this)[i] = v[i];
		}
		val(const rgba<T>& v) noexcept {
			static_assert(N == 4);
			for (size_t i = 0; i < 4; i++)
				(*this)[i] = v[i];
		}
		val(const quat<T>& v) noexcept {
			static_assert(N == 4);
			for (size_t i = 0; i < 4; i++)
				(*this)[i] = v[i];
		}
		val(const euler<T>& v) noexcept {
			static_assert(N == 3);
			for (size_t i = 0; i < 3; i++)
				(*this)[i] = v[i];
		}
	};

	template<size_t N>
	using valf = val<float, N>;

	using valf1 = valf<1>;
	using valf2 = valf<2>;
	using valf3 = valf<3>;
	using valf4 = valf<4>;

	template<size_t N>
	using vali = val<int, N>;

	using vali1 = vali<1>;
	using vali2 = vali<2>;
	using vali3 = vali<3>;
	using vali4 = vali<4>;

	template<size_t N>
	using valu = val<unsigned, N>;

	using valu1 = valu<1>;
	using valu2 = valu<2>;
	using valu3 = valu<3>;
	using valu4 = valu<4>;
}
