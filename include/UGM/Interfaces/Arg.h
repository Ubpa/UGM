#pragma once

#include <UTemplate/Typelist.h>
#include <UTemplate/Num.h>

namespace Ubpa {
	template<typename ArgList>
	using Arg_T = At_t<At_t<ArgList, 0>, 0>;

	template<typename ArgList>
	constexpr size_t Arg_N = At_t<At_t<ArgList, 0>, 1>::value;

	template<typename ArgList>
	using Arg_F = At_t<ArgList, 1>;

	template<typename ArgList>
	using Arg_ImplV = At_t<ArgList, 2>;

	template<typename ArgList>
	using Arg_ImplP = At_t<ArgList, 0>; // quat
}
