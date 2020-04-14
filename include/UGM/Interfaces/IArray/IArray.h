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
	struct alignas(16) IArray<Base, Impl, TypeList<TypeList<float, Size<4>>, float, Args...>> : Base, std::array<float, 4> {
	private:
		using ArgList = TypeList<TypeList<float, Size<4>>, float, Args...>;
		using Base::operator[];
	public:
		using std::array<float, 4>::operator[];

	public:
		using T = float;
		static constexpr size_t N = 4;
		using F = float;

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
#endif
}
