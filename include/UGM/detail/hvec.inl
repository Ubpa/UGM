#pragma once

namespace Ubpa {
	template<typename T, size_t N>
	template<typename Container, size_t... Ns>
	hvec<T, N>::hvec(const Container& c, T h, std::index_sequence<Ns...>) noexcept
		: Base{ c[Ns]...,h } {}
	template<typename T, size_t N>
	template<typename Container, size_t... Ns>
	hvec<T, N>::hvec(const Container& c, std::index_sequence<Ns...>) noexcept
		: Base{ c[Ns]... } {}

	template<typename T, size_t N>
	hvec<T, N>::hvec(const point<T, N - 1>& p) noexcept
		: hvec{ p, ONE<T>, std::make_index_sequence<N - 1>{} } {}

	template<typename T, size_t N>
	hvec<T, N>::hvec(const vec<T, N - 1>& v) noexcept
		: hvec{ v, ZERO<T>, std::make_index_sequence<N - 1>{} } {}

	template<typename T, size_t N>
	hvec<T, N>::hvec(const vec<T, N>& v) noexcept
		: hvec{ v,std::make_index_sequence<N>{} } {}

	template<typename T, size_t N>
	point<T, N - 1> hvec<T, N>::to_point() const noexcept {
		assert((*this)[N - 1] != ZERO<T>);
#ifdef USE_XSIMD
		if constexpr (std::is_same_v<T, float> && N == 4)
			return ((*this) / (*this)[3]).cast_to<pointf3>();
		else
#endif // USE_XSIMD
		{
			if constexpr (N >= 4) {
				if ((*this)[N - 1] == ONE<T>)
					return (*this)->cast_to<point<T, N - 1>>();
				else {
					point<T, N - 1> rst;
					T inv = ONE<T> / (*this)[N - 1];
					for (size_t i = 0; i < N - 1; i++)
						rst[i] = inv * (*this)[i];
					return rst;
				}
			}
			else {
				point<T, N - 1> rst;
				T inv = ONE<T> / (*this)[N - 1];
				for (size_t i = 0; i < N - 1; i++)
					rst[i] = inv * (*this)[i];
				return rst;
			}
		}
	}

	template<typename T, size_t N>
	vec<T, N - 1> hvec<T, N>::to_vec() const noexcept {
		assert(std::abs((*this)[N - 1]) < EPSILON<T>);
		return this->cast_to<vec<T, N - 1>>();
	}
}
