#pragma once

#include "point.h"

namespace Ubpa {
	template<typename T>
	struct ImplTraits<normal<T>> : ImplTraits<vec<T, 3>> {};

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
		const point<T, 2> to_sphere_coordinate() const noexcept;

		const vec<T, 3> to_sphere_tangent() const noexcept;
	};

	using normalf = normal<float>;

	// maybe error in editor, but no compile error
	static_assert(sizeof(normalf) == 3 * sizeof(float));
}

#include "details/normal.inl"
