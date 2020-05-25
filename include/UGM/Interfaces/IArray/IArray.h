#pragma once

#include "../../basic.h"
#include "../ImplTraits.h"

#include <array>
#include <vector>
#include <iostream>

#include <cmath>
#include <cassert>

namespace Ubpa {
	template<bool SIMD, typename Base, typename Impl>
	struct IArrayImpl;

	template<typename Base, typename Impl>
	struct IArray : IArrayImpl<SupportSIMD_v<Impl>, Base, Impl> {
		using IArrayImpl<SupportSIMD_v<Impl>, Base, Impl>::IArrayImpl;
	};

	template<typename Base, typename Impl>
	struct IArrayImpl<false, Base, Impl> : Base, std::array<ImplTraits_T<Impl>, ImplTraits_N<Impl>> {
		using T = ImplTraits_T<Impl>;
		using F = ImplTraits_F<Impl>;
		static constexpr size_t N = ImplTraits_N<Impl>;

	private:
		using Base::operator[];
	public:
		using std::array<T, N>::operator[];

		static_assert(N > 0);

		using Base::Base;
		using std::array<T, N>::array;

		IArrayImpl() noexcept {};

		constexpr IArrayImpl(T t) noexcept {
			for (size_t i = 0; i < N; i++)
				(*this)[i] = T{ t };
		}

		template<typename... U, typename = std::enable_if_t<(std::is_convertible_v<U, T>&&...)>>
		constexpr IArrayImpl(U... data) noexcept : std::array<T, N>{static_cast<T>(data)...} {
			static_assert(sizeof...(U) == N, "number of parameters is not correct");
		}
	};

#ifdef UBPA_USE_XSIMD
	// alignas(16)
	template<typename Base, typename Impl>
	struct IArrayImpl<true, Base, Impl>
		: protected xsimd::batch<float, 4>, Base
	{
		template<typename Base, typename Impl>
		friend struct IEuclideanA;
	private:
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

		IArrayImpl() noexcept {};

		IArrayImpl(xsimd::batch<float, 4>&& b) noexcept : xsimd::batch<float, 4>{ std::move(b) } {};

		constexpr IArrayImpl(T t) noexcept : xsimd::batch<float, 4>{t} {}

		template<typename... U, typename = std::enable_if_t<(std::is_convertible_v<U, T>&&...)>>
		constexpr IArrayImpl(U... data) noexcept : xsimd::batch<float, 4>{static_cast<T>(data)...} {
			static_assert(sizeof...(U) == N, "number of parameters is not correct");
		}

		const auto& get_batch() const noexcept { return this->get(); }

		constexpr T* data() noexcept {
			return m_array;
		}

		constexpr  const T* data() const noexcept {
			return m_array;
		}

		constexpr size_t size() const noexcept {
			return 4;
		}
	};
#endif
}
