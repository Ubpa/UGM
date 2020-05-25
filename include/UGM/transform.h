#pragma once

#include "bbox.h"
#include "line.h"
#include "ray.h"
#include "euler.h"
#include "quat.h"
#include "scale.h"
#include "mat.h"
#include "point.h"
#include "normal.h"
#include "vec.h"
#include "hvec.h"

#include "Interfaces/IMatrix/IMatrixMul.h"
#include "Interfaces/IMatrix/IMatrixInOut.h"

namespace Ubpa {
	template<typename T>
	struct ImplTraits<transform<T>> : ArrayTraits<vec<T, 4>, 4> {
		using IList = TemplateList<IMatrixInOut, IMatrixMul>;
		using F = T;
	};

	// TODO: distinguish different kinds of transformations
	// - basic transformation: translation, reflection, rotation, scaling, shearing, projection (projective transformation)
	// - rigid transformation: translation + rotation + reflection
	// - similarity transformation: translation + rotation + reflection + scaling
	// - linear transformation: rotation + reflection + scaling + shearing
	// - affine transformation: translation + linear transformation
	// - transformation: affine transformation + projection
	template<typename T>
	struct transform : SI<transform<T>> {
		using Base = SI<transform<T>>;
		using Base::Base;
		using Base::operator*;

		inline explicit transform(const mat<T, 4>& m) noexcept;
		inline explicit transform(const mat<T, 3>& m) noexcept;

		inline explicit transform(const point<T, 3>& pos) noexcept;
		inline explicit transform(const scale<T, 3>& scale) noexcept;
		inline explicit transform(const quat<T>& rot) noexcept;
		inline explicit transform(const euler<T>& euler) noexcept;

		inline transform(const point<T, 3>& pos, const scale<T, 3>& scale) noexcept;
		inline transform(const point<T, 3>& pos, const quat<T>& rot) noexcept;
		inline transform(const point<T, 3>& pos, const scale<T, 3>& scale, const quat<T>& rot) noexcept;
		inline transform(const vec<T, 3>& axis, T radian) noexcept;

		// world space -> camera space
		inline static const transform look_at(const point<T, 3>& pos, const point<T, 3>& target, const vec<T, 3>& up = vec<T, 3>(0, 1, 0)) noexcept;
		// orthographic, camera space -> clip space
		inline static const transform orthographic(T width, T height, T zNear, T zFar) noexcept;
		// perspective, camera space -> clip space
		// fovy: verticle field of view in radian
		// aspect : width / height
		inline static const transform perspective(T fovy, T aspect, T zNear, T zFar) noexcept;

		// sample: rotate_with<Axis::X>(to_radian(theta))
		template<Axis axis>
		inline static const transform rotate_with(T theta) noexcept;

		inline const point<T, 3> decompose_position() const noexcept { return { (*this)(0,3), (*this)(1,3), (*this)(2,3) }; }
		inline const scale<T, 3> decompose_scale() const noexcept;
		inline const mat<T, 3> decompose_rotation_matrix() const noexcept;
		inline const quat<T> decompose_quatenion() const noexcept;
		inline const euler<T> decompose_euler() const noexcept;
		inline const mat<T, 3> decompose_mat3() const noexcept;

		// faster than IMatrixMul::inverse
		// "sim" : similarity (translation, rotation [reflection], scaling)
		// ref: https://en.wikipedia.org/wiki/Similarity_(geometry)
		inline const transform inverse_sim() const noexcept;
		// TODO: transform::inverse_rigid
		// const transform inverse_rigid() const noexcept;

		inline const hvec<T, 4> operator*(const hvec<T, 4>& hv) const noexcept;
		inline const point<T, 3> operator*(const point<T, 3>& p) const noexcept;
		inline const vec<T, 3> operator*(const vec<T, 3>& v) const noexcept;
		// result isn't normalized
		inline const normal<T> operator*(const normal<T>& n) const noexcept;
		inline const bbox<T, 3> operator*(const bbox<T, 3>& b) const noexcept;
		inline const line<T, 3> operator*(const line<T, 3>& r) const noexcept;
		inline const ray<T, 3> operator*(const ray<T, 3>& r) const noexcept;
	};

	using transformf = transform<float>;

	// maybe error in editor, but no compile error
	static_assert(sizeof(transformf) == 16 * sizeof(float));
}

#include "detail/transform.inl"
