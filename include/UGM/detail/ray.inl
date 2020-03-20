#pragma once

#include "../line.h"

namespace Ubpa {
	template<typename T, size_t N>
	void ray<T, N>::print(std::ostream& os) const {
		os
			<< "[ origin : " << this->origin() << " ]" << std::endl
			<< "[ dir    : " << this->dir() << " ]" << std::endl
			<< "[ tmin   : " << this->tmin << " ]" << std::endl
			<< "[ tmax   : " << this->tmin << " ]" << std::endl;
	}

	template<typename T, size_t N>
	std::ostream& ray<T, N>::impl_out(std::ostream& os) const {
		os << this->origin() << " " << this->dir() << " " << tmin << " " << tmax;
		return os;
	}

	template<typename T, size_t N>
	std::istream& ray<T, N>::impl_in(std::istream& is) {
		is >> this->origin() >> this->dir() >> tmin >> tmax;
		return is;
	}

	template<typename T, size_t N>
	const std::tuple<bool, std::array<T, 3>, T> ray<T, N>::intersect_triangle(const point<T, 3>& v0, const point<T, 3>& v1, const point<T, 3>& v2) const {
		static_assert(N == 3);

		const auto& o = this->origin();
		const auto& d = this->dir();

		auto rst = line<T, 3>(o, d).intersect_triangle(v0, v1, v2);
		const auto& [isIntersect, wuv, t] = rst;

		if (isIntersect && t > tmin&& t < tmax)
			return rst;
		else
			return { false, std::array<T, 3>{static_cast<T>(0)}, static_cast<T>(0) };
	}
}
