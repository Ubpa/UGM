#pragma once

namespace Ubpa {
	template<typename T, size_t N>
	line<T, N> ray<T, N>::to_line() const noexcept {
		return { this->point, this->dir };
	}

	template<typename T, size_t N>
	ray<T, N> ray<T, N>::impl_move(const ray& r, const point<T, N>& p) noexcept {
		return { p, r.dir, r.tmin, r.tmax };
	}

	template<typename T, size_t N>
	void ray<T, N>::print(std::ostream& os) const {
		os
			<< "[ point : " << this->point << " ]" << std::endl
			<< "[ dir   : " << this->dir << " ]" << std::endl
			<< "[ tmin  : " << this->tmin << " ]" << std::endl
			<< "[ tmax  : " << this->tmin << " ]" << std::endl;
	}

	template<typename T, size_t N>
	std::ostream& ray<T, N>::impl_out(std::ostream& os) const {
		os << this->point << " " << this->dir << " " << tmin << " " << tmax;
		return os;
	}

	template<typename T, size_t N>
	std::istream& ray<T, N>::impl_in(std::istream& is) {
		is >> this->point >> this->dir >> tmin >> tmax;
		return is;
	}

	template<typename T, size_t N>
	std::tuple<bool, std::array<T, 3>, T> ray<T, N>::intersect(const triangle<T, 3>& tri) const noexcept {
		static_assert(N == 3);

		auto rst = to_line().intersect(tri);

		if (std::get<0>(rst) && std::get<2>(rst) > tmin && std::get<2>(rst) < tmax)
			return rst;
		else
			return { false, std::array<T, 3>{ZERO<T>}, ZERO<T> };
	}

	template<typename T, size_t N>
	std::tuple<bool, T, T> ray<T, N>::intersect(const bbox<T, N>& box) const noexcept {
		return to_line().intersect(box, tmin, tmax);
	}

	template<typename T, size_t N>
	std::tuple<bool, T> ray<T, N>::intersect_std_sphere() const noexcept {
		constexpr auto r = static_cast<T>(1);
		constexpr auto r2 = pow2(r);
		const auto& p = this->point;
		const auto& d = this->dir;
#ifdef UBPA_UGM_USE_SIMD
		// 36 instructions
		if constexpr (std::is_same_v<T, float> && N == 3) {
			vecf4 oc = _mm_loadu_ps(p.data());
			vecf4 dir = _mm_loadu_ps(d.data());
			T a = dir.v3_norm2();
			T b = oc.v3_dot(dir);
			T c = oc.v3_dot(oc) - r2;
			T discriminant = b * b - a * c;

			if (discriminant < 0)
				return { false, ZERO<T> };

			T sqrt_discriminant = std::sqrt(discriminant);
			T inv_a = 1 / a;

			T neg_b = -b;
			T t = (neg_b - sqrt_discriminant) * inv_a;
			if (t > tmax || t < tmin) {
				t = (neg_b + sqrt_discriminant) * inv_a;
				if (t > tmax || t < tmin)
					return { false, ZERO<T> };
			}

			return { true, t };
		}
		else
#endif // UBPA_UGM_USE_SIMD
		// 51 instructions
		{
			vec<T, N> oc = p.template cast_to<vec<T, N>>();
			T a = d.norm2();
			T b = oc.dot(d);
			T c = oc.dot(oc) - r2;
			T discriminant = b * b - a * c;

			if (discriminant < 0)
				return { false, ZERO<T> };

			T sqrt_discriminant = std::sqrt(discriminant);
			T inv_a = 1 / a;

			T neg_b = -b;
			T t = (neg_b - sqrt_discriminant) * inv_a;
			if (t > tmax || t < tmin) {
				t = (neg_b + sqrt_discriminant) * inv_a;
				if (t > tmax || t < tmin)
					return { false, ZERO<T> };
			}

			return { true, t };
		}
	}

	template<typename T, size_t N>
	std::tuple<bool, T> ray<T, N>::intersect_sphere(const point<T, 3>& center, T radius) const noexcept {
		const T r2 = pow2(radius);
		const auto& p = this->point;
		const auto& d = this->dir;
#ifdef UBPA_UGM_USE_SIMD
		if constexpr (std::is_same_v<T, float> && N == 3) {
			vecf4 oc = _mm_loadu_ps((p - center).data());
			vecf4 dir = _mm_loadu_ps(d.data());
			T a = dir.v3_norm2();
			T b = oc.v3_dot(dir);
			T c = oc.v3_dot(oc) - r2;
			T discriminant = b * b - a * c;

			if (discriminant < 0)
				return { false, ZERO<T> };

			T sqrt_discriminant = std::sqrt(discriminant);
			T inv_a = 1 / a;

			T neg_b = -b;
			T t = (neg_b - sqrt_discriminant) * inv_a;
			if (t > tmax || t < tmin) {
				t = (neg_b + sqrt_discriminant) * inv_a;
				if (t > tmax || t < tmin)
					return { false, ZERO<T> };
			}

			return { true, t };
		}
		else
#endif // UBPA_UGM_USE_SIMD
		{
			vec<T, N> oc = p - center;
			T a = d.norm2();
			T b = oc.dot(d);
			T c = oc.dot(oc) - r2;
			T discriminant = b * b - a * c;

			if (discriminant < 0)
				return { false, ZERO<T> };

			T sqrt_discriminant = std::sqrt(discriminant);
			T inv_a = 1 / a;

			T neg_b = -b;
			T t = (neg_b - sqrt_discriminant) * inv_a;
			if (t > tmax || t < tmin) {
				t = (neg_b + sqrt_discriminant) * inv_a;
				if (t > tmax || t < tmin)
					return { false, ZERO<T> };
			}

			return { true, t };
		}
	}

	template<typename T, size_t N>
	std::tuple<bool, T, point<T, 2>> ray<T, N>::intersect_std_square() const noexcept {
		static_assert(N == 3);

		const auto& d = this->dir;
		if (d[1] == 0)
			return { false, ZERO<T>, point<T,2>{0,0} };

		const auto& p = this->point;
		T t = -p[1] / d[1];
		if (t < tmin || t > tmax)
			return { false, ZERO<T>, point<T,2>{0,0} };

		auto x = p[0] + t * d[0];
		if (x < -1 || x > 1)
			return { false, ZERO<T>, point<T,2>{0,0} };

		auto z = p[2] + t * d[2];
		if (z < -1 || z > 1)
			return { false, ZERO<T>, point<T,2>{0,0} };

		return { true, t, point<T,2>{x,z} };
	}

	template<typename T, size_t N>
	std::tuple<bool, T, point<T, 2>> ray<T, N>::intersect_std_disk() const noexcept {
		static_assert(N == 3);

		const auto& d = this->dir;
		if (d[1] == 0)
			return { false, ZERO<T>, point<T,2>{0,0} };

		const auto& p = this->point;
		T t = -p[1] / d[1];
		if (t < tmin || t > tmax)
			return { false, ZERO<T>, point<T,2>{0,0} };

		auto x = p[0] + t * d[0];
		auto z = p[2] + t * d[2];

		if (x * x + z * z > 1)
			return { false, ZERO<T>, point<T,2>{0,0} };

		return { true, t, point<T,2>{x,z} };
	}
}
