#pragma once

#include "../../basic.h"
#include "../Arg.h"

#ifdef USE_XSIMD
#include <xsimd/xsimd.hpp>
// link: https://lxjk.github.io/2017/09/03/Fast-4x4-Matrix-Inverse-with-SSE-SIMD-Explained.html

#define MakeShuffleMask(x,y,z,w)           (x | (y<<2) | (z<<4) | (w<<6))

// vec(0, 1, 2, 3) -> (vec[x], vec[y], vec[z], vec[w])
#define VecSwizzleMask(vec, mask)          _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(vec), mask))
#define VecSwizzle(vec, x, y, z, w)        VecSwizzleMask(vec, MakeShuffleMask(x,y,z,w))
#define VecSwizzle1(vec, x)                VecSwizzleMask(vec, MakeShuffleMask(x,x,x,x))
// special swizzle
#define VecSwizzle_0022(vec)               _mm_moveldup_ps(vec)
#define VecSwizzle_1133(vec)               _mm_movehdup_ps(vec)

// return (vec1[x], vec1[y], vec2[z], vec2[w])
#define VecShuffle(vec1, vec2, x,y,z,w)    _mm_shuffle_ps(vec1, vec2, MakeShuffleMask(x,y,z,w))
// special shuffle
#define VecShuffle_0101(vec1, vec2)        _mm_movelh_ps(vec1, vec2)
#define VecShuffle_2323(vec1, vec2)        _mm_movehl_ps(vec2, vec1)
#endif

#include <UTemplate/SI.h>

#include <array>
#include <vector>
#include <iostream>

#include <cmath>
#include <cassert>

namespace Ubpa {
	template<typename Base, typename Impl, typename ArgList>
	struct IArray : Base, std::array<Arg_T<ArgList>, Arg_N<ArgList>> {
	private:
		using Base::operator[];
	public:
		using std::array<Arg_T<ArgList>, Arg_N<ArgList>>::operator[];

	public:
		using T = Arg_T<ArgList>;
		static constexpr size_t N = Arg_N<ArgList>;
		using F = Arg_F<ArgList>;
		static_assert(N > 0);

		using Base::Base;
		using std::array<Arg_T<ArgList>, Arg_N<ArgList>>::array;

		IArray() noexcept {};

		constexpr IArray(T t) noexcept {
			for (size_t i = 0; i < N; i++)
				(*this)[i] = T{ t };
		}
		
		template<typename... U, typename = std::enable_if_t<(std::is_convertible_v<U, T>&&...)>>
		constexpr IArray(U... data) noexcept : std::array<T, N>{static_cast<T>(data)...} {
			static_assert(sizeof...(U) == N, "number of parameters is not correct");
		}
	};

#ifdef USE_XSIMD
	// alignas(16)
	template<typename Base, typename Impl, typename... Args>
	struct IArray<Base, Impl, TypeList<TypeList<float, Size<4>>, float, Args...>>
		: protected xsimd::batch<float, 4>, Base
	{
		template<typename Base, typename Impl, typename ArgList>
		friend struct IEuclideanA;
	private:
		using ArgList = TypeList<TypeList<float, Size<4>>, float, Args...>;
		using Base::operator[];

	protected:
		using xsimd::batch<float, 4>::operator+=;
		using xsimd::batch<float, 4>::operator-=;
		using xsimd::batch<float, 4>::operator*=;
		using xsimd::batch<float, 4>::operator/=;

	public:
		using xsimd::batch<float, 4>::operator[];
		using xsimd::batch<float, 4>::begin;
		using xsimd::batch<float, 4>::cbegin;
		using xsimd::batch<float, 4>::end;
		using xsimd::batch<float, 4>::cend;
		using xsimd::batch<float, 4>::operator __m128;
		using xsimd::batch<float, 4>::batch;

	public:
		using T = float;
		static constexpr size_t N = 4;
		using F = float;

		using Base::Base;

		IArray() noexcept {};

		IArray(xsimd::batch<float, 4>&& b) noexcept : xsimd::batch<float, 4>{ std::move(b) } {};

		constexpr IArray(T t) noexcept : xsimd::batch<float, 4>{t} {}

		template<typename... U, typename = std::enable_if_t<(std::is_convertible_v<U, T>&&...)>>
		constexpr IArray(U... data) noexcept : xsimd::batch<float, 4>{static_cast<T>(data)...} {
			static_assert(sizeof...(U) == N, "number of parameters is not correct");
		}

		const auto& get_batch() const noexcept { return this->get(); }

		constexpr T* data() noexcept {
			return &m_array;
		}

		constexpr  const T* data() const noexcept {
			return &m_array;
		}

	};
#endif
}
