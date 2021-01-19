#pragma once

#include "point.h"
#include "euler.h"

#include "Interfaces/IArray/IArrayUtil.h"
#include "Interfaces/IMul.h"

namespace Ubpa {
	template<typename T>
	struct SI_ImplTraits<quat<T>>
		: Array1DTraits<T, 4>,
		SIMDTraits<false>, // float4 not use SIMD
		IListTraits<IMul, IArrayUtil>
		{};

	template<typename T>
	struct quat : SI<quat<T>> {
		using Base = SI<quat<T>>;
		using Base::Base;
		using Base::operator*;

		static quat imag_real(const vec<T, 3>& imag, T real) noexcept;

		quat(const vec<T, 3>& axis, T theta) noexcept;

		// from and to is normalized
		quat(const vec<T, 3>& from, const vec<T, 3>& to) noexcept;

		// axis * sin(theta/2)
		vec<T, 3>& imag() noexcept;
		const vec<T, 3>& imag() const noexcept;

		// cos(thete/2)
		T& real() noexcept;
		T real() const noexcept;

		// radian
		T theta() const noexcept;
		vec<T, 3> axis() const noexcept;

		bool is_identity() const noexcept;
		static quat identity() noexcept;

		bool is_unit() const noexcept;

		// rotate with axis x/y/z
		// theta : radian
		// example: quatf::rotate_with<Axis::X>(to_radian(45.f))
		template<Axis axis>
		static quat rotate_with(T theta) noexcept;

		euler<T> to_euler() const noexcept;

		vec<T, 3> operator*(const vec<T, 3>& v) const noexcept;

	private:
		template<typename Base, typename quat>
		friend struct IMul;

		quat impl_mul(const quat& y) const noexcept;
		quat impl_inverse() const noexcept;

		template<typename Base, typename quat>
		friend struct IInOut;

		std::ostream& impl_out(std::ostream& os) const noexcept;
		std::istream& impl_in(std::istream& is) noexcept;
	};

	using quatf = quat<float>;

	// maybe error in editor, but no compile error
	static_assert(sizeof(quatf) == 4 * sizeof(float));
}

#include "details/quat.inl"
