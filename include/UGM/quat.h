#pragma once

#include "euler.h"
#include "point.h"

#include "Interfaces/IArray/IArrayUtil.h"
#include "Interfaces/IMul.h"

namespace Ubpa {
	template<typename T>
	struct quat : SI<TemplateList<IMul, IArrayUtil>, quat<T>, TypeList<T, Size<4>>, T> {
		using Base = SI<TemplateList<IMul, IArrayUtil>, quat<T>, TypeList<T, Size<4>>, T>;
		using Base::Base;
		using Base::operator*;

		inline static const quat imag_real(const vec<T, 3>& imag, T real) noexcept;

		inline quat(const vec<T, 3>& axis, T theta) noexcept;

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

		inline const point<T, 3> operator*(const point<T, 3>& p) const noexcept;

	private:
		template<typename Base, typename quat, typename T>
		friend struct IMul;

		inline const quat impl_mul(const quat& y) const noexcept;
		inline const quat impl_inverse() const noexcept;

		template<typename Base,typename quat,typename ArgList>
		friend struct IInOut;

		inline std::ostream& impl_out(std::ostream& os) const noexcept;
		inline std::istream& impl_in(std::istream& is) noexcept;
	};

	using quatf = quat<float>;

	// maybe error in editor, but no compile error
	static_assert(sizeof(quatf) == 4 * sizeof(float));
}

#include "detail/quat.inl"
