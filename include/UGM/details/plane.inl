#pragma once

namespace Ubpa {
	template<typename T>
	void plane<T>::print(std::ostream& os) const {
		os << "[ point  : " << this->point << " ]" << std::endl
			<< "[ normal : " << this->normal << " ]" << std::endl;
	}

	template<typename T>
	std::ostream& plane<T>::impl_out(std::ostream& os) const {
		os << this->point << " " << this->normal;
		return os;
	}

	template<typename T>
	std::istream& plane<T>::impl_in(std::istream& is) {
		is >> this->point >> this->normal;
		return is;
	}
}
