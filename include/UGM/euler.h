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
	struct euler;

	template<typename T_>
	struct ImplTraits<euler<T_>> {
		using IList = TemplateList<IArray1D_Util>;
		using T = T_;
		static constexpr size_t N = 3;
		using F = T;
	};

	template<typename T>
	struct euler : SI<euler<T>> {
		using SI<euler<T>>::SI;

		inline const quat<T> to_quat() const noexcept;
	};

	using eulerf = euler<float>;

	// maybe error in editor, but no compile error
	static_assert(sizeof(eulerf) == 3 * sizeof(float));
}

#include "detail/euler.inl"
