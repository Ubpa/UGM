#pragma once

namespace Ubpa {
	template<typename T, size_t N>
	val<T, N>::val(const vec<T, N>& v) noexcept {
		for (size_t i = 0; i < N; i++)
			(*this)[i] = v[i];
	}

	template<typename T, size_t N>
	val<T, N>::val(const normal<T>& v) noexcept {
		static_assert(N == 3);
		for (size_t i = 0; i < 3; i++)
			(*this)[i] = v[i];
	}

	template<typename T, size_t N>
	val<T, N>::val(const point<T, N>& v) noexcept {
		for (size_t i = 0; i < N; i++)
			(*this)[i] = v[i];
	}

	template<typename T, size_t N>
	val<T, N>::val(const rgb<T>& v) noexcept {
		static_assert(N == 3);
		for (size_t i = 0; i < 3; i++)
			(*this)[i] = v[i];
	}

	template<typename T, size_t N>
	val<T, N>::val(const rgba<T>& v) noexcept {
		static_assert(N == 4);
		for (size_t i = 0; i < 4; i++)
			(*this)[i] = v[i];
	}

	template<typename T, size_t N>
	val<T, N>::val(const quat<T>& v) noexcept {
		static_assert(N == 4);
		for (size_t i = 0; i < 4; i++)
			(*this)[i] = v[i];
	}

	template<typename T, size_t N>
	val<T, N>::val(const euler<T>& v) noexcept {
		static_assert(N == 3);
		for (size_t i = 0; i < 3; i++)
			(*this)[i] = v[i];
	}
}
