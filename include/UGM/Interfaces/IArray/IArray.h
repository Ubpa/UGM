#pragma once

#include "../../basic.h"
#include "../ImplTraits.h"

#include <array>
#include <vector>
#include <iostream>

#include <cmath>
#include <cassert>
#include <compare>

namespace Ubpa {
	template<typename Base, typename Impl>
	struct IArray1D;
}

namespace Ubpa::details {
	enum class ArrayMode {
		One,
		Two,
		Three,
		Four,
#ifdef UBPA_UGM_USE_SIMD
		Four_SIMD, // float4
#endif
		Basic
	};

	template<typename Impl>
	constexpr ArrayMode GetArrayMode() {
		constexpr std::size_t N = SI_ImplTraits_N<Impl>;
		if constexpr (SI_Contains_v<Impl, IArray1D>) {
			if constexpr (N == 1)
				return ArrayMode::One;
			else if constexpr (N == 2)
				return ArrayMode::Two;
			else if constexpr (N == 3)
				return ArrayMode::Three;
			else if constexpr (N == 4) {
				if constexpr (SI_ImplTraits_SupportSIMD<Impl>::value)
					return ArrayMode::Four_SIMD;
				else
					return ArrayMode::Four;
			}
			else
				return ArrayMode::Basic;
		}
		else
			return ArrayMode::Basic;
	}

	template<typename Base, typename Impl>
	struct IStdArrayBasic : Base {
		using T = SI_ImplTraits_T<Impl>;
		using F = SI_ImplTraits_F<Impl>;
		static constexpr std::size_t N = SI_ImplTraits_N<Impl>;
		static_assert(N > 0);

		using Base::Base;
		using Base::operator[];

		std::array<T, N>& to_array() noexcept { return reinterpret_cast<std::array<T, N>&>(*this); }
		const std::array<T, N>& to_array() const noexcept { return const_cast<IStdArrayBasic*>(this)->to_array(); }

		using value_type = T;
		using size_type = std::size_t;
		using difference_type = ptrdiff_t;
		using pointer = T*;
		using const_pointer = const T*;
		using reference = T&;
		using const_reference = const T&;

		using iterator = std::array<float, 4>::iterator;
		using const_iterator = std::array<float, 4>::const_iterator;

		using reverse_iterator = std::array<float, 4>::reverse_iterator;
		using const_reverse_iterator = std::array<float, 4>::const_reverse_iterator;

		void fill(T v) noexcept { to_array().fill(v); }
		void swap(Impl& other) noexcept { to_array().swap(other.to_array()); }

		iterator begin() noexcept { return to_array().begin(); }
		const_iterator begin() const noexcept { return to_array().begin(); }
		iterator end() noexcept { return to_array().end(); }
		const_iterator end() const noexcept { return to_array().end(); }

		iterator rbegin() noexcept { return to_array().rbegin(); }
		const_iterator rbegin() const noexcept { return to_array().rbegin(); }
		iterator rend() noexcept { return to_array().rend(); }
		const_iterator rend() const noexcept { return to_array().rend(); }

		const_iterator cbegin() const noexcept { return begin(); }
		const_iterator cend() const noexcept { return end(); }
		const_iterator crbegin() const noexcept { return rbegin(); }
		const_iterator crend() const noexcept { return rend(); }

		constexpr size_t size() const noexcept { return N; }
		constexpr size_t max_size() const noexcept { return N; }
		constexpr bool empty() const noexcept { return false; }

		reference at(size_t i) { return to_array().at(i); }
		const_reference at(size_t i) const { return to_array().at(i); }
		reference operator[](size_t i) noexcept { return to_array().at(i); }
		const_reference operator[](size_t i) const noexcept { return to_array().at(i); }
		reference front() noexcept { return to_array().front(); }
		const_reference front() const noexcept { return to_array().front(); }
		reference back() noexcept { return to_array().back(); }
		const_reference back() const noexcept { return to_array().back(); }

		T* data() noexcept { return reinterpret_cast<T*>(this); }
		const T* data() const noexcept { return const_cast<IStdArrayBasic*>(this)->data(); }
	};

	template<typename Base, typename Impl>
	struct alignas(alignof(SI_ImplTraits_T<Impl>)) IStdArray : IStdArrayBasic<Base, Impl> {
		using T = SI_ImplTraits_T<Impl>;
		using F = SI_ImplTraits_F<Impl>;
		static constexpr std::size_t N = SI_ImplTraits_N<Impl>;

		using IStdArrayBasic<Base, Impl>::IStdArrayBasic;
		using IStdArrayBasic<Base, Impl>::operator==;
		using IStdArrayBasic<Base, Impl>::operator!=;
		using IStdArrayBasic<Base, Impl>::operator>;
		using IStdArrayBasic<Base, Impl>::operator>=;
		using IStdArrayBasic<Base, Impl>::operator<;
		using IStdArrayBasic<Base, Impl>::operator<=;

		friend bool operator==(const Impl& lhs, const Impl& rhs) { return lhs.to_array() == rhs.to_array(); }
		friend bool operator!=(const Impl& lhs, const Impl& rhs) { return lhs.to_array() != rhs.to_array(); }
		friend bool operator< (const Impl& lhs, const Impl& rhs) { return lhs.to_array() <  rhs.to_array(); }
		friend bool operator> (const Impl& lhs, const Impl& rhs) { return lhs.to_array() >  rhs.to_array(); }
		friend bool operator<=(const Impl& lhs, const Impl& rhs) { return lhs.to_array() <= rhs.to_array(); }
		friend bool operator>=(const Impl& lhs, const Impl& rhs) { return lhs.to_array() >= rhs.to_array(); }
	};

	template<typename Base, typename Impl>
	struct IArrayCommon : IStdArray<Base, Impl> {
		using T = SI_ImplTraits_T<Impl>;
		using F = SI_ImplTraits_F<Impl>;
		static constexpr std::size_t N = SI_ImplTraits_N<Impl>;

	private:
		template<size_t... Ns>
		IArrayCommon(const T* arr, std::index_sequence<Ns...>) noexcept {
			(((*this)[Ns] = arr[Ns]), ...);
		}
		template<size_t... Ns>
		IArrayCommon(const T& t, std::index_sequence<Ns...>) noexcept {
			(((*this)[Ns] = t), ...);
		};
		template<size_t... Ns>
		IArrayCommon(const Impl& arr, std::index_sequence<Ns...>) noexcept {
			(((*this)[Ns] = arr[Ns]), ...);
		};

	public:
		using IStdArray<Base, Impl>::IStdArray;

		IArrayCommon() noexcept {}

		constexpr IArrayCommon(const T* arr) noexcept : IArrayCommon{ arr, std::make_index_sequence<N>{} } {}

		constexpr IArrayCommon(const Impl& arr) noexcept : IArrayCommon{ arr, std::make_index_sequence<N>{} } {}

		constexpr IArrayCommon(const T& t) noexcept : IArrayCommon{ t, std::make_index_sequence<N>{} } {}

		static constexpr Impl zero() noexcept {
			return Impl{ T{static_cast<F>(0)} };
		}

		template<size_t i>
		T get() const noexcept {
			static_assert(i < N);
			return (*this)[i];
		}

		template<size_t i>
		void set(T v) const noexcept {
			static_assert(i < N);
			(*this)[i] = v;
		}

		template<size_t i>
		Impl replicate() const noexcept {
			static_assert(i < N);
			Impl rst;
			rst.fill(get<i>());
			return rst;
		}

		bool lex_lt(const Impl& y) const noexcept {
			const auto& x = static_cast<const Impl&>(*this);
			return x < y;
		}
		bool lex_le(const Impl& y) const noexcept {
			const auto& x = static_cast<const Impl&>(*this);
			return x <= y;
		}
		bool lex_gt(const Impl& y) const noexcept {
			const auto& x = static_cast<const Impl&>(*this);
			return x > y;
		}
		bool lex_ge(const Impl& y) const noexcept {
			const auto& x = static_cast<const Impl&>(*this);
			return x >= y;
		}
		static bool all_lt(const Impl& x, const Impl& y) noexcept {
			for (size_t i = 0; i < N; i++) {
				if (x[i] >= y[i])
					return false;
			}
			return true;
		}
		static bool all_le(const Impl& x, const Impl& y) noexcept {
			for (size_t i = 0; i < N; i++) {
				if (x[i] > y[i])
					return false;
			}
			return true;
		}
		static bool all_gt(const Impl& x, const Impl& y) noexcept {
			for (size_t i = 0; i < N; i++) {
				if (x[i] <= y[i])
					return false;
			}
			return true;
		}
		static bool all_ge(const Impl& x, const Impl& y) noexcept {
			for (size_t i = 0; i < N; i++) {
				if (x[i] < y[i])
					return false;
			}
			return true;
		}
		bool all_lt(const Impl& y) const noexcept {
			const auto& x = static_cast<const Impl&>(*this);
			return all_lt(x, y);
		}
		bool all_le(const Impl& y) const noexcept {
			const auto& x = static_cast<const Impl&>(*this);
			return all_le(x, y);
		}
		bool all_gt(const Impl& y) const noexcept {
			const auto& x = static_cast<const Impl&>(*this);
			return all_gt(x, y);
		}
		bool all_ge(const Impl& y) const noexcept {
			const auto& x = static_cast<const Impl&>(*this);
			return all_ge(x, y);
		}
	};

