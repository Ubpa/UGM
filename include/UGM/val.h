#pragma once

#include "detail/traits.h"

#include "Interfaces/IArray/IArrayLinear.h"
#include "Interfaces/IArray/IArrayHadamardProduct.h"
#include "Interfaces/IArray/IArray1D_Util.h"

#include <UTemplate/SI.h>

namespace Ubpa {
	template<typename T, size_t N>
	struct ImplTraits<val<T, N>>
		: Array1DTraits<T, N>,
		SIMDTraits<false>, // float4 not use SIMD
		IListTraits<IArrayLinear, IArrayHadamardProduct, IArray1D_Util> {};

	template<typename T, size_t N>
	struct val : SI<val<T, N>> {
		using SI<val<T, N>>::SI;

		val(const euler<T>& v) noexcept { *this = v.as<val>(); }
		val(const hvec<T, N>& v) noexcept { *this = v.as<val>(); }
		template<size_t M>
		val(const mat<T, M>& v) noexcept { return *this = v.as<val>(); }
		val(const normal<T>& v) noexcept { *this = v.as<val>(); }
		val(const point<T, N>& v) noexcept { *this = v.as<val>(); }
		val(const quat<T>& v) noexcept { *this = v.as<val>(); }
		val(const rgb<T>& v) noexcept { *this = v.as<val>(); }
		val(const rgba<T>& v) noexcept { *this = v.as<val>(); }
		val(const scale<T, N>& v) noexcept { *this = v.as<val>(); }
		val(const svec<T>& v) noexcept { *this = v.as<val>(); }
		val(const transform<T>& v) noexcept { return *this = v.as<val>(); }
		val(const vec<T, N>& v) noexcept { *this = v.as<val>(); }

		operator euler<T>& () noexcept { return this->as<euler<T>>(); }
		operator hvec<T, N>& () noexcept { return this->as<hvec<T, N>>(); }
		template<size_t M>
		operator mat<T, M>& () noexcept { return this->as<mat<T, M>>(); }
		operator normal<T>& () noexcept { return this->as<normal<T>>(); }
		operator point<T, N>& () noexcept { return this->as<point<T, N>>(); }
		operator quat<T>& () noexcept { return this->as<quat<T>>(); }
		operator rgb<T>& () noexcept { return this->as<rgb<T>>(); }
		operator rgba<T>& () noexcept { return this->as<rgba<T>>(); }
		operator scale<T, N>& () noexcept { return this->as<scale<T, N>>(); }
		operator svec<T>& () noexcept { return this->as<svec<T>>(); }
		operator transform<T>& () noexcept { return this->as<transform<T>>(); }
		operator vec<T, N>& () noexcept { return this->as<vec<T, N>>(); }
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

	// maybe error in editor, but no compile error
	static_assert(sizeof(valf4) == 4 * sizeof(float));
}
