#pragma once

#include "val.h"

namespace Ubpa {
	// [0, 1) x [0, 1)
	template<typename T>
	const val<T, 2> uniform_in_square() noexcept;

	// center: (0, 0), radius: 1
	template<typename T>
	const val<T, 2> uniform_on_disk() noexcept;

	// center: (0, 0), radius: [0, 1)
	template<typename T>
	const val<T, 2> uniform_in_disk() noexcept;

	// center: (0, 0, 0), radius: 1
	template<typename T>
	const val<T, 3> uniform_on_sphere() noexcept;

	// center: (0, 0, 0), radius: [0, 1)
	template<typename T>
	const val<T, 3> uniform_in_sphere() noexcept;

	// center: (0, 0, 0), radius: 1
	template<typename T, Axis up = Axis::Z>
	const val<T, 3> cos_weighted_on_hemisphere() noexcept;
}

#include "detail/sample.inl"
