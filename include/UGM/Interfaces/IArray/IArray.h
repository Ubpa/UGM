#pragma once

#include "../../basic.h"
#include "../Arg.h"

#ifdef USE_XSIMD
#include <xsimd/xsimd.hpp>
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
	};
#endif
}