	template<std::size_t... Ns>
	constexpr bool is_duplicate(std::index_sequence<Ns...>) noexcept {
		std::size_t indices[]{ Ns... };
		for (std::size_t i{ 0 }; i < std::extent_v<decltype(indices)>; i++) {
			for (std::size_t j{ 0 }; j < i; j++) {
				if (indices[j] == indices[i])
					return true;
			}
		}
		return false;
	}

	template<typename Impl, std::size_t... Ns>
	struct SwizzleImplBase {
		auto& operator [](std::size_t i) noexcept {
			constexpr std::size_t indices[]{ Ns... };
			return get(indices[i]);
		}

		const auto& operator [](std::size_t i) const noexcept {
			return const_cast<SwizzleImplBase&>(*this)[i];
		}

	protected:
		template<std::size_t N>
		auto get() const noexcept {
			return reinterpret_cast<const Impl&>(*this).template get<N>();
		}

		auto& get(std::size_t i) noexcept {
			return reinterpret_cast<Impl&>(*this)[i];
		}

		const auto& get(std::size_t i) const noexcept {
			return const_cast<SwizzleImplBase*>(this)->get(i);
		}
	};

	template<bool IsDuplicate, bool SupportImplN, typename Impl, std::size_t... Ns>
	struct SwizzleImpl : SwizzleImplBase<Impl, Ns...> {};

	template<typename Impl, std::size_t... Ns>
	struct SwizzleImpl<true, true, Impl, Ns...> : SwizzleImplBase<Impl, Ns...> {
		using ImplN = SI_ImplTraits_ImplN<Impl, sizeof...(Ns)>;
		auto to_impl() const {
			return ImplN{ this->template get<Ns>()... };
		}
		operator ImplN() const {
			return to_impl();
		}
	};

	template<typename Impl, std::size_t... Ns>
	struct SwizzleImpl<false, true, Impl, Ns...> : SwizzleImpl<true, true, Impl, Ns...> {
		using ImplN = SI_ImplTraits_ImplN<Impl, sizeof...(Ns)>;
		Impl& operator=(const ImplN& rhs) noexcept {
			constexpr auto assign_impl = []<std::size_t... Ms>(Impl & lhs, const ImplN & rhs, std::index_sequence<Ms...>) -> Impl& {
				(lhs.template set<Ns>(rhs.template get<Ms>()), ...);
				return lhs;
			};
			return assign_impl(reinterpret_cast<Impl&>(*this), rhs, std::make_index_sequence<sizeof...(Ns)>{});
		}
	};

	template<typename Impl, std::size_t... Ns>
	struct Swizzle : SwizzleImpl<is_duplicate(std::index_sequence<Ns...>{}), Ubpa::SI_ImplTraits_SupportImplN<Impl, sizeof...(Ns)>, Impl, Ns...> {};
}

#define UBPA_ARRAY_SWIZZLE2_2(EI0, EI1, E0, E1) \
Ubpa::details::Swizzle<Impl, EI0, EI0> E0##E0;  \
Ubpa::details::Swizzle<Impl, EI0, EI1> E0##E1;  \
Ubpa::details::Swizzle<Impl, EI1, EI0> E1##E0;  \
Ubpa::details::Swizzle<Impl, EI1, EI1> E1##E1

#define UBPA_ARRAY_SWIZZLE3_3(EI0, EI1, EI2, E0, E1, E2) \
Ubpa::details::Swizzle<Impl, EI0, EI0, EI0> E0##E0##E0;  \
Ubpa::details::Swizzle<Impl, EI0, EI0, EI1> E0##E0##E1;  \
Ubpa::details::Swizzle<Impl, EI0, EI0, EI2> E0##E0##E2;  \
Ubpa::details::Swizzle<Impl, EI0, EI1, EI0> E0##E1##E0;  \
Ubpa::details::Swizzle<Impl, EI0, EI1, EI1> E0##E1##E1;  \
Ubpa::details::Swizzle<Impl, EI0, EI1, EI2> E0##E1##E2;  \
Ubpa::details::Swizzle<Impl, EI0, EI2, EI0> E0##E2##E0;  \
Ubpa::details::Swizzle<Impl, EI0, EI2, EI1> E0##E2##E1;  \
Ubpa::details::Swizzle<Impl, EI0, EI2, EI2> E0##E2##E2;  \
Ubpa::details::Swizzle<Impl, EI1, EI0, EI0> E1##E0##E0;  \
Ubpa::details::Swizzle<Impl, EI1, EI0, EI1> E1##E0##E1;  \
Ubpa::details::Swizzle<Impl, EI1, EI0, EI2> E1##E0##E2;  \
Ubpa::details::Swizzle<Impl, EI1, EI1, EI0> E1##E1##E0;  \
Ubpa::details::Swizzle<Impl, EI1, EI1, EI1> E1##E1##E1;  \
Ubpa::details::Swizzle<Impl, EI1, EI1, EI2> E1##E1##E2;  \
Ubpa::details::Swizzle<Impl, EI1, EI2, EI0> E1##E2##E0;  \
Ubpa::details::Swizzle<Impl, EI1, EI2, EI1> E1##E2##E1;  \
Ubpa::details::Swizzle<Impl, EI1, EI2, EI2> E1##E2##E2;  \
Ubpa::details::Swizzle<Impl, EI2, EI0, EI0> E2##E0##E0;  \
Ubpa::details::Swizzle<Impl, EI2, EI0, EI1> E2##E0##E1;  \
Ubpa::details::Swizzle<Impl, EI2, EI0, EI2> E2##E0##E2;  \
Ubpa::details::Swizzle<Impl, EI2, EI1, EI0> E2##E1##E0;  \
Ubpa::details::Swizzle<Impl, EI2, EI1, EI1> E2##E1##E1;  \
Ubpa::details::Swizzle<Impl, EI2, EI1, EI2> E2##E1##E2;  \
Ubpa::details::Swizzle<Impl, EI2, EI2, EI0> E2##E2##E0;  \
Ubpa::details::Swizzle<Impl, EI2, EI2, EI1> E2##E2##E1;  \
Ubpa::details::Swizzle<Impl, EI2, EI2, EI2> E2##E2##E2

