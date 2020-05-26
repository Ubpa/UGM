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
		template<size_t... Ns>
		IArrayImpl(const IArrayImpl& arr, std::index_sequence<Ns...>) noexcept {
			((this->at(Ns) = arr[Ns]), ...);
		};
		IArrayImpl(const IArrayImpl& arr) noexcept : IArrayImpl{ arr, std::make_index_sequence<N>{} } {}

		template<size_t... Ns>
		IArrayImpl(T t, std::index_sequence<Ns...>) noexcept {
			((this->at(Ns) = t), ...);
		};
		constexpr IArrayImpl(T t) noexcept : IArrayImpl{ t, std::make_index_sequence<N>{} } {}

		template<typename... U, typename = std::enable_if_t<(std::is_convertible_v<U, T>&&...)>>
		constexpr IArrayImpl(U... data) noexcept : std::array<T, N>{static_cast<T>(data)...} {
			static_assert(sizeof...(U) == N, "number of parameters is not correct");
		}
	};

#ifdef UBPA_USE_SIMD
	// alignas(16)
	template<typename Base, typename Impl>
	struct alignas(16) IArrayImpl<true, Base, Impl> : Base
	{
	public:
		__m128 m;

	public:
		using T = float;
		static constexpr size_t N = 4;
		using F = float;

		// array interface
		std::array<float, 4>& to_array() noexcept { return reinterpret_cast<std::array<float, 4>&>(*this); }
		const std::array<float, 4>& to_array() const noexcept { return const_cast<IArrayImpl*>(this)->to_array(); }

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

		float x() const noexcept { return _mm_cvtss_f32(m); }
		float y() const noexcept { return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(1, 1, 1, 1))); }
		float z() const noexcept { return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(2, 2, 2, 2))); }
		float w() const noexcept { return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(3, 3, 3, 3))); }
		void set_x(float v) noexcept { m = _mm_move_ss(m, _mm_set_ss(v)); }
		void set_y(float v) noexcept {
			__m128 t = _mm_move_ss(m, _mm_set_ss(v));
			t = _mm_shuffle_ps(t, t, _MM_SHUFFLE(3, 2, 0, 0));
			m = _mm_move_ss(t, m);
		}
		void set_z(float v) noexcept {
			__m128 t = _mm_move_ss(m, _mm_set_ss(v));
			t = _mm_shuffle_ps(t, t, _MM_SHUFFLE(3, 0, 1, 0));
			m = _mm_move_ss(t, m);
		}
		void set_w(float v) noexcept {
			__m128 t = _mm_move_ss(m, _mm_set_ss(v));
			t = _mm_shuffle_ps(t, t, _MM_SHUFFLE(0, 2, 1, 0));
			m = _mm_move_ss(t, m);
		}
		template<size_t i>
		float at() const noexcept {
			static_assert(i < 4);
			if constexpr (i == 0)
				return x();
			else
				return _mm_cvtss_f32(_mm_shuffle_ps(m, m, _MM_SHUFFLE(i, i, i, i)));
		}
		template<size_t i>
		void set(float v) const noexcept {
			static_assert(i < 4);
			if constexpr (i == 0)
				return set_x(v);
			else if constexpr (i == 1)
				return set_y(v);
			else if constexpr (i == 2)
				return set_z(v);
			else if constexpr (i == 3)
				return set_w(v);
		}

		float* data() noexcept { return reinterpret_cast<float*>(this); }
		const float* data() const noexcept { return const_cast<IArrayImpl*>(this)->data(); }

		IArrayImpl() noexcept {}
		IArrayImpl(__m128 f4) noexcept : m{ f4 } {}
		explicit IArrayImpl(float v) noexcept : m{ _mm_set1_ps(v) } {}
		IArrayImpl(const IArrayImpl& f4) noexcept : m{ f4.m } {}
		IArrayImpl& operator=(const IArrayImpl& f4) noexcept { m = f4.m; return *this; }
		operator __m128& () noexcept { return m; }
		operator const __m128&() const noexcept { return const_cast<IArrayImpl*>(this)->operator __m128 &(); }
		IArrayImpl(float x, float y, float z, float w) noexcept : m{ _mm_set_ps(w, z, y, x) } {} // little-endianness
		template<typename Ux, typename Uy, typename Uz, typename Uw>
		IArrayImpl(Ux x, Uy y, Uz z, Uw w) noexcept
			: IArrayImpl{ static_cast<float>(x),static_cast<float>(y),static_cast<float>(z),static_cast<float>(w) } {}
	};
#endif
}
