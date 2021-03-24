#pragma once

#include "point.hpp"

namespace Ubpa {
	template<typename T, size_t N>
	struct SI_ImplTraits<hvec<T, N>> :
		SI_ImplTraits<vec<T, N>> {};

	template<typename T, size_t N>
	struct hvec : SI<hvec<T, N>> {
	private:
		using Base = SI<hvec<T, N>>;
		using Base::Base;

		static_assert(N > 1);

		template<typename Container, size_t... Ns>
		hvec(const Container& c, T h, std::index_sequence<Ns...>) noexcept;
		template<typename Container, size_t... Ns>
		hvec(const Container& c, std::index_sequence<Ns...>) noexcept;

	public:
		hvec(const point<T, N - 1>& p) noexcept;
		hvec(const vec<T, N - 1>& v) noexcept;
		hvec(const vec<T, N>& v) noexcept;

		point<T, N - 1> to_point() const noexcept;
		vec<T, N - 1> to_vec() const noexcept;
	};

	template<size_t N>
	using hvecf = hvec<float, N>;

	using hvecf1 = hvecf<1>;
	using hvecf2 = hvecf<2>;
	using hvecf3 = hvecf<3>;
	using hvecf4 = hvecf<4>;

	template<size_t N>
	using hveci = hvec<int, N>;

	using hveci1 = hveci<1>;
	using hveci2 = hveci<2>;
	using hveci3 = hveci<3>;
	using hveci4 = hveci<4>;

	template<size_t N>
	using hvecu = hvec<unsigned, N>;

	using hvecu1 = hvecu<1>;
	using hvecu2 = hvecu<2>;
	using hvecu3 = hvecu<3>;
	using hvecu4 = hvecu<4>;

	// maybe error in editor, but no compile error
	static_assert(sizeof(hvecf4) == 4 * sizeof(float));
}

#include "details/hvec.inl"
