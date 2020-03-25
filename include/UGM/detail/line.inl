#pragma once

namespace Ubpa {
	template<typename T, size_t N>
	const line<T, N> line<T, N>::impl_move(const line& line, const point<T, N>& p) noexcept {
		return { p, line.dir() };
	}

	template<typename T, size_t N>
	void line<T, N>::print(std::ostream& os) const {
		os << "[ point : " << this->point() << " ]" << std::endl
			<< "[  dir  : " << this->dir() << " ]" << std::endl;
	}

	template<typename T, size_t N>
	std::ostream& line<T, N>::impl_out(std::ostream& os) const {
		os << this->point() << " " << this->dir();
		return os;
	}

	template<typename T, size_t N>
	std::istream& line<T, N>::impl_in(std::istream& is) {
		is >> this->point() >> this->dir();
		return is;
	}

	template<typename T, size_t N>
	const std::tuple<bool, std::array<T, 3>, T> line<T, N>::intersect(const triangle<T, 3>& tri) const noexcept {
		static_assert(N == 3);

		const auto& p = this->point();
		const auto& d = this->dir();
		const auto& v0 = tri[0];
		const auto& v1 = tri[0];
		const auto& v2 = tri[0];

		const auto e1 = v1 - v0;
		const auto e2 = v2 - v0;

		const auto e1_x_d = e1.cross(d);
		const auto denominator = e1_x_d.dot(e2);

		if (denominator == 0) // parallel
			return { false, std::array<T, 3>{static_cast<T>(0)}, static_cast<T>(0) };

		const auto inv_denominator = static_cast<T>(1) / denominator;

		const auto s = p - v0;

		const auto e2_x_s = e2.cross(s);
		const auto r1 = e2_x_s.dot(d);
		const auto u = r1 * inv_denominator;
		if (u < 0 || u > 1)
			return { false, std::array<T, 3>{static_cast<T>(0)}, static_cast<T>(0) };

		const auto r2 = e1_x_d.dot(s);
		const auto v = r2 * inv_denominator;
		if (v < 0 || v > 1)
			return { false, std::array<T, 3>{static_cast<T>(0)}, static_cast<T>(0) };

		const auto u_plus_v = u + v;
		if (u_plus_v > 1)
			return { false, std::array<T, 3>{static_cast<T>(0)}, static_cast<T>(0) };

		const auto r3 = e2_x_s.dot(e1);
		const auto t = r3 * inv_denominator;

		if (t < 0)
			return { false, std::array<T, 3>{static_cast<T>(0)}, static_cast<T>(0) };

		return { true, std::array<T, 3>{static_cast<T>(1) - u_plus_v, u, v}, t };
	}

	template<typename T, size_t N>
	const std::tuple<bool, T, T> line<T, N>::intersect(const bbox<T, N>& box, T tmin, T tmax) const noexcept {
		const auto& origin = this->point();
		const auto& dir = this->dir();
		const auto& boxminP = box.minP();
		const auto& boxmaxP = box.maxP();

		for (int i = 0; i < N; i++) {
			T invD = 1 / dir[i];
			T t0 = (boxminP[i] - origin[i]) * invD;
			T t1 = (boxmaxP[i] - origin[i]) * invD;
			if (invD < 0)
				std::swap(t0, t1);

			tmin = std::max(t0, tmin);
			tmax = std::min(t1, tmax);
			if (tmax < tmin)
				return { false, 0, 0 };
		}

		return { true, tmin, tmax };
	}
}