#define UBPA_ARRAY_SWIZZLE4_4(EI0, EI1, EI2, EI3, E0, E1, E2, E3) \
Ubpa::details::Swizzle<Impl, EI0, EI0, EI0, EI0> E0##E0##E0##E0;  \
Ubpa::details::Swizzle<Impl, EI0, EI0, EI0, EI1> E0##E0##E0##E1;  \
Ubpa::details::Swizzle<Impl, EI0, EI0, EI0, EI2> E0##E0##E0##E2;  \
Ubpa::details::Swizzle<Impl, EI0, EI0, EI0, EI3> E0##E0##E0##E3;  \
Ubpa::details::Swizzle<Impl, EI0, EI0, EI1, EI0> E0##E0##E1##E0;  \
Ubpa::details::Swizzle<Impl, EI0, EI0, EI1, EI1> E0##E0##E1##E1;  \
Ubpa::details::Swizzle<Impl, EI0, EI0, EI1, EI2> E0##E0##E1##E2;  \
Ubpa::details::Swizzle<Impl, EI0, EI0, EI1, EI3> E0##E0##E1##E3;  \
Ubpa::details::Swizzle<Impl, EI0, EI0, EI2, EI0> E0##E0##E2##E0;  \
Ubpa::details::Swizzle<Impl, EI0, EI0, EI2, EI1> E0##E0##E2##E1;  \
Ubpa::details::Swizzle<Impl, EI0, EI0, EI2, EI2> E0##E0##E2##E2;  \
Ubpa::details::Swizzle<Impl, EI0, EI0, EI2, EI3> E0##E0##E2##E3;  \
Ubpa::details::Swizzle<Impl, EI0, EI0, EI3, EI0> E0##E0##E3##E0;  \
Ubpa::details::Swizzle<Impl, EI0, EI0, EI3, EI1> E0##E0##E3##E1;  \
Ubpa::details::Swizzle<Impl, EI0, EI0, EI3, EI2> E0##E0##E3##E2;  \
Ubpa::details::Swizzle<Impl, EI0, EI0, EI3, EI3> E0##E0##E3##E3;  \
Ubpa::details::Swizzle<Impl, EI0, EI1, EI0, EI0> E0##E1##E0##E0;  \
Ubpa::details::Swizzle<Impl, EI0, EI1, EI0, EI1> E0##E1##E0##E1;  \
Ubpa::details::Swizzle<Impl, EI0, EI1, EI0, EI2> E0##E1##E0##E2;  \
Ubpa::details::Swizzle<Impl, EI0, EI1, EI0, EI3> E0##E1##E0##E3;  \
Ubpa::details::Swizzle<Impl, EI0, EI1, EI1, EI0> E0##E1##E1##E0;  \
Ubpa::details::Swizzle<Impl, EI0, EI1, EI1, EI1> E0##E1##E1##E1;  \
Ubpa::details::Swizzle<Impl, EI0, EI1, EI1, EI2> E0##E1##E1##E2;  \
Ubpa::details::Swizzle<Impl, EI0, EI1, EI1, EI3> E0##E1##E1##E3;  \
Ubpa::details::Swizzle<Impl, EI0, EI1, EI2, EI0> E0##E1##E2##E0;  \
Ubpa::details::Swizzle<Impl, EI0, EI1, EI2, EI1> E0##E1##E2##E1;  \
Ubpa::details::Swizzle<Impl, EI0, EI1, EI2, EI2> E0##E1##E2##E2;  \
Ubpa::details::Swizzle<Impl, EI0, EI1, EI2, EI3> E0##E1##E2##E3;  \
Ubpa::details::Swizzle<Impl, EI0, EI1, EI3, EI0> E0##E1##E3##E0;  \
Ubpa::details::Swizzle<Impl, EI0, EI1, EI3, EI1> E0##E1##E3##E1;  \
Ubpa::details::Swizzle<Impl, EI0, EI1, EI3, EI2> E0##E1##E3##E2;  \
Ubpa::details::Swizzle<Impl, EI0, EI1, EI3, EI3> E0##E1##E3##E3;  \
Ubpa::details::Swizzle<Impl, EI0, EI2, EI0, EI0> E0##E2##E0##E0;  \
Ubpa::details::Swizzle<Impl, EI0, EI2, EI0, EI1> E0##E2##E0##E1;  \
Ubpa::details::Swizzle<Impl, EI0, EI2, EI0, EI2> E0##E2##E0##E2;  \
Ubpa::details::Swizzle<Impl, EI0, EI2, EI0, EI3> E0##E2##E0##E3;  \
Ubpa::details::Swizzle<Impl, EI0, EI2, EI1, EI0> E0##E2##E1##E0;  \
Ubpa::details::Swizzle<Impl, EI0, EI2, EI1, EI1> E0##E2##E1##E1;  \
Ubpa::details::Swizzle<Impl, EI0, EI2, EI1, EI2> E0##E2##E1##E2;  \
Ubpa::details::Swizzle<Impl, EI0, EI2, EI1, EI3> E0##E2##E1##E3;  \
Ubpa::details::Swizzle<Impl, EI0, EI2, EI2, EI0> E0##E2##E2##E0;  \
Ubpa::details::Swizzle<Impl, EI0, EI2, EI2, EI1> E0##E2##E2##E1;  \
Ubpa::details::Swizzle<Impl, EI0, EI2, EI2, EI2> E0##E2##E2##E2;  \
Ubpa::details::Swizzle<Impl, EI0, EI2, EI2, EI3> E0##E2##E2##E3;  \
Ubpa::details::Swizzle<Impl, EI0, EI2, EI3, EI0> E0##E2##E3##E0;  \
Ubpa::details::Swizzle<Impl, EI0, EI2, EI3, EI1> E0##E2##E3##E1;  \
Ubpa::details::Swizzle<Impl, EI0, EI2, EI3, EI2> E0##E2##E3##E2;  \
Ubpa::details::Swizzle<Impl, EI0, EI2, EI3, EI3> E0##E2##E3##E3;  \
Ubpa::details::Swizzle<Impl, EI0, EI3, EI0, EI0> E0##E3##E0##E0;  \
Ubpa::details::Swizzle<Impl, EI0, EI3, EI0, EI1> E0##E3##E0##E1;  \
Ubpa::details::Swizzle<Impl, EI0, EI3, EI0, EI2> E0##E3##E0##E2;  \
Ubpa::details::Swizzle<Impl, EI0, EI3, EI0, EI3> E0##E3##E0##E3;  \
Ubpa::details::Swizzle<Impl, EI0, EI3, EI1, EI0> E0##E3##E1##E0;  \
Ubpa::details::Swizzle<Impl, EI0, EI3, EI1, EI1> E0##E3##E1##E1;  \
Ubpa::details::Swizzle<Impl, EI0, EI3, EI1, EI2> E0##E3##E1##E2;  \
Ubpa::details::Swizzle<Impl, EI0, EI3, EI1, EI3> E0##E3##E1##E3;  \
Ubpa::details::Swizzle<Impl, EI0, EI3, EI2, EI0> E0##E3##E2##E0;  \
Ubpa::details::Swizzle<Impl, EI0, EI3, EI2, EI1> E0##E3##E2##E1;  \
Ubpa::details::Swizzle<Impl, EI0, EI3, EI2, EI2> E0##E3##E2##E2;  \
Ubpa::details::Swizzle<Impl, EI0, EI3, EI2, EI3> E0##E3##E2##E3;  \
Ubpa::details::Swizzle<Impl, EI0, EI3, EI3, EI0> E0##E3##E3##E0;  \
Ubpa::details::Swizzle<Impl, EI0, EI3, EI3, EI1> E0##E3##E3##E1;  \
Ubpa::details::Swizzle<Impl, EI0, EI3, EI3, EI2> E0##E3##E3##E2;  \
Ubpa::details::Swizzle<Impl, EI0, EI3, EI3, EI3> E0##E3##E3##E3;  \
Ubpa::details::Swizzle<Impl, EI1, EI0, EI0, EI0> E1##E0##E0##E0;  \
Ubpa::details::Swizzle<Impl, EI1, EI0, EI0, EI1> E1##E0##E0##E1;  \
Ubpa::details::Swizzle<Impl, EI1, EI0, EI0, EI2> E1##E0##E0##E2;  \
Ubpa::details::Swizzle<Impl, EI1, EI0, EI0, EI3> E1##E0##E0##E3;  \
Ubpa::details::Swizzle<Impl, EI1, EI0, EI1, EI0> E1##E0##E1##E0;  \
Ubpa::details::Swizzle<Impl, EI1, EI0, EI1, EI1> E1##E0##E1##E1;  \
Ubpa::details::Swizzle<Impl, EI1, EI0, EI1, EI2> E1##E0##E1##E2;  \
Ubpa::details::Swizzle<Impl, EI1, EI0, EI1, EI3> E1##E0##E1##E3;  \
Ubpa::details::Swizzle<Impl, EI1, EI0, EI2, EI0> E1##E0##E2##E0;  \
Ubpa::details::Swizzle<Impl, EI1, EI0, EI2, EI1> E1##E0##E2##E1;  \
Ubpa::details::Swizzle<Impl, EI1, EI0, EI2, EI2> E1##E0##E2##E2;  \
Ubpa::details::Swizzle<Impl, EI1, EI0, EI2, EI3> E1##E0##E2##E3;  \
Ubpa::details::Swizzle<Impl, EI1, EI0, EI3, EI0> E1##E0##E3##E0;  \
Ubpa::details::Swizzle<Impl, EI1, EI0, EI3, EI1> E1##E0##E3##E1;  \
Ubpa::details::Swizzle<Impl, EI1, EI0, EI3, EI2> E1##E0##E3##E2;  \
Ubpa::details::Swizzle<Impl, EI1, EI0, EI3, EI3> E1##E0##E3##E3;  \
Ubpa::details::Swizzle<Impl, EI1, EI1, EI0, EI0> E1##E1##E0##E0;  \
Ubpa::details::Swizzle<Impl, EI1, EI1, EI0, EI1> E1##E1##E0##E1;  \
Ubpa::details::Swizzle<Impl, EI1, EI1, EI0, EI2> E1##E1##E0##E2;  \
Ubpa::details::Swizzle<Impl, EI1, EI1, EI0, EI3> E1##E1##E0##E3;  \
Ubpa::details::Swizzle<Impl, EI1, EI1, EI1, EI0> E1##E1##E1##E0;  \
Ubpa::details::Swizzle<Impl, EI1, EI1, EI1, EI1> E1##E1##E1##E1;  \
Ubpa::details::Swizzle<Impl, EI1, EI1, EI1, EI2> E1##E1##E1##E2;  \
Ubpa::details::Swizzle<Impl, EI1, EI1, EI1, EI3> E1##E1##E1##E3;  \
Ubpa::details::Swizzle<Impl, EI1, EI1, EI2, EI0> E1##E1##E2##E0;  \
Ubpa::details::Swizzle<Impl, EI1, EI1, EI2, EI1> E1##E1##E2##E1;  \
Ubpa::details::Swizzle<Impl, EI1, EI1, EI2, EI2> E1##E1##E2##E2;  \
Ubpa::details::Swizzle<Impl, EI1, EI1, EI2, EI3> E1##E1##E2##E3;  \
Ubpa::details::Swizzle<Impl, EI1, EI1, EI3, EI0> E1##E1##E3##E0;  \
Ubpa::details::Swizzle<Impl, EI1, EI1, EI3, EI1> E1##E1##E3##E1;  \
Ubpa::details::Swizzle<Impl, EI1, EI1, EI3, EI2> E1##E1##E3##E2;  \
Ubpa::details::Swizzle<Impl, EI1, EI1, EI3, EI3> E1##E1##E3##E3;  \
Ubpa::details::Swizzle<Impl, EI1, EI2, EI0, EI0> E1##E2##E0##E0;  \
Ubpa::details::Swizzle<Impl, EI1, EI2, EI0, EI1> E1##E2##E0##E1;  \
Ubpa::details::Swizzle<Impl, EI1, EI2, EI0, EI2> E1##E2##E0##E2;  \
Ubpa::details::Swizzle<Impl, EI1, EI2, EI0, EI3> E1##E2##E0##E3;  \
Ubpa::details::Swizzle<Impl, EI1, EI2, EI1, EI0> E1##E2##E1##E0;  \
Ubpa::details::Swizzle<Impl, EI1, EI2, EI1, EI1> E1##E2##E1##E1;  \
Ubpa::details::Swizzle<Impl, EI1, EI2, EI1, EI2> E1##E2##E1##E2;  \
Ubpa::details::Swizzle<Impl, EI1, EI2, EI1, EI3> E1##E2##E1##E3;  \
Ubpa::details::Swizzle<Impl, EI1, EI2, EI2, EI0> E1##E2##E2##E0;  \
Ubpa::details::Swizzle<Impl, EI1, EI2, EI2, EI1> E1##E2##E2##E1;  \
Ubpa::details::Swizzle<Impl, EI1, EI2, EI2, EI2> E1##E2##E2##E2;  \
Ubpa::details::Swizzle<Impl, EI1, EI2, EI2, EI3> E1##E2##E2##E3;  \
Ubpa::details::Swizzle<Impl, EI1, EI2, EI3, EI0> E1##E2##E3##E0;  \
Ubpa::details::Swizzle<Impl, EI1, EI2, EI3, EI1> E1##E2##E3##E1;  \
Ubpa::details::Swizzle<Impl, EI1, EI2, EI3, EI2> E1##E2##E3##E2;  \
Ubpa::details::Swizzle<Impl, EI1, EI2, EI3, EI3> E1##E2##E3##E3;  \
Ubpa::details::Swizzle<Impl, EI1, EI3, EI0, EI0> E1##E3##E0##E0;  \
Ubpa::details::Swizzle<Impl, EI1, EI3, EI0, EI1> E1##E3##E0##E1;  \
Ubpa::details::Swizzle<Impl, EI1, EI3, EI0, EI2> E1##E3##E0##E2;  \
Ubpa::details::Swizzle<Impl, EI1, EI3, EI0, EI3> E1##E3##E0##E3;  \
Ubpa::details::Swizzle<Impl, EI1, EI3, EI1, EI0> E1##E3##E1##E0;  \
Ubpa::details::Swizzle<Impl, EI1, EI3, EI1, EI1> E1##E3##E1##E1;  \
Ubpa::details::Swizzle<Impl, EI1, EI3, EI1, EI2> E1##E3##E1##E2;  \
Ubpa::details::Swizzle<Impl, EI1, EI3, EI1, EI3> E1##E3##E1##E3;  \
Ubpa::details::Swizzle<Impl, EI1, EI3, EI2, EI0> E1##E3##E2##E0;  \
Ubpa::details::Swizzle<Impl, EI1, EI3, EI2, EI1> E1##E3##E2##E1;  \
Ubpa::details::Swizzle<Impl, EI1, EI3, EI2, EI2> E1##E3##E2##E2;  \
Ubpa::details::Swizzle<Impl, EI1, EI3, EI2, EI3> E1##E3##E2##E3;  \
Ubpa::details::Swizzle<Impl, EI1, EI3, EI3, EI0> E1##E3##E3##E0;  \
Ubpa::details::Swizzle<Impl, EI1, EI3, EI3, EI1> E1##E3##E3##E1;  \
Ubpa::details::Swizzle<Impl, EI1, EI3, EI3, EI2> E1##E3##E3##E2;  \
Ubpa::details::Swizzle<Impl, EI1, EI3, EI3, EI3> E1##E3##E3##E3;  \
Ubpa::details::Swizzle<Impl, EI2, EI0, EI0, EI0> E2##E0##E0##E0;  \
Ubpa::details::Swizzle<Impl, EI2, EI0, EI0, EI1> E2##E0##E0##E1;  \
Ubpa::details::Swizzle<Impl, EI2, EI0, EI0, EI2> E2##E0##E0##E2;  \
Ubpa::details::Swizzle<Impl, EI2, EI0, EI0, EI3> E2##E0##E0##E3;  \
Ubpa::details::Swizzle<Impl, EI2, EI0, EI1, EI0> E2##E0##E1##E0;  \
Ubpa::details::Swizzle<Impl, EI2, EI0, EI1, EI1> E2##E0##E1##E1;  \
Ubpa::details::Swizzle<Impl, EI2, EI0, EI1, EI2> E2##E0##E1##E2;  \
Ubpa::details::Swizzle<Impl, EI2, EI0, EI1, EI3> E2##E0##E1##E3;  \
Ubpa::details::Swizzle<Impl, EI2, EI0, EI2, EI0> E2##E0##E2##E0;  \
Ubpa::details::Swizzle<Impl, EI2, EI0, EI2, EI1> E2##E0##E2##E1;  \
Ubpa::details::Swizzle<Impl, EI2, EI0, EI2, EI2> E2##E0##E2##E2;  \
Ubpa::details::Swizzle<Impl, EI2, EI0, EI2, EI3> E2##E0##E2##E3;  \
Ubpa::details::Swizzle<Impl, EI2, EI0, EI3, EI0> E2##E0##E3##E0;  \
Ubpa::details::Swizzle<Impl, EI2, EI0, EI3, EI1> E2##E0##E3##E1;  \
Ubpa::details::Swizzle<Impl, EI2, EI0, EI3, EI2> E2##E0##E3##E2;  \
Ubpa::details::Swizzle<Impl, EI2, EI0, EI3, EI3> E2##E0##E3##E3;  \
Ubpa::details::Swizzle<Impl, EI2, EI1, EI0, EI0> E2##E1##E0##E0;  \
Ubpa::details::Swizzle<Impl, EI2, EI1, EI0, EI1> E2##E1##E0##E1;  \
Ubpa::details::Swizzle<Impl, EI2, EI1, EI0, EI2> E2##E1##E0##E2;  \
Ubpa::details::Swizzle<Impl, EI2, EI1, EI0, EI3> E2##E1##E0##E3;  \
Ubpa::details::Swizzle<Impl, EI2, EI1, EI1, EI0> E2##E1##E1##E0;  \
Ubpa::details::Swizzle<Impl, EI2, EI1, EI1, EI1> E2##E1##E1##E1;  \
Ubpa::details::Swizzle<Impl, EI2, EI1, EI1, EI2> E2##E1##E1##E2;  \
Ubpa::details::Swizzle<Impl, EI2, EI1, EI1, EI3> E2##E1##E1##E3;  \
Ubpa::details::Swizzle<Impl, EI2, EI1, EI2, EI0> E2##E1##E2##E0;  \
Ubpa::details::Swizzle<Impl, EI2, EI1, EI2, EI1> E2##E1##E2##E1;  \
Ubpa::details::Swizzle<Impl, EI2, EI1, EI2, EI2> E2##E1##E2##E2;  \
Ubpa::details::Swizzle<Impl, EI2, EI1, EI2, EI3> E2##E1##E2##E3;  \
Ubpa::details::Swizzle<Impl, EI2, EI1, EI3, EI0> E2##E1##E3##E0;  \
Ubpa::details::Swizzle<Impl, EI2, EI1, EI3, EI1> E2##E1##E3##E1;  \
Ubpa::details::Swizzle<Impl, EI2, EI1, EI3, EI2> E2##E1##E3##E2;  \
Ubpa::details::Swizzle<Impl, EI2, EI1, EI3, EI3> E2##E1##E3##E3;  \
Ubpa::details::Swizzle<Impl, EI2, EI2, EI0, EI0> E2##E2##E0##E0;  \
Ubpa::details::Swizzle<Impl, EI2, EI2, EI0, EI1> E2##E2##E0##E1;  \
Ubpa::details::Swizzle<Impl, EI2, EI2, EI0, EI2> E2##E2##E0##E2;  \
Ubpa::details::Swizzle<Impl, EI2, EI2, EI0, EI3> E2##E2##E0##E3;  \
Ubpa::details::Swizzle<Impl, EI2, EI2, EI1, EI0> E2##E2##E1##E0;  \
Ubpa::details::Swizzle<Impl, EI2, EI2, EI1, EI1> E2##E2##E1##E1;  \
Ubpa::details::Swizzle<Impl, EI2, EI2, EI1, EI2> E2##E2##E1##E2;  \
Ubpa::details::Swizzle<Impl, EI2, EI2, EI1, EI3> E2##E2##E1##E3;  \
Ubpa::details::Swizzle<Impl, EI2, EI2, EI2, EI0> E2##E2##E2##E0;  \
Ubpa::details::Swizzle<Impl, EI2, EI2, EI2, EI1> E2##E2##E2##E1;  \
Ubpa::details::Swizzle<Impl, EI2, EI2, EI2, EI2> E2##E2##E2##E2;  \
Ubpa::details::Swizzle<Impl, EI2, EI2, EI2, EI3> E2##E2##E2##E3;  \
Ubpa::details::Swizzle<Impl, EI2, EI2, EI3, EI0> E2##E2##E3##E0;  \
Ubpa::details::Swizzle<Impl, EI2, EI2, EI3, EI1> E2##E2##E3##E1;  \
Ubpa::details::Swizzle<Impl, EI2, EI2, EI3, EI2> E2##E2##E3##E2;  \
Ubpa::details::Swizzle<Impl, EI2, EI2, EI3, EI3> E2##E2##E3##E3;  \
Ubpa::details::Swizzle<Impl, EI2, EI3, EI0, EI0> E2##E3##E0##E0;  \
Ubpa::details::Swizzle<Impl, EI2, EI3, EI0, EI1> E2##E3##E0##E1;  \
Ubpa::details::Swizzle<Impl, EI2, EI3, EI0, EI2> E2##E3##E0##E2;  \
Ubpa::details::Swizzle<Impl, EI2, EI3, EI0, EI3> E2##E3##E0##E3;  \
Ubpa::details::Swizzle<Impl, EI2, EI3, EI1, EI0> E2##E3##E1##E0;  \
Ubpa::details::Swizzle<Impl, EI2, EI3, EI1, EI1> E2##E3##E1##E1;  \
Ubpa::details::Swizzle<Impl, EI2, EI3, EI1, EI2> E2##E3##E1##E2;  \
Ubpa::details::Swizzle<Impl, EI2, EI3, EI1, EI3> E2##E3##E1##E3;  \
Ubpa::details::Swizzle<Impl, EI2, EI3, EI2, EI0> E2##E3##E2##E0;  \
Ubpa::details::Swizzle<Impl, EI2, EI3, EI2, EI1> E2##E3##E2##E1;  \
Ubpa::details::Swizzle<Impl, EI2, EI3, EI2, EI2> E2##E3##E2##E2;  \
Ubpa::details::Swizzle<Impl, EI2, EI3, EI2, EI3> E2##E3##E2##E3;  \
Ubpa::details::Swizzle<Impl, EI2, EI3, EI3, EI0> E2##E3##E3##E0;  \
Ubpa::details::Swizzle<Impl, EI2, EI3, EI3, EI1> E2##E3##E3##E1;  \
Ubpa::details::Swizzle<Impl, EI2, EI3, EI3, EI2> E2##E3##E3##E2;  \
Ubpa::details::Swizzle<Impl, EI2, EI3, EI3, EI3> E2##E3##E3##E3;  \
Ubpa::details::Swizzle<Impl, EI3, EI0, EI0, EI0> E3##E0##E0##E0;  \
Ubpa::details::Swizzle<Impl, EI3, EI0, EI0, EI1> E3##E0##E0##E1;  \
Ubpa::details::Swizzle<Impl, EI3, EI0, EI0, EI2> E3##E0##E0##E2;  \
Ubpa::details::Swizzle<Impl, EI3, EI0, EI0, EI3> E3##E0##E0##E3;  \
Ubpa::details::Swizzle<Impl, EI3, EI0, EI1, EI0> E3##E0##E1##E0;  \
Ubpa::details::Swizzle<Impl, EI3, EI0, EI1, EI1> E3##E0##E1##E1;  \
Ubpa::details::Swizzle<Impl, EI3, EI0, EI1, EI2> E3##E0##E1##E2;  \
Ubpa::details::Swizzle<Impl, EI3, EI0, EI1, EI3> E3##E0##E1##E3;  \
Ubpa::details::Swizzle<Impl, EI3, EI0, EI2, EI0> E3##E0##E2##E0;  \
Ubpa::details::Swizzle<Impl, EI3, EI0, EI2, EI1> E3##E0##E2##E1;  \
Ubpa::details::Swizzle<Impl, EI3, EI0, EI2, EI2> E3##E0##E2##E2;  \
Ubpa::details::Swizzle<Impl, EI3, EI0, EI2, EI3> E3##E0##E2##E3;  \
Ubpa::details::Swizzle<Impl, EI3, EI0, EI3, EI0> E3##E0##E3##E0;  \
Ubpa::details::Swizzle<Impl, EI3, EI0, EI3, EI1> E3##E0##E3##E1;  \
Ubpa::details::Swizzle<Impl, EI3, EI0, EI3, EI2> E3##E0##E3##E2;  \
Ubpa::details::Swizzle<Impl, EI3, EI0, EI3, EI3> E3##E0##E3##E3;  \
Ubpa::details::Swizzle<Impl, EI3, EI1, EI0, EI0> E3##E1##E0##E0;  \
Ubpa::details::Swizzle<Impl, EI3, EI1, EI0, EI1> E3##E1##E0##E1;  \
Ubpa::details::Swizzle<Impl, EI3, EI1, EI0, EI2> E3##E1##E0##E2;  \
Ubpa::details::Swizzle<Impl, EI3, EI1, EI0, EI3> E3##E1##E0##E3;  \
Ubpa::details::Swizzle<Impl, EI3, EI1, EI1, EI0> E3##E1##E1##E0;  \
Ubpa::details::Swizzle<Impl, EI3, EI1, EI1, EI1> E3##E1##E1##E1;  \
Ubpa::details::Swizzle<Impl, EI3, EI1, EI1, EI2> E3##E1##E1##E2;  \
Ubpa::details::Swizzle<Impl, EI3, EI1, EI1, EI3> E3##E1##E1##E3;  \
Ubpa::details::Swizzle<Impl, EI3, EI1, EI2, EI0> E3##E1##E2##E0;  \
Ubpa::details::Swizzle<Impl, EI3, EI1, EI2, EI1> E3##E1##E2##E1;  \
Ubpa::details::Swizzle<Impl, EI3, EI1, EI2, EI2> E3##E1##E2##E2;  \
Ubpa::details::Swizzle<Impl, EI3, EI1, EI2, EI3> E3##E1##E2##E3;  \
Ubpa::details::Swizzle<Impl, EI3, EI1, EI3, EI0> E3##E1##E3##E0;  \
Ubpa::details::Swizzle<Impl, EI3, EI1, EI3, EI1> E3##E1##E3##E1;  \
Ubpa::details::Swizzle<Impl, EI3, EI1, EI3, EI2> E3##E1##E3##E2;  \
Ubpa::details::Swizzle<Impl, EI3, EI1, EI3, EI3> E3##E1##E3##E3;  \
Ubpa::details::Swizzle<Impl, EI3, EI2, EI0, EI0> E3##E2##E0##E0;  \
Ubpa::details::Swizzle<Impl, EI3, EI2, EI0, EI1> E3##E2##E0##E1;  \
Ubpa::details::Swizzle<Impl, EI3, EI2, EI0, EI2> E3##E2##E0##E2;  \
Ubpa::details::Swizzle<Impl, EI3, EI2, EI0, EI3> E3##E2##E0##E3;  \
Ubpa::details::Swizzle<Impl, EI3, EI2, EI1, EI0> E3##E2##E1##E0;  \
Ubpa::details::Swizzle<Impl, EI3, EI2, EI1, EI1> E3##E2##E1##E1;  \
Ubpa::details::Swizzle<Impl, EI3, EI2, EI1, EI2> E3##E2##E1##E2;  \
Ubpa::details::Swizzle<Impl, EI3, EI2, EI1, EI3> E3##E2##E1##E3;  \
Ubpa::details::Swizzle<Impl, EI3, EI2, EI2, EI0> E3##E2##E2##E0;  \
Ubpa::details::Swizzle<Impl, EI3, EI2, EI2, EI1> E3##E2##E2##E1;  \
Ubpa::details::Swizzle<Impl, EI3, EI2, EI2, EI2> E3##E2##E2##E2;  \
Ubpa::details::Swizzle<Impl, EI3, EI2, EI2, EI3> E3##E2##E2##E3;  \
Ubpa::details::Swizzle<Impl, EI3, EI2, EI3, EI0> E3##E2##E3##E0;  \
Ubpa::details::Swizzle<Impl, EI3, EI2, EI3, EI1> E3##E2##E3##E1;  \
Ubpa::details::Swizzle<Impl, EI3, EI2, EI3, EI2> E3##E2##E3##E2;  \
Ubpa::details::Swizzle<Impl, EI3, EI2, EI3, EI3> E3##E2##E3##E3;  \
Ubpa::details::Swizzle<Impl, EI3, EI3, EI0, EI0> E3##E3##E0##E0;  \
Ubpa::details::Swizzle<Impl, EI3, EI3, EI0, EI1> E3##E3##E0##E1;  \
Ubpa::details::Swizzle<Impl, EI3, EI3, EI0, EI2> E3##E3##E0##E2;  \
Ubpa::details::Swizzle<Impl, EI3, EI3, EI0, EI3> E3##E3##E0##E3;  \
Ubpa::details::Swizzle<Impl, EI3, EI3, EI1, EI0> E3##E3##E1##E0;  \
Ubpa::details::Swizzle<Impl, EI3, EI3, EI1, EI1> E3##E3##E1##E1;  \
Ubpa::details::Swizzle<Impl, EI3, EI3, EI1, EI2> E3##E3##E1##E2;  \
Ubpa::details::Swizzle<Impl, EI3, EI3, EI1, EI3> E3##E3##E1##E3;  \
Ubpa::details::Swizzle<Impl, EI3, EI3, EI2, EI0> E3##E3##E2##E0;  \
Ubpa::details::Swizzle<Impl, EI3, EI3, EI2, EI1> E3##E3##E2##E1;  \
Ubpa::details::Swizzle<Impl, EI3, EI3, EI2, EI2> E3##E3##E2##E2;  \
Ubpa::details::Swizzle<Impl, EI3, EI3, EI2, EI3> E3##E3##E2##E3;  \
Ubpa::details::Swizzle<Impl, EI3, EI3, EI3, EI0> E3##E3##E3##E0;  \
Ubpa::details::Swizzle<Impl, EI3, EI3, EI3, EI1> E3##E3##E3##E1;  \
Ubpa::details::Swizzle<Impl, EI3, EI3, EI3, EI2> E3##E3##E3##E2;  \
Ubpa::details::Swizzle<Impl, EI3, EI3, EI3, EI3> E3##E3##E3##E3

#define UBPA_ARRAY_SWIZZLE3_2(EI0, EI1, EI2, E0, E1, E2) \
Ubpa::details::Swizzle<Impl, EI0, EI0> E0##E0;           \
Ubpa::details::Swizzle<Impl, EI0, EI1> E0##E1;           \
Ubpa::details::Swizzle<Impl, EI0, EI2> E0##E2;           \
Ubpa::details::Swizzle<Impl, EI1, EI0> E1##E0;           \
Ubpa::details::Swizzle<Impl, EI1, EI1> E1##E1;           \
Ubpa::details::Swizzle<Impl, EI1, EI2> E1##E2;           \
Ubpa::details::Swizzle<Impl, EI2, EI0> E2##E0;           \
Ubpa::details::Swizzle<Impl, EI2, EI1> E2##E1;           \
Ubpa::details::Swizzle<Impl, EI2, EI2> E2##E2

#define UBPA_ARRAY_SWIZZLE4_2(EI0, EI1, EI2, EI3, E0, E1, E2, E3) \
Ubpa::details::Swizzle<Impl, EI0, EI0> E0##E0;                    \
Ubpa::details::Swizzle<Impl, EI0, EI1> E0##E1;                    \
Ubpa::details::Swizzle<Impl, EI0, EI2> E0##E2;                    \
Ubpa::details::Swizzle<Impl, EI0, EI3> E0##E3;                    \
Ubpa::details::Swizzle<Impl, EI1, EI0> E1##E0;                    \
Ubpa::details::Swizzle<Impl, EI1, EI1> E1##E1;                    \
Ubpa::details::Swizzle<Impl, EI1, EI2> E1##E2;                    \
Ubpa::details::Swizzle<Impl, EI1, EI3> E1##E3;                    \
Ubpa::details::Swizzle<Impl, EI2, EI0> E2##E0;                    \
Ubpa::details::Swizzle<Impl, EI2, EI1> E2##E1;                    \
Ubpa::details::Swizzle<Impl, EI2, EI2> E2##E2;                    \
Ubpa::details::Swizzle<Impl, EI2, EI3> E2##E3;                    \
Ubpa::details::Swizzle<Impl, EI3, EI0> E3##E0;                    \
Ubpa::details::Swizzle<Impl, EI3, EI1> E3##E1;                    \
Ubpa::details::Swizzle<Impl, EI3, EI2> E3##E2;                    \
Ubpa::details::Swizzle<Impl, EI3, EI3> E3##E3

