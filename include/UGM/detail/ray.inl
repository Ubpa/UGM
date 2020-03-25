#pragma once

namespace Ubpa {
	template<typename T, size_t N>
	const line<T, N> ray<T, N>::to_line() const noexcept {
		return { this->point(), this->dir() };
	}

	template<typename T, size_t N>
	void ray<T, N>::print(std::ostream& os) const {
		os
			<< "[ point : " << this->point() << " ]" << std::endl
			<< "[ dir   : " << this->dir() << " ]" << std::endl
			<< "[ tmin  : " << this->tmin << " ]" << std::endl
			<< "[ tmax  : " << this->tmin << " ]" << std::endl;
	}

	template<typename T, size_t N>
	std::ostream& ray<T, N>::impl_out(std::ostream& os) const {
		os << this->point() << " " << this->dir() << " " << tmin << " " << tmax;
		return os;
	}

	template<typename T, size_t N>
	std::istream& ray<T, N>::impl_in(std::istream& is) {
		is >> this->point() >> this->dir() >> tmin >> tmax;
		return is;
	}

	template<typename T, size_t N>
	const std::tuple<bool, std::array<T, 3>, T> ray<T, N>::intersect(const triangle<T, 3>& tri) const noexcept {
		static_assert(N == 3);

		auto rst = to_line().intersect(tri);
		const auto& [isIntersect, wuv, t] = rst;

		if (isIntersect && t > tmin && t < tmax)
			return rst;
		else
			return { false, std::array<T, 3>{static_cast<T>(0)}, static_cast<T>(0) };
	}

	template<typename T, size_t N>
	const std::tuple<bool, T, T> ray<T, N>::intersect(const bbox<T, N>& box) const noexcept {
		return to_line().intersect(box, tmin, tmax);
	}
}
