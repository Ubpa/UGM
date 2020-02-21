#pragma once

#include "../../vec.h"
#include "../../point.h"

namespace Ubpa {
	// unit quatenion
	template<typename Base, typename Impl, typename ArgList>
	struct IQuat : Base {
		using T = Front_t<ArgList>;
		static_assert(std::is_floating_point_v<T>, "std::is_floating_point_v<T>");
	private:
		static constexpr auto EPSILON = static_cast<T>(0.000001);
	public:
		using Base::Base;

		IQuat() {}

		IQuat(const point<T, 3>& p) : real(static_cast<T>(0)), imag(p[0], p[1], p[2]) {}

		template<typename U, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
		IQuat(U real, const vec<T, 3>& imag) : real(static_cast<T>(real)), imag(imag) {}

		template<typename U, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
		IQuat(const vec<T,3>& axis, U theta) {
			init(axis, theta);
		}

		template<typename U, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
		void init(const vec<T, 3>& axis, U theta) noexcept {
			init_pro(axis.normalize(), static_cast<T>(theta));
		}

		template<typename U, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
		void init_pro(const vec<T, 3>& normalized_axis, U theta) noexcept {
			assert((normalized_axis.normalize() - normalized_axis).norm() < EPSILON);
			T halfTheta = static_cast<T>(theta) / static_cast<T>(2);
			real = std::cos(halfTheta);
			imag = normalized_axis * std::sin(halfTheta);
		}

		bool is_unit() const noexcept {
			return std::abs(real*real + imag.dot(imag) - static_cast<T>(1)) < EPSILON;
		}

		T real;
		vec<T,3> imag;
	};
}