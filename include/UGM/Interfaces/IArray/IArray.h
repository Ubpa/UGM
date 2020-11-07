#pragma once

#include "../../basic.h"
#include "../ImplTraits.h"

#include <array>
#include <vector>
#include <iostream>

#include <cmath>
#include <cassert>

namespace Ubpa {
	template<bool SIMD, typename IArray_Base, typename Impl>
	struct IArray_Impl;

	template<typename Base, typename Impl>
	struct IArray : IArray_Impl<ImplTraits_SupportSIMD<Impl>, Base, Impl> {
		using IArray_Impl<ImplTraits_SupportSIMD<Impl>, Base, Impl>::IArray_Impl;
	};

	template<typename IArray_Base, typename Impl>
	struct alignas(alignof(ImplTraits_T<Impl>)) IArray_Impl<false, IArray_Base, Impl>
		: IArray_Base, std::array<ImplTraits_T<Impl>, ImplTraits_N<Impl>>
	{
		using T = ImplTraits_T<Impl>;
		using F = ImplTraits_F<Impl>;
		static constexpr size_t N = ImplTraits_N<Impl>;

	private:
		using IArray_Base::operator[];

		template<size_t... Ns>
		inline IArray_Impl(const T* arr, std::index_sequence<Ns...>) noexcept {
			(((*this)[Ns] = arr[Ns]), ...);
		}
		template<size_t... Ns>
		inline IArray_Impl(const T& t, std::index_sequence<Ns...>) noexcept {
			(((*this)[Ns] = t), ...);
		};
		template<size_t... Ns>
		inline IArray_Impl(const IArray_Impl& arr, std::index_sequence<Ns...>) noexcept {
			(((*this)[Ns] = arr[Ns]), ...);
		};
	public:
		using std::array<T, N>::operator[];

		static_assert(N > 0);

		using IArray_Base::IArray_Base;
		using std::array<T, N>::array;

		IArray_Impl() noexcept {};

		constexpr IArray_Impl(const T* arr) noexcept : IArray_Impl{ arr, std::make_index_sequence<N>{} } {}

		constexpr IArray_Impl(const IArray_Impl& arr) noexcept : IArray_Impl{ arr, std::make_index_sequence<N>{} } {}

		constexpr IArray_Impl(const T& t) noexcept : IArray_Impl{ t, std::make_index_sequence<N>{} } {}

		template<typename... Us, std::enable_if_t<sizeof...(Us) == N, int> = 0>
		constexpr IArray_Impl(Us... vals) noexcept : std::array<T, N>{static_cast<T>(vals)...} {}

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

#ifdef UBPA_UGM_USE_SIMD
	template<typename IArray_Base, typename Impl>
	struct alignas(16) IArray_Impl<true, IArray_Base, Impl> : IArray_Base {
	public:
		__m128 m;

	public:
		using T = float;
		static constexpr size_t N = 4;
		using F = float;

		// array interface
		std::array<float, 4>& to_array() noexcept { return reinterpret_cast<std::array<float, 4>&>(*this); }
		const std::array<float, 4>& to_array() const noexcept { return const_cast<IArray_Impl*>(this)->to_array(); }

		using value_type      = float;
		using size_type       = size_t;
		using difference_type = ptrdiff_t;
		using pointer         = float*;
		using const_pointer   = const float*;
		using reference       = float&;
		using const_reference = const float&;

		using iterator       = std::array<float, 4>::iterator;
		using const_iterator = std::array<float, 4>::const_iterator;

		using reverse_iterator       = std::array<float, 4>::reverse_iterator;
		using const_reverse_iterator = std::array<float, 4>::const_reverse_iterator;

		void fill(float v) noexcept { m = _mm_set1_ps(v); }
		void swap(Impl& other) noexcept { std::swap(m, other.m); }

		iterator begin() noexcept { return to_array().begin();
		}
		const_iterator begin() const noexcept { return to_array().begin(); }
		iterator end() noexcept {
			return to_array().end();
		}
		const_iterator end() const noexcept { return to_array().end(); }

		iterator rbegin() noexcept {
			return to_array().rbegin();
		}
		const_iterator rbegin() const noexcept { return to_array().rbegin(); }
		iterator rend() noexcept {
			return to_array().rend();
		}
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

		float* data() noexcept { return reinterpret_cast<float*>(this); }
		const float* data() const noexcept { return const_cast<IArray_Impl*>(this)->data(); }

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
		void set(float v) const noexcept {
			static_assert(i < 4);
			if constexpr (i == 0) {
				m = _mm_move_ss(m, _mm_set_ss(v));
			}
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
