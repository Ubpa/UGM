#pragma once

namespace Ubpa {
	template<typename T, size_t N>
	void plane<T, N>::print(std::ostream& os) const {
		os << "[ point  : " << this->point() << " ]" << std::endl
			<< "[ normal : " << this->normal() << " ]" << std::endl;
	}

	template<typename T, size_t N>
	std::ostream& plane<T, N>::impl_out(std::ostream& os) const {
		os << this->point() << " " << this->normal();
		return os;
	}

	template<typename T, size_t N>
	std::istream& plane<T, N>::impl_in(std::istream& is) {
		is >> this->point() >> this->normal();
		return is;
	}
}
