#pragma once

namespace Ubpa {
	template<typename T>
	quat<T> quat<T>::imag_real(const vec<T, 3>& imag, T real) noexcept {
		return { imag[0],imag[1],imag[2],real };
	}

	template<typename T>
	quat<T>::quat(const vec<T, 3>& from, const vec<T, 3>& to) noexcept {
		assert(from.is_normalized() && to.is_normalized());
		auto sin_theta_axis = from.cross(to);
		float cos_theta = from.dot(to);
		float cos_half_theta = std::sqrt(std::max(ZERO<T>, (cos_theta + 1) / 2));
		imag() = sin_theta_axis / (2 * cos_half_theta);
		real() = cos_half_theta;
	}

	template<typename T>
	quat<T>::quat(const vec<T, 3>& axis, T theta) noexcept {
		T halfTheta = static_cast<T>(0.5)* theta;
		real() = std::cos(halfTheta);
		imag() = axis.normalize() * std::sin(halfTheta);
	}

	// axis * sin(theta/2)
	template<typename T>
	vec<T, 3>& quat<T>::imag() noexcept { return *reinterpret_cast<vec<T, 3>*>(this); }

	template<typename T>
	const vec<T, 3>& quat<T>::imag() const noexcept { return *reinterpret_cast<const vec<T, 3>*>(this); }

	// cos(thete/2)
	template<typename T>
	T& quat<T>::real() noexcept { return (*this)[3]; }
	template<typename T>
	T quat<T>::real() const noexcept { return (*this)[3]; }

	// radian
	template<typename T>
	T quat<T>::theta() const noexcept { return 2 * std::acos(real()); }

	template<typename T>
	vec<T, 3> quat<T>::axis() const noexcept {
		assert(!is_identity());
		return imag() / std::sqrt(1 - real() * real());
	}

	template<typename T>
	bool quat<T>::is_identity() const noexcept { return real() == ONE<T>; }

	template<typename T>
	quat<T> quat<T>::identity() noexcept { return { 0,0,0,1 }; }

	template<typename T>
	bool quat<T>::is_unit() const noexcept {
		return std::abs(real() * real() + imag().dot(imag()) - 1) < EPSILON<T>;
	}

	// rotate with axis x/y/z
	// theta : radian
	// example: quatf::rotate_with<Axis::X>(to_radian(45.f))
	template<typename T>
	template<Axis axisIdx>
	quat<T> quat<T>::rotate_with(T theta) noexcept {
		T halfTheta = static_cast<T>(0.5)* theta;

		vec<T, 3> imag;
		constexpr size_t idx = static_cast<size_t>(axisIdx);
		for (size_t i = 0; i < idx; i++)
			imag[i] = ZERO<T>;
		imag[idx] = std::sin(halfTheta);
		for (size_t i = idx + 1; i < 3; i++)
			imag[i] = 0;

		T real = std::cos(halfTheta);

		return imag_real(imag, real);
	}

	template<typename T>
	euler<T> quat<T>::to_euler() const noexcept {
		T x = imag()[0];
		T y = imag()[1];
		T z = imag()[2];
		T w = real();

		T sX = 2 * (x * w - y * z);
		if (std::abs(sX) == 1) {
			T half_sY = x * y - z * w;
			T half_cY = y * z + x * w;
			return {
				to_degree(std::asin(sX)),
				to_degree(std::atan2(half_sY, half_cY)),
				0
			};
		}
		else {
			T cXsY = 2 * (x * z + y * w);
			T cXcY = 1 - 2 * (x * x + y * y);
			T cXsZ = 2 * (x * y + z * w);
			T cXcZ = 1 - 2 * (z * z + x * x);
			return {
				to_degree(std::asin(sX)),
				to_degree(std::atan2(cXsY,cXcY)),
				to_degree(std::atan2(cXsZ,cXcZ))
			};
		}
	}

	template<typename T>
	vec<T, 3> quat<T>::operator*(const vec<T, 3>& v) const noexcept {
		// slow
		//return (q * quat(v) * q.inverse()).imag();

		// fast
		T r = real();
		const auto& im = imag();
		return (r * r - im.norm2()) * v
			+ 2 * (im.dot(v) * im + r * im.cross(v));
	}

	template<typename T>
	quat<T> quat<T>::impl_mul(const quat& y) const noexcept {
		auto& x = static_cast<const quat&>(*this);
		T real = x.real() * y.real() - x.imag().dot(y.imag());
		vec<T, 3> imag = x.real() * y.imag() + y.real() * x.imag() + x.imag().cross(y.imag());
		return imag_real(imag, real);
	}

	template<typename T>
	quat<T> quat<T>::impl_inverse() const noexcept {
		return imag_real(-imag(), real());
	}

	template<typename T>
	std::ostream& quat<T>::impl_out(std::ostream& os) const noexcept {
		os << rmv_epsilon(imag()[0])
			<< " " << rmv_epsilon(imag()[1])
			<< " " << rmv_epsilon(imag()[2])
			<< " " << rmv_epsilon(real());

		return os;
	}

	template<typename T>
	std::istream& quat<T>::impl_in(std::istream& is) noexcept {
		for (size_t i = 0; i < 3; i++)
			is >> imag()[i];

		is >> real();

		assert(is_unit());

		return is;
	}
}
