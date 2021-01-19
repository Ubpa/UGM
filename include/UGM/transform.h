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
	template<typename F>
	struct SI_ImplTraits<transform<F>>
		: ArrayTraits<vec<F, 4>, 4, F>,
		IListTraits<IMatrixInOut, IMatrixMul> {};

	// TODO: distinguish different kinds of transformations
	// - basic transformation: translation, reflection, rotation, scaling, shearing, projection (projective transformation)
	// - rigid transformation: translation + rotation + reflection
	// - similarity transformation: translation + rotation + reflection + scaling
	// - linear transformation: rotation + reflection + scaling + shearing
	// - affine transformation: translation + linear transformation
	// - transformation: affine transformation + projection
	template<typename F>
	struct transform : SI<transform<F>> {
		using Base = SI<transform<F>>;
		using Base::Base;
		using Base::operator*;

		explicit transform(const mat<F, 4>& m) noexcept;
		explicit transform(const mat<F, 3>& m) noexcept;

		explicit transform(const vec<F, 3>& translation) noexcept;
		explicit transform(const quat<F>& rot) noexcept;
		explicit transform(const euler<F>& euler) noexcept;
		transform(const vec<F, 3>& axis, F radian) noexcept;
		explicit transform(const scale<F, 3>& scale) noexcept;
		explicit transform(F scale) noexcept;

		transform(const vec<F, 3>& translation, const scale<F, 3>& scale) noexcept;
		transform(const vec<F, 3>& translation, const quat<F>& rot) noexcept;
		transform(const quat<F>& rot, const scale<F, 3>& scale) noexcept;
		transform(const vec<F, 3>& translation, const quat<F>& rot, const scale<F, 3>& scale) noexcept;

		// world space -> camera space
		// right hand, forward is camera's back
		static transform look_at(
			const point<F, 3>& pos,
			const point<F, 3>& target,
			const vec<F, 3>& up = vec<F, 3>(0, 1, 0)
		) noexcept;

		// orthographic, camera space -> clip space
		static transform orthographic(F width, F height, F zNear, F zFar) noexcept;
		// perspective, camera space -> clip space
		// fovy: verticle field of view in radian
		// aspect : width / height
		static transform perspective(F fovy, F aspect, F zNear, F zFar,
			F near_clip_vlaue = UBPA_DEFAULT_NEAR_CLIP_VALUE) noexcept;

		// sample: rotate_with<Axis::X>(to_radian(theta))
		template<Axis axis>
		static transform rotate_with(F theta) noexcept;

		vec<F, 3> decompose_translation() const noexcept;
		mat<F, 3> decompose_rotation_matrix() const noexcept;
		quat<F> decompose_quatenion() const noexcept;
		euler<F> decompose_euler() const noexcept;
		mat<F, 3> decompose_mat3() const noexcept;
		scale<F, 3> decompose_scale() const noexcept;

		// faster than IMatrixMul::inverse
		// "sim" : similarity (translation, scaling, rotation [reflection])
		// ref: https://en.wikipedia.org/wiki/Similarity_(geometry)
		transform inverse_sim() const noexcept;
		// TODO: transform::inverse_rigid
		// transform inverse_rigid() const noexcept;

		hvec<F, 4> operator*(const hvec<F, 4>& hv) const noexcept;
		point<F, 3> operator*(const point<F, 3>& p) const noexcept;
		vec<F, 3> operator*(const vec<F, 3>& v) const noexcept;
		// result isn't normalized
		normal<F> operator*(const normal<F>& n) const noexcept;
		bbox<F, 3> operator*(const bbox<F, 3>& b) const noexcept;
		line<F, 3> operator*(const line<F, 3>& r) const noexcept;
		ray<F, 3> operator*(const ray<F, 3>& r) const noexcept;
	};

	using transformf = transform<float>;

	// maybe error in editor, but no compile error
	static_assert(sizeof(transformf) == 16 * sizeof(float));
}

#include "details/transform.inl"
