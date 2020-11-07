#pragma once

#include "point.h"
#include "euler.h"

#include "Interfaces/IArray/IArrayUtil.h"
#include "Interfaces/IMul.h"

namespace Ubpa {
	template<typename T>
	struct ImplTraits<quat<T>>
		: Array1DTraits<T, 4>,
		SIMDTraits<false>, // float4 not use SIMD
		IListTraits<IMul, IArrayUtil>
		{};

	template<typename T>
	struct quat : SI<quat<T>> {
		using Base = SI<quat<T>>;
		using Base::Base;
		using Base::operator*;

		inline static const quat imag_real(const vec<T, 3>& imag, T real) noexcept;

		inline quat(const vec<T, 3>& axis, T theta) noexcept;

		// from and to is normalized
		inline quat(const vec<T, 3>& from, const vec<T, 3>& to) noexcept;

		// axis * sin(theta/2)
		inline vec<T, 3>& imag() noexcept;
		inline const vec<T, 3>& imag() const noexcept;

		// cos(thete/2)
		inline T& real() noexcept;
		inline T real() const noexcept;

		// radian
		inline T theta() const noexcept;
		inline const vec<T, 3> axis() const noexcept;

		inline bool is_identity() const noexcept;
		inline static const quat identity() noexcept;

		inline bool is_unit() const noexcept;

		// rotate with axis x/y/z
		// theta : radian
		// example: quatf::rotate_with<Axis::X>(to_radian(45.f))
		template<Axis axis>
		inline static const quat rotate_with(T theta) noexcept;

		inline const euler<T> to_euler() const noexcept;

		inline const vec<T, 3> operator*(const vec<T, 3>& v) const noexcept;

	private:
		template<typename Base, typename quat>
		friend struct IMul;

		inline const quat impl_mul(const quat& y) const noexcept;
		inline const quat impl_inverse() const noexcept;

		template<typename Base, typename quat>
		friend struct IInOut;

		inline std::ostream& impl_out(std::ostream& os) const noexcept;
		inline std::istream& impl_in(std::istream& is) noexcept;
	};

	using quatf = quat<float>;

	// maybe error in editor, but no compile error
	static_assert(sizeof(quatf) == 4 * sizeof(float));
}

#include "details/quat.inl"
