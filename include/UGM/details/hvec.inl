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
		: hvec{ p, One<T>, std::make_index_sequence<N - 1>{} } {}

	template<typename T, size_t N>
	hvec<T, N>::hvec(const vec<T, N - 1>& v) noexcept
		: hvec{ v, Zero<T>, std::make_index_sequence<N - 1>{} } {}

	template<typename T, size_t N>
	hvec<T, N>::hvec(const vec<T, N>& v) noexcept
		: hvec{ v,std::make_index_sequence<N>{} } {}

	template<typename T, size_t N>
	point<T, N - 1> hvec<T, N>::to_point() const noexcept {
		assert((*this)[N - 1] != Zero<T>);
#ifdef UBPA_UGM_USE_SIMD
		if constexpr (SI_ImplTraits_SupportSIMD<hvec>)
			return ((*this) / this->template get<3>()).template cast_to<pointf3>();
		else
#endif // UBPA_UGM_USE_SIMD
		{
			if constexpr (N >= 4) {
				if ((*this)[N - 1] == One<T>)
					return this->template cast_to<point<T, N - 1>>();
				else {
					point<T, N - 1> rst;
					T inv = One<T> / (*this)[N - 1];
					for (size_t i = 0; i < N - 1; i++)
						rst[i] = inv * (*this)[i];
					return rst;
				}
			}
			else {
				point<T, N - 1> rst;
				T inv = One<T> / (*this)[N - 1];
				for (size_t i = 0; i < N - 1; i++)
					rst[i] = inv * (*this)[i];
				return rst;
			}
		}
	}

	template<typename T, size_t N>
	vec<T, N - 1> hvec<T, N>::to_vec() const noexcept {
		assert(std::abs((*this)[N - 1]) < Epsilon<T>);
		return this->template cast_to<vec<T, N - 1>>();
	}
}
