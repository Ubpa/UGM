#pragma once

#include "../../vec.h"
#include "../../point.h"
#include "../../basic.h"

#include <tuple>

namespace Ubpa {
	// unit quatenion
	template<typename Base, typename Impl, typename T>
	struct IQuat : Base, std::tuple<vec<T, 3>, T> {
		static_assert(std::is_floating_point_v<T>, "std::is_floating_point_v<T>");
	private:
		using std::tuple<vec<T, 3>, T>::tuple;
	public:
		using Base::Base;

		IQuat() {}

		IQuat(const point<T, 3>& p) : std::tuple<T, vecf<3>>{ static_cast<T>(0),vec<T, 3>(p) } {}

		template<typename U, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
		IQuat(const vec<T, 3>& imag, U real) : std::tuple<vecf<3>, T>{ imag, static_cast<T>(real) } {}

		vec<T, 3>& imag() noexcept { return std::get<0>(*this); }
		const vec<T, 3>& imag() const noexcept { return std::get<0>(*this); }

		T& real() noexcept { return std::get<1>(*this); }
		T real() const noexcept { return std::get<1>(*this); }

		// radian
		T theta() const noexcept { return static_cast<T>(2) * std::acos(real()); }
		const vec<T, 3> axis() const noexcept {
			assert(!is_identity());
			return imag() / std::sqrt(static_cast<T>(1) - real() * real());
		}

		bool is_identity() const noexcept { return real() == static_cast<T>(1); }
		static const Impl identity() noexcept { return { vec<T,3>{0,0,0}, static_cast<T>(1) }; }

		template<typename U, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
		static const Impl axis_angle(const vec<T, 3>& axis, U theta) {
			Impl rst;
			rst.init(axis, theta);
			return rst;
		}

		template<typename U, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
		static const Impl axis_angle_pro(const vec<T, 3>& normalized_axis, U theta) {
			Impl rst;
			rst.init_pro(axis_angle_pro, theta);
			return rst;
		}

		template<typename U, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
		void init(const vec<T, 3>& axis, U theta) noexcept {
			init_pro(axis.normalize(), static_cast<T>(theta));
		}

		template<typename U, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
		void init_pro(const vec<T, 3>& normalized_axis, U theta) noexcept {
			assert(normalized_axis.is_normalized());
			T halfTheta = static_cast<T>(theta) / static_cast<T>(2);
			real() = std::cos(halfTheta);
			imag() = normalized_axis * std::sin(halfTheta);
		}

		bool is_unit() const noexcept {
			return std::abs(real()*real() + imag().dot(imag()) - static_cast<T>(1)) < EPSILON;
		}

		// rotate with axis x/y/z
		// theta : radian
		template<Axis axis, typename U, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
		static const Impl rotate_with(U theta) noexcept {
			T halfTheta = theta / static_cast<T>(2);

			vec<T, 3> imag;
			constexpr size_t idx = static_cast<size_t>(axis);
			for (size_t i = 0; i < idx; i++)
				imag[i] = static_cast<T>(0);
			imag[idx] = std::sin(halfTheta);
			for (size_t i = idx + 1; i < 3; i++)
				imag[i] = static_cast<T>(0);

			T real = std::cos(halfTheta);

			return { imag, real };
		}
	};
}