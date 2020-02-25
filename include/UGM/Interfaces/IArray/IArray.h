#pragma once

#include "../Arg.h"

#include <UTemplate/SI.h>

#include <array>
#include <iostream>

#include <cmath>
#include <assert.h>

namespace Ubpa {
	template<typename Base, typename Impl, typename ArgList>
	struct IArray : Base, std::array<At_t<At_t<ArgList,0>, 0>, At_t<At_t<ArgList, 0>, 1>::value> {
	private:
		using Base::operator[];
	public:
		using std::array<At_t<At_t<ArgList, 0>, 0>, At_t<At_t<ArgList, 0>, 1>::value>::operator[];

	public:
		using T = Arg_T<ArgList>;
		static constexpr size_t N = Arg_N<ArgList>;
		using F = Arg_F<ArgList>;
		static_assert(N > 0, "N > 0");
		//static_assert(std::is_arithmetic_v<F>, "std::is_arithmetic_v<F>");

		using Base::Base;

		IArray() {}
		
		template<typename... U,
			typename = std::enable_if_t<
			 Conjunction_t<Bool<std::is_convertible_v<U, T>>...>::value
			>>
		IArray(U... data) : std::array<T, N>{static_cast<T>(data)...} {
			static_assert(sizeof...(U) == N, "sizeof...(U) == N");
		}
	};
}
