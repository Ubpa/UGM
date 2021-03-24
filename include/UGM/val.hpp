#pragma once

#include "details/traits.hpp"

#include "Interfaces/IArray/ICross.hpp"
#include "Interfaces/IArray/IEuclideanA.hpp"
#include "Interfaces/IArray/IEuclideanV.hpp"
#include "Interfaces/IArray/IArrayHadamardProduct.hpp"
#include "Interfaces/IArray/IArray1D_Util.hpp"

#include <UTemplate/SI.hpp>

namespace Ubpa {
	template<typename T, size_t N>
	struct SI_ImplTraits<val<T, N>>
		: Array1DTraits<T, N>,
		SIMDTraits<false>, // float4 not use SIMD
		IListTraits<IEuclideanV, IEuclideanA, IArrayHadamardProduct, IArray1D_Util>,
		ArrayImplNTraits<val, T>
	{
		using V = val<T, N>;
	};

	template<typename T>
	struct SI_ImplTraits<val<T, 3>>
		: Array1DTraits<T, 3>,
		IListTraits<IEuclideanV, IEuclideanA, ICross, IArrayHadamardProduct, IArray1D_Util>,
		ArrayImplNTraits<val, T>
	{
		using V = val<T, 3>;
	};

	template<typename T, size_t N>
	struct val : SI<val<T, N>> {
		using SI<val<T, N>>::SI;

		val(const euler<T>& v) noexcept { *this = v.template as<val>(); }
		val(const hvec<T, N>& v) noexcept { *this = v.template as<val>(); }
		template<size_t M>
		val(const mat<T, M>& v) noexcept { *this = v.template as<val>(); }
		val(const normal<T>& v) noexcept { *this = v.template as<val>(); }
		val(const point<T, N>& v) noexcept { *this = v.template as<val>(); }
		val(const quat<T>& v) noexcept { *this = v.template as<val>(); }
		val(const rgb<T>& v) noexcept { *this = v.template as<val>(); }
		val(const rgba<T>& v) noexcept { *this = v.template as<val>(); }
		val(const scale<T, N>& v) noexcept { *this = v.template as<val>(); }
		val(const svec<T>& v) noexcept { *this = v.template as<val>(); }
		val(const transform<T>& v) noexcept { *this = v.template as<val>(); }
		val(const vec<T, N>& v) noexcept { *this = v.template as<val>(); }

		operator euler<T> () const noexcept { return this->template cast_to<euler<T>>(); }
		operator hvec<T, N> () const noexcept { return this->template cast_to<hvec<T, N>>(); }
		template<size_t M>
		operator mat<T, M>() const noexcept;
		operator normal<T> () const noexcept { return this->template cast_to<normal<T>>(); }
		operator point<T, N> () const noexcept { return this->template cast_to<point<T, N>>(); }
		operator quat<T> () const noexcept { return this->template cast_to<quat<T>>(); }
		operator rgb<T> () const noexcept { return this->template cast_to<rgb<T>>(); }
		operator rgba<T> () const noexcept { return this->template cast_to<rgba<T>>(); }
		operator scale<T, N> () const noexcept { return this->template cast_to<scale<T, N>>(); }
		operator svec<T> () const noexcept { return this->template cast_to<svec<T>>(); }
		operator transform<T>() const noexcept;
		operator vec<T, N> () const noexcept { return this->template cast_to<vec<T, N>>(); }
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

#include "details/val.inl"
