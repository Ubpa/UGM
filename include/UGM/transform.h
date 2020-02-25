#pragma once

#include "vec.h"
#include "point.h"
#include "normal.h"
#include "mat.h"
#include "scale.h"
#include "quat.h"

#include "Interfaces/IMatrix/IMatrixMul.h"
#include "Interfaces/IMatrix/IMatrixInOut.h"

namespace Ubpa {
	template<typename T>
	struct transform : SIIT_CRTP<TemplateList<IMatrixInOut, IMatrixMul>, transform<T>, TypeList<TypeList<vec<T, 4>, Size<4>>, T>> {
		using SIIT_CRTP<TemplateList<IMatrixInOut, IMatrixMul>, transform<T>, TypeList<TypeList<vec<T, 4>, Size<4>>, T>>::SIIT_CRTP;
		using SIIT_CRTP<TemplateList<IMatrixInOut, IMatrixMul>, transform<T>, TypeList<TypeList<vec<T, 4>, Size<4>>, T>>::init;

		explicit transform(const mat<T, 4>& m) noexcept : transform(m[0], m[1], m[2], m[3]) {}
		
		explicit transform(const vec<T, 3>& translation) noexcept;
		explicit transform(const scale<T, 3>& scale) noexcept;
		transform(const vec<T, 3>& normalizedAxis, float radian) noexcept { init(normalizedAxis, radian); }
		explicit transform(const quat<T>& q) noexcept;

		// world space -> camera space
		static const transform look_at(const point<T,3>& pos, const point<T, 3>& target, const vec<T,3>& up = vec<T,3>(0, 1, 0)) noexcept;
		// orthographic, camera space -> clip space
		static const transform orthographic(float width, float height, float zNear, float zFar) noexcept;
		// perspcetive, camera space -> clip space
		// fovy: verticle field of view in radian
		// aspect : width / height
		static const transform perspcetive(float fovy, float aspect, float zNear, float zFar) noexcept;

	private:
		void init(const vec<T, 3>& normalizedAxis, float radian) noexcept;
	};

	using transformf = transform<float>;
}

#include "transform.inl"