#define UBPA_ARRAY_SWIZZLE4_3(EI0, EI1, EI2, EI3, E0, E1, E2, E3) \
Ubpa::details::Swizzle<Impl, EI0, EI0, EI0> E0##E0##E0;           \
Ubpa::details::Swizzle<Impl, EI0, EI0, EI1> E0##E0##E1;           \
Ubpa::details::Swizzle<Impl, EI0, EI0, EI2> E0##E0##E2;           \
Ubpa::details::Swizzle<Impl, EI0, EI0, EI3> E0##E0##E3;           \
Ubpa::details::Swizzle<Impl, EI0, EI1, EI0> E0##E1##E0;           \
Ubpa::details::Swizzle<Impl, EI0, EI1, EI1> E0##E1##E1;           \
Ubpa::details::Swizzle<Impl, EI0, EI1, EI2> E0##E1##E2;           \
Ubpa::details::Swizzle<Impl, EI0, EI1, EI3> E0##E1##E3;           \
Ubpa::details::Swizzle<Impl, EI0, EI2, EI0> E0##E2##E0;           \
Ubpa::details::Swizzle<Impl, EI0, EI2, EI1> E0##E2##E1;           \
Ubpa::details::Swizzle<Impl, EI0, EI2, EI2> E0##E2##E2;           \
Ubpa::details::Swizzle<Impl, EI0, EI2, EI3> E0##E2##E3;           \
Ubpa::details::Swizzle<Impl, EI0, EI3, EI0> E0##E3##E0;           \
Ubpa::details::Swizzle<Impl, EI0, EI3, EI1> E0##E3##E1;           \
Ubpa::details::Swizzle<Impl, EI0, EI3, EI2> E0##E3##E2;           \
Ubpa::details::Swizzle<Impl, EI0, EI3, EI3> E0##E3##E3;           \
Ubpa::details::Swizzle<Impl, EI1, EI0, EI0> E1##E0##E0;           \
Ubpa::details::Swizzle<Impl, EI1, EI0, EI1> E1##E0##E1;           \
Ubpa::details::Swizzle<Impl, EI1, EI0, EI2> E1##E0##E2;           \
Ubpa::details::Swizzle<Impl, EI1, EI0, EI3> E1##E0##E3;           \
Ubpa::details::Swizzle<Impl, EI1, EI1, EI0> E1##E1##E0;           \
Ubpa::details::Swizzle<Impl, EI1, EI1, EI1> E1##E1##E1;           \
Ubpa::details::Swizzle<Impl, EI1, EI1, EI2> E1##E1##E2;           \
Ubpa::details::Swizzle<Impl, EI1, EI1, EI3> E1##E1##E3;           \
Ubpa::details::Swizzle<Impl, EI1, EI2, EI0> E1##E2##E0;           \
Ubpa::details::Swizzle<Impl, EI1, EI2, EI1> E1##E2##E1;           \
Ubpa::details::Swizzle<Impl, EI1, EI2, EI2> E1##E2##E2;           \
Ubpa::details::Swizzle<Impl, EI1, EI2, EI3> E1##E2##E3;           \
Ubpa::details::Swizzle<Impl, EI1, EI3, EI0> E1##E3##E0;           \
Ubpa::details::Swizzle<Impl, EI1, EI3, EI1> E1##E3##E1;           \
Ubpa::details::Swizzle<Impl, EI1, EI3, EI2> E1##E3##E2;           \
Ubpa::details::Swizzle<Impl, EI1, EI3, EI3> E1##E3##E3;           \
Ubpa::details::Swizzle<Impl, EI2, EI0, EI0> E2##E0##E0;           \
Ubpa::details::Swizzle<Impl, EI2, EI0, EI1> E2##E0##E1;           \
Ubpa::details::Swizzle<Impl, EI2, EI0, EI2> E2##E0##E2;           \
Ubpa::details::Swizzle<Impl, EI2, EI0, EI3> E2##E0##E3;           \
Ubpa::details::Swizzle<Impl, EI2, EI1, EI0> E2##E1##E0;           \
Ubpa::details::Swizzle<Impl, EI2, EI1, EI1> E2##E1##E1;           \
Ubpa::details::Swizzle<Impl, EI2, EI1, EI2> E2##E1##E2;           \
Ubpa::details::Swizzle<Impl, EI2, EI1, EI3> E2##E1##E3;           \
Ubpa::details::Swizzle<Impl, EI2, EI2, EI0> E2##E2##E0;           \
Ubpa::details::Swizzle<Impl, EI2, EI2, EI1> E2##E2##E1;           \
Ubpa::details::Swizzle<Impl, EI2, EI2, EI2> E2##E2##E2;           \
Ubpa::details::Swizzle<Impl, EI2, EI2, EI3> E2##E2##E3;           \
Ubpa::details::Swizzle<Impl, EI2, EI3, EI0> E2##E3##E0;           \
Ubpa::details::Swizzle<Impl, EI2, EI3, EI1> E2##E3##E1;           \
Ubpa::details::Swizzle<Impl, EI2, EI3, EI2> E2##E3##E2;           \
Ubpa::details::Swizzle<Impl, EI2, EI3, EI3> E2##E3##E3;           \
Ubpa::details::Swizzle<Impl, EI3, EI0, EI0> E3##E0##E0;           \
Ubpa::details::Swizzle<Impl, EI3, EI0, EI1> E3##E0##E1;           \
Ubpa::details::Swizzle<Impl, EI3, EI0, EI2> E3##E0##E2;           \
Ubpa::details::Swizzle<Impl, EI3, EI0, EI3> E3##E0##E3;           \
Ubpa::details::Swizzle<Impl, EI3, EI1, EI0> E3##E1##E0;           \
Ubpa::details::Swizzle<Impl, EI3, EI1, EI1> E3##E1##E1;           \
Ubpa::details::Swizzle<Impl, EI3, EI1, EI2> E3##E1##E2;           \
Ubpa::details::Swizzle<Impl, EI3, EI1, EI3> E3##E1##E3;           \
Ubpa::details::Swizzle<Impl, EI3, EI2, EI0> E3##E2##E0;           \
Ubpa::details::Swizzle<Impl, EI3, EI2, EI1> E3##E2##E1;           \
Ubpa::details::Swizzle<Impl, EI3, EI2, EI2> E3##E2##E2;           \
Ubpa::details::Swizzle<Impl, EI3, EI2, EI3> E3##E2##E3;           \
Ubpa::details::Swizzle<Impl, EI3, EI3, EI0> E3##E3##E0;           \
Ubpa::details::Swizzle<Impl, EI3, EI3, EI1> E3##E3##E1;           \
Ubpa::details::Swizzle<Impl, EI3, EI3, EI2> E3##E3##E2;           \
Ubpa::details::Swizzle<Impl, EI3, EI3, EI3> E3##E3##E3

