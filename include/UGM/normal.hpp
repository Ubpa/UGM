#pragma once

#include "point.hpp"

namespace Ubpa {
	template<typename T>
	struct SI_ImplTraits<normal<T>>
		: Array1DTraits<T, 3>,
		IListTraits<IArray1D_Util, ICross> {};

	// it's necessary to distinguish vector and normal(bivector) when transformings
	// ref
	// - PBRT p. 86-87
	// - wikipedia-bivector: https://en.wikipedia.org/wiki/Bivector
	// - https://stackoverflow.com/questions/30465573/transforming-surface-normal-vectors-and-tangent-vectors
	template<typename T>
	struct normal : SI<normal<T>> {
		using SI<normal<T>>::SI;

		// (theta, phi)
		// theta : 0 - pi
		// phi   : 0 - 2 pi
		point<T, 2> to_sphere_coordinate() const noexcept;

		vec<T, 3> to_sphere_tangent() const noexcept;
	};

	using normalf = normal<float>;
	using normald = normal<double>;

	// maybe error in editor, but no compile error
	static_assert(sizeof(normalf) == 3 * sizeof(float));
}

#include "details/normal.inl"
