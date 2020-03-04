#pragma once

#include "euler.h"
#include "point.h"
#include "vec.h"

#include "Interfaces/IArray/IArrayUtil.h"
#include "Interfaces/IMul.h"

namespace Ubpa {
	template<typename T>
	struct euler;

	template<typename T>
	struct quat : SIIT_CRTP<TemplateList<IMul, IArrayUtil>, quat<T>, TypeList<TypeList<T,Size<4>>, T>> {
		using Base = SIIT_CRTP<TemplateList<IMul, IArrayUtil>, quat<T>, TypeList<TypeList<T, Size<4>>, T>>;
		using Base::Base;
		using Base::operator*;

		//template<typename U, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
		//quat(const vec<T, 3>& imag, U real) : Base { imag[0], imag[1], imag[2], static_cast<T>(real) } {}
		static const quat imag_real(const vec<T, 3>& imag, T real) noexcept {
			return { imag[0],imag[1],imag[2],real };
		}

		// quat(const point<T, 3>& p) : quat{ vec<T,3>{p}, 0 } {}

		quat(const vec<T, 3>& axis, T theta) noexcept {
			T halfTheta = static_cast<T>(0.5) * theta;
			real() = std::cos(halfTheta);
			imag() = axis.normalize() * std::sin(halfTheta);
		}

		// axis * sin(theta/2)
		inline vec<T, 3>& imag() noexcept { return *reinterpret_cast<vec<T,3>*>(this); }
		inline const vec<T, 3>& imag() const noexcept { return *reinterpret_cast<const vec<T, 3>*>(this); }

		// cos(thete/2)
		inline T& real() noexcept { return (*this)[3]; }
		inline T real() const noexcept { return (*this)[3]; }

		// radian
		inline T theta() const noexcept { return 2 * std::acos(real()); }
		inline const vec<T, 3> axis() const noexcept {
			assert(!is_identity());
			return imag() / std::sqrt(1 - real() * real());
		}

		inline bool is_identity() const noexcept { return real() == static_cast<T>(1); }
		inline static const quat identity() noexcept { return { 0,0,0,1 }; }

		inline bool is_unit() const noexcept {
			return std::abs(real() * real() + imag().dot(imag()) - 1) < EPSILON;
		}

		// rotate with axis x/y/z
		// theta : radian
		// example: quatf::rotate_with<Axis::X>(to_radian(45.f))
		template<Axis axis>
		inline static const quat rotate_with(T theta) noexcept {
			T halfTheta = static_cast<T>(0.5) * theta;

			vec<T, 3> imag;
			constexpr size_t idx = static_cast<size_t>(axis);
			for (size_t i = 0; i < idx; i++)
				imag[i] = static_cast<T>(0);
			imag[idx] = std::sin(halfTheta);
			for (size_t i = idx + 1; i < 3; i++)
				imag[i] = 0;

			T real = std::cos(halfTheta);

			return imag_real(imag, real);
		}

		const euler<T> to_euler() const noexcept {
			T x = imag()[0];
			T y = imag()[1];
			T z = imag()[2];
			T w = real();

			T sX = 2 * (x * w - y * z);
			if (std::abs(sX) == 1) {
				T half_sY = x * y - z * w;
				T half_cY = y * z + x * w;
				return { std::asin(sX), std::atan2(half_sY, half_cY), 0 };
			}
			else {
				T cXsY = 2 * (x * z + y * w);
				T cXcY = 1 - 2 * (x * x + y * y);
				T cXsZ = 2 * (x * y + z * w);
				T cXcZ = 1 - 2 * (z * z + x * x);
				return{ std::asin(sX), std::atan2(cXsY,cXcY), std::atan2(cXsZ,cXcZ) };
			}
		}

		const point<T, 3> operator*(const point<T, 3>& p) const noexcept {
			// slow
			//return point<T,3>((q * quat(p) * q.inverse()).imag());

			// fast
			auto pV = p.cast_to<vec<T, 3>>();
			T r = real();
			const auto& im = imag();
			return ((r * r - im.norm2()) * pV
				+ 2 * (im.dot(pV) * im + r * im.cross(pV))).cast_to<point<T, 3>>();
		}

	private:
		template<typename Base, typename quat, typename T>
		friend struct IMul;

		const quat impl_mul(const quat& y) const noexcept {
			auto& x = static_cast<const quat&>(*this);
			T real = x.real() * y.real() - x.imag().dot(y.imag());
			vec<T, 3> imag = x.real() * y.imag() + y.real() * x.imag() + x.imag().cross(y.imag());
			return imag_real(imag, real);
		}

		inline const quat impl_inverse() const noexcept {
			return imag_real(-imag(), real());
		}

	private:
		template<typename Base,typename quat,typename ArgList>
		friend struct IInOut;

		std::ostream& impl_out(std::ostream& os) const noexcept {
			os << rmv_epsilon(imag()[0])
				<< " " << rmv_epsilon(imag()[1])
				<< " " << rmv_epsilon(imag()[2])
				<< " " << rmv_epsilon(real());

			return os;
		}

		std::istream& impl_in(std::istream& is) noexcept {
			for (size_t i = 0; i < 3; i++)
				is >> imag()[i];

			is >> real();

			assert(is_unit());

			return is;
		}
	};

	using quatf = quat<float>;
}
