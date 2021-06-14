#pragma once

#include "vec.hpp"

#include "Interfaces/IArray/IArray1D_Util.hpp"
#include "Interfaces/IArray/IEuclideanA.hpp"

namespace Ubpa {
	template<typename T, size_t N>
	struct SI_ImplTraits<point<T, N>>
		: Array1DTraits<T, N>,
		IListTraits<IArray1D_Util, IEuclideanA>,
		ArrayImplNTraits<point, T>
	{
		using V = vec<T, N>;
	};

	template<typename T, size_t N>
	struct point : SI<point<T, N>> {
		using SI<point<T, N>>::SI;

		template<typename Container>
		static point combine(const Container& points, T weight) noexcept;

		template<typename PContainer, typename WContainer>
		static point combine(PContainer points, WContainer weights) noexcept;

		template<typename Container>
		static point center(const Container& points) noexcept {
			return combine(points, static_cast<T>(1) / static_cast<T>(points.size()));
		}
	};

	template<size_t N>
	using pointf = point<float, N>;

	using pointf1 = pointf<1>;
	using pointf2 = pointf<2>;
	using pointf3 = pointf<3>;
	using pointf4 = pointf<4>;

	template<size_t N>
	using pointd = point<double, N>;

	using pointd1 = pointd<1>;
	using pointd2 = pointd<2>;
	using pointd3 = pointd<3>;
	using pointd4 = pointd<4>;

	template<size_t N>
	using pointi = point<int, N>;

	using pointi1 = pointi<1>;
	using pointi2 = pointi<2>;
	using pointi3 = pointi<3>;
	using pointi4 = pointi<4>;

	template<size_t N>
	using pointu = point<unsigned, N>;

	using pointu1 = pointu<1>;
	using pointu2 = pointu<2>;
	using pointu3 = pointu<3>;
	using pointu4 = pointu<4>;

	// maybe error in editor, but no compile error
	static_assert(sizeof(pointf3) == sizeof(float) * 3);
}

#include "details/point.inl"
