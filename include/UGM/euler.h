#pragma once

#include "quat.h"

#include "Interfaces/IArray/IArray1D_Util.h"

namespace Ubpa {
	template<typename T>
	struct quat;

	// same with Unity3D
	// Z -> X -> Y
	// roll -> pitch -> yaw
	// Tait¨CBryan angles, extrinsic rotations (math: x-y-z, game: z-x-y)
	// ref: https://en.wikipedia.org/wiki/Euler_angles
	template<typename T>
	struct euler : SI<TemplateList<IArray1D_Util>, euler<T>, TypeList<T, Size<3>>, T> {
		using SI<TemplateList<IArray1D_Util>, euler<T>, TypeList<T, Size<3>>, T>::SI;

		inline const quat<T> to_quat() const noexcept;
	};

	using eulerf = euler<float>;

	// maybe error in editor, but no compile error
	static_assert(sizeof(eulerf) == 3 * sizeof(float));
}

#include "detail/euler.inl"