#define UBPA_ARRAY_SWIZZLE2(E0, E1) UBPA_ARRAY_SWIZZLE2_2(0, 1, E0, E1)

#define UBPA_ARRAY_SWIZZLE3(E0, E1, E2)     \
UBPA_ARRAY_SWIZZLE3_2(0, 1, 2, E0, E1, E2); \
UBPA_ARRAY_SWIZZLE3_3(0, 1, 2, E0, E1, E2)

#define UBPA_ARRAY_SWIZZLE4(E0, E1, E2, E3)        \
UBPA_ARRAY_SWIZZLE4_2(0, 1, 2, 3, E0, E1, E2, E3); \
UBPA_ARRAY_SWIZZLE4_3(0, 1, 2, 3, E0, E1, E2, E3); \
UBPA_ARRAY_SWIZZLE4_4(0, 1, 2, 3, E0, E1, E2, E3)

namespace Ubpa {
	template<details::ArrayMode mode, typename IArray_Base, typename Impl>
	struct IArray_Impl;

	template<typename Base, typename Impl>
	struct IArray : IArray_Impl<details::GetArrayMode<Impl>(), Base, Impl> {
		using IArray_Impl<details::GetArrayMode<Impl>(), Base, Impl>::IArray_Impl;
	};

	template<typename IArray_Base, typename Impl>
	struct IArray_Impl<details::ArrayMode::One, IArray_Base, Impl>
		: details::IArrayCommon<IArray_Base, Impl>
	{
		using T = SI_ImplTraits_T<Impl>;
		static_assert(SI_ImplTraits_N<Impl> == 1);

		union {
			T _Elems[1];
			struct { T x; };
			struct { T r; };
			struct { T s; };
		};

		using details::IArrayCommon<IArray_Base, Impl>::IArrayCommon;
	};

