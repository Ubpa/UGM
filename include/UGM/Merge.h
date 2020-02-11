#pragma once
#ifndef UBPA_MERGE_H_
#define UBPA_MERGE_H_

namespace Ubpa {
	struct Nil {};

	template<template<typename, typename, typename, size_t> class... Interfaces>
	struct IMergeT;

	template<typename Base, typename Impl, typename T, size_t N,
		template<typename, typename, typename, size_t> class... Interfaces>
	using IMerge = typename IMergeT<Interfaces...>::template type<Base, Impl, T, N>;

	template<>
	struct IMergeT<> {
	private:
		template<typename Base, typename Impl, typename T, size_t N>
		struct INil : Base {};
	public:
		template<typename Base, typename Impl, typename T, size_t N>
		using type = INil<Base, Impl, T, N>;
	};

	template<template<typename, typename, typename, size_t> class Interface>
	struct IMergeT<Interface> {
		template<typename Base, typename Impl, typename T, size_t N>
		using type = Interface<Base, Impl, T, N>;
	};

	template<template<typename, typename, typename, size_t> class IHead,
		template<typename, typename, typename, size_t> class... ITail>
	struct IMergeT<IHead, ITail...> {
		template<typename Base, typename Impl, typename T, size_t N>
		using type = IHead<IMerge<Base, Impl, T, N, ITail...>, Impl, T, N>;
	};
}

#endif // !UBPA_MERGE_H_
