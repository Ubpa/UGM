#pragma once

#include "forward_decl.hpp"
#include <UTemplate/SI.hpp>

namespace Ubpa {
	template<typename T_, size_t N_, typename F_>
	struct ArrayTraits {
		using T = T_;
		static constexpr size_t N = N_;
		using F = F_;
	};

	template<typename T, size_t N>
	struct Array1DTraits : ArrayTraits<T, N, T> {};

	template<template<typename, typename>class... Interfaces>
	struct IListTraits {
		using IList = TemplateList<Interfaces...>;
	};

	template<template<typename, std::size_t>class TImpl, typename T>
	struct ArrayImplNTraits {
		template<std::size_t N>
		using ImplN = TImpl<T, N>;
	};

	template<bool support>
	struct SIMDTraits {
		static constexpr bool support_SIMD = support;
	};
}