	template<typename IArray_Base, typename Impl>
	struct IArray_Impl<details::ArrayMode::Two, IArray_Base, Impl>
		: details::IArrayCommon<IArray_Base, Impl>
	{
		using T = SI_ImplTraits_T<Impl>;
		static_assert(SI_ImplTraits_N<Impl> == 2);

		union {
			T _Elems[2];
			struct { T x, y; };
			struct { T r, g; };
			struct { T s, t; };

			UBPA_ARRAY_SWIZZLE2(x, y);
			UBPA_ARRAY_SWIZZLE2(r, g);
			UBPA_ARRAY_SWIZZLE2(s, t);
		};

		using details::IArrayCommon<IArray_Base, Impl>::IArrayCommon;

		template<typename... Us, std::enable_if_t<sizeof...(Us) == 2, int> = 0>
		constexpr IArray_Impl(Us... vals) noexcept : _Elems{ static_cast<T>(vals)... } {}
	};

	template<typename IArray_Base, typename Impl>
	struct IArray_Impl<details::ArrayMode::Three, IArray_Base, Impl>
		: details::IArrayCommon<IArray_Base, Impl>
	{
		using T = SI_ImplTraits_T<Impl>;
		static_assert(SI_ImplTraits_N<Impl> == 3);

		union {
			T _Elems[3];
			struct { T x, y, z; };
			struct { T r, g, b; };
			struct { T s, t, p; };

			UBPA_ARRAY_SWIZZLE3(x, y, z);
			UBPA_ARRAY_SWIZZLE3(r, g, b);
			UBPA_ARRAY_SWIZZLE3(s, t, p);
		};

		using details::IArrayCommon<IArray_Base, Impl>::IArrayCommon;

		template<typename... Us, std::enable_if_t<sizeof...(Us) == 3, int> = 0>
		constexpr IArray_Impl(Us... vals) noexcept : _Elems{ static_cast<T>(vals)... } {}
	};

