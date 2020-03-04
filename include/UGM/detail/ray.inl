#pragma once

namespace Ubpa {
	template<typename T, size_t N>
	void ray<T, N>::print(std::ostream& os) const {
		os << "[ o : " << o << " ]" << std::endl
			<< "[ d : " << d << " ]" << std::endl;
	}

	template<typename T, size_t N>
	std::ostream& ray<T, N>::impl_out(std::ostream& os) const {
		os << o << " " << d;
		return os;
	}

	template<typename T, size_t N>
	std::istream& ray<T, N>::impl_in(std::istream& is) {
		is >> o >> d;
		return is;
	}

	template<typename T, size_t N>
	const std::tuple<bool, val<T, 3>, T> ray<T, N>::intersect_triangle(const point<T, 3>& v0, const point<T, 3>& v1, const point<T, 3>& v2) const {
		static_assert(N == 3);
		const auto e1 = v1 - v0;
		const auto e2 = v2 - v0;

		const auto e1_x_d = e1.cross(d);
		const auto denominator = e1_x_d.dot(e2);

		if (denominator == 0) // parallel
			return { false, val<T, 3>(static_cast<T>(0)), static_cast<T>(0) };

		const auto inv_denominator = static_cast<T>(1) / denominator;

		const auto s = o - v0;

		const auto e2_x_s = e2.cross(s);
		const auto r1 = e2_x_s.dot(d);
		const auto u = r1 * inv_denominator;
		if (u < 0 || u > 1)
			return { false, val<T, 3>(static_cast<T>(0)), static_cast<T>(0) };

		const auto r2 = e1_x_d.dot(s);
		const auto v = r2 * inv_denominator;
		if (v < 0 || v > 1)
			return { false, val<T, 3>(static_cast<T>(0)), static_cast<T>(0) };

		const auto u_plus_v = u + v;
		if (u_plus_v > 1)
			return { false, val<T, 3>(static_cast<T>(0)), static_cast<T>(0) };

		const auto r3 = e2_x_s.dot(e1);
		const auto t = r3 * inv_denominator;

		if (t < 0)
			return { false, val<T, 3>(static_cast<T>(0)), static_cast<T>(0) };

		return { true, val<T, 3>(static_cast<T>(1) - u_plus_v, u, v), t };
	}
}
