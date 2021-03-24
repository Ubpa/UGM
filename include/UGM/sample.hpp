#pragma once

#include "val.hpp"
#include "svec.hpp"

namespace Ubpa {
	// [0, 1) x [0, 1)
	template<typename T>
	val<T, 2> uniform_in_square() noexcept;

	// center: (0, 0), radius: 1
	template<typename T>
	val<T, 2> uniform_on_disk() noexcept;

	// center: (0, 0), radius: [0, 1)
	template<typename T>
	val<T, 2> uniform_in_disk() noexcept;

	// center: (0, 0, 0), radius: 1
	template<typename T>
	val<T, 3> uniform_on_sphere() noexcept;

	// center: (0, 0, 0), radius: [0, 1)
	template<typename T>
	val<T, 3> uniform_in_sphere() noexcept;

	// center: (0, 0, 0), radius: 1
	template<typename T, Axis up = Axis::Z>
	val<T, 3> cos_weighted_on_hemisphere() noexcept;

	// [summary]
	// sample microfacet normal from GGX normal distribuion function
	// ref: Walter B, et al. Microfacet Models for Refraction through Rough Surfaces.
	// [return]
	// wm: microfacet normal
	svecf sample_GGX_D(float alpha) noexcept;
}

#include "details/sample.inl"