	template<typename IArray_Base, typename Impl>
	struct IArray_Impl<details::ArrayMode::Four, IArray_Base, Impl>
		: details::IArrayCommon<IArray_Base, Impl>
	{
		using T = SI_ImplTraits_T<Impl>;
		static_assert(SI_ImplTraits_N<Impl> == 4);

		union {
			T _Elems[4];
			struct { T x, y, z, w; };
			struct { T r, g, b, a; };
			struct { T s, t, p, q; };

			UBPA_ARRAY_SWIZZLE4(x, y, z, w);
			UBPA_ARRAY_SWIZZLE4(r, g, b, a);
			UBPA_ARRAY_SWIZZLE4(s, t, p, q);
		};

		using details::IArrayCommon<IArray_Base, Impl>::IArrayCommon;

		template<typename... Us, std::enable_if_t<sizeof...(Us) == 4, int> = 0>
		constexpr IArray_Impl(Us... vals) noexcept : _Elems{ static_cast<T>(vals)... } {}
	};

	template<typename IArray_Base, typename Impl>
	struct IArray_Impl<details::ArrayMode::Basic, IArray_Base, Impl>
		: details::IArrayCommon<IArray_Base, Impl>
	{
		using T = SI_ImplTraits_T<Impl>;
		static constexpr std::size_t N = SI_ImplTraits_N<Impl>;

		T _Elems[N];

		using details::IArrayCommon<IArray_Base, Impl>::IArrayCommon;

		template<typename... Us, std::enable_if_t<sizeof...(Us) == N, int> = 0>
		constexpr IArray_Impl(Us... vals) noexcept : _Elems{static_cast<T>(vals)...} {}
	};

#ifdef UBPA_UGM_USE_SIMD
	template<typename IArray_Base, typename Impl>
	struct alignas(16) IArray_Impl<details::ArrayMode::Four_SIMD, IArray_Base, Impl>
		: details::IStdArrayBasic<IArray_Base, Impl>
	{
	public:
		using T = float;
		static constexpr size_t N = 4;
		using F = float;

		union {
			__m128 m;
			T _Elems[4];
			struct { T x, y, z, w; };
			struct { T r, g, b, a; };
			struct { T s, t, p, q; };

			UBPA_ARRAY_SWIZZLE4(x, y, z, w);
			UBPA_ARRAY_SWIZZLE4(r, g, b, a);
			UBPA_ARRAY_SWIZZLE4(s, t, p, q);
		};

		friend bool operator==(const Impl& x, const Impl& y) noexcept {
			return _mm_movemask_ps(_mm_cmpeq_ps(x, y)) == 0xf;
		}
		friend bool operator!=(const Impl& x, const Impl& y) noexcept {
			return _mm_movemask_ps(_mm_cmpneq_ps(x, y)) == 0xf;
		}

		friend bool operator<(const Impl& x, const Impl& y) noexcept {
			int mask_lt = _mm_movemask_ps(_mm_cmplt_ps(x, y));
			int mask_eq = _mm_movemask_ps(_mm_cmpeq_ps(x, y));
			for (size_t i = 0; i < 4; i++) {
				int cur_bit = (1 << i);
				if (mask_eq & cur_bit)
					continue;
				if (mask_lt & cur_bit)
					return true;
				else
					return false;
			}
			return false;
		}
		friend bool operator<=(const Impl& x, const Impl& y) noexcept {
			int mask_lt = _mm_movemask_ps(_mm_cmplt_ps(x, y));
			int mask_eq = _mm_movemask_ps(_mm_cmpeq_ps(x, y));
			for (size_t i = 0; i < 4; i++) {
				int cur_bit = (1 << i);
				if (mask_eq & cur_bit)
					continue;
				if (mask_lt & cur_bit)
					return true;
				else
					return false;
			}
			return true;
		}
		friend bool operator>(const Impl& x, const Impl& y) noexcept {
			int mask_lt = _mm_movemask_ps(_mm_cmplt_ps(x, y));
			int mask_eq = _mm_movemask_ps(_mm_cmpeq_ps(x, y));
			for (size_t i = 0; i < 4; i++) {
				int cur_bit = (1 << i);
				if (mask_eq & cur_bit)
					continue;
				if (mask_lt & cur_bit)
					return false;
				else
					return true;
			}
			return false;
		}
		friend bool operator>=(const Impl& x, const Impl& y) noexcept {
			int mask_lt = _mm_movemask_ps(_mm_cmplt_ps(x, y));
			int mask_eq = _mm_movemask_ps(_mm_cmpeq_ps(x, y));
			for (size_t i = 0; i < 4; i++) {
				int cur_bit = (1 << i);
				if (mask_eq & cur_bit)
					continue;
				if (mask_lt & cur_bit)
					return false;
				else
					return true;
			}
			return true;
		}
		bool lex_lt(const Impl& y) const noexcept {
			const auto& x = static_cast<const Impl&>(*this);
			return x < y;
		}
		bool lex_le(const Impl& y) const noexcept {
			const auto& x = static_cast<const Impl&>(*this);
			return x <= y;
		}
		bool lex_gt(const Impl& y) const noexcept {
			const auto& x = static_cast<const Impl&>(*this);
			return x > y;
		}
		bool lex_ge(const Impl& y) const noexcept {
			const auto& x = static_cast<const Impl&>(*this);
			return x >= y;
		}
		static bool all_lt(const Impl& x, const Impl& y) noexcept {
			return _mm_movemask_ps(_mm_cmplt_ps(x, y)) == 0xf;
		}
		static bool all_le(const Impl& x, const Impl& y) noexcept {
			return _mm_movemask_ps(_mm_cmple_ps(x, y)) == 0xf;
		}
		static bool all_gt(const Impl& x, const Impl& y) noexcept {
			return _mm_movemask_ps(_mm_cmpgt_ps(x, y)) == 0xf;
		}
		static bool all_ge(const Impl& x, const Impl& y) noexcept {
			return _mm_movemask_ps(_mm_cmpge_ps(x, y)) == 0xf;
		}
		bool all_lt(const Impl& y) const noexcept {
			const auto& x = static_cast<const Impl&>(*this);
			return all_lt(x, y);
		}
		bool all_le(const Impl& y) const noexcept {
			const auto& x = static_cast<const Impl&>(*this);
			return all_le(x, y);
		}
		bool all_gt(const Impl& y) const noexcept {
			const auto& x = static_cast<const Impl&>(*this);
			return all_gt(x, y);
		}
		bool all_ge(const Impl& y) const noexcept {
			const auto& x = static_cast<const Impl&>(*this);
			return all_ge(x, y);
		}

		// ==================

		using details::IStdArrayBasic<IArray_Base, Impl>::IStdArrayBasic;

		IArray_Impl() noexcept {}
		IArray_Impl(const float* f4) noexcept : m{ _mm_loadu_ps(f4) } {}
		IArray_Impl(__m128 f4) noexcept : m{ f4 } {} // align
		explicit IArray_Impl(float v) noexcept : m{ _mm_set1_ps(v) } {}
		IArray_Impl(const IArray_Impl& f4) noexcept : m{ f4.m } {}
		IArray_Impl& operator=(const IArray_Impl& f4) noexcept { m = f4.m; return *this; }
		operator __m128& () noexcept { return m; }
		operator const __m128&() const noexcept { return const_cast<IArray_Impl*>(this)->operator __m128 &(); }
		IArray_Impl(float x, float y, float z, float w) noexcept : m{ _mm_set_ps(w, z, y, x) } {} // little-endianness
		template<typename Ux, typename Uy, typename Uz, typename Uw>
		IArray_Impl(Ux x, Uy y, Uz z, Uw w) noexcept
			: IArray_Impl{ static_cast<float>(x),static_cast<float>(y),static_cast<float>(z),static_cast<float>(w) } {}

		static Impl zero() noexcept {
			return _mm_setzero_ps();
		}

		template<size_t i>
		float get() const noexcept {
			static_assert(i < 4);
			if constexpr (i == 0)
				return _mm_cvtss_f32(m);
			else
				return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(i, i, i, i)));
		}

		template<size_t i>
		void set(float v) noexcept {
			static_assert(i < 4);
			if constexpr (i == 0)
				m = _mm_move_ss(m, _mm_set_ss(v));
			else {
				__m128 t = _mm_move_ss(m, _mm_set_ss(v));

				if constexpr (i == 1)
					t = _mm_shuffle_ps(t, t, _MM_SHUFFLE(3, 2, 0, 0));
				else if constexpr (i == 2)
					t = _mm_shuffle_ps(t, t, _MM_SHUFFLE(3, 0, 1, 0));
				else // if constexpr (i == 3)
					t = _mm_shuffle_ps(t, t, _MM_SHUFFLE(0, 2, 1, 0));

				m = _mm_move_ss(t, m);
			}
		}

		template<size_t i>
		Impl replicate() const noexcept {
			static_assert(i < N);
			return _mm_shuffle_ps(m, m, _MM_SHUFFLE(i, i, i, i));
		}
	};
#endif
}
