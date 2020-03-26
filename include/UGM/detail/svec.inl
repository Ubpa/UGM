#pragma once

namespace Ubpa {
	template<typename T>
	static const mat<T, 3> svec<T>::TBN(const vec<T, 3>& n, const vec<T, 3>& t) noexcept {
		auto nN = n.normalize();
		auto nT = t.normalize();

		assert(std::abs(nN.dot(nT)) < EPSILON<T>);

		return { nT, nN.cross(nT), nN };
	}

	template<typename T>
	T svec<T>::cos_stheta() const noexcept {
		assert(this->is_normalized());
		return (*this)[2];
	}

	template<typename T>
	T svec<T>::sin_stheta() const noexcept {
		auto c = cos_stheta();
		return 1 - c * c;
	}

	template<typename T>
	T svec<T>::cos_sphi() const noexcept {
		auto s = sin_stheta();
		return s == 0 ? 1 : (*this)[0] / s;
	}

	template<typename T>
	T svec<T>::sin_sphi() const noexcept {
		auto s = sin_stheta();
		return s == 0 ? 0 : (*this)[1] / s;
	}

	template<typename T>
	bool svec<T>::is_above() const noexcept {
		return (*this)[2] > 0;
	}

	template<typename T>
	const svec<T> svec<T>::mid(const svec<T>& x, const svec<T>& y) noexcept {
		assert(x.is_normalized() && y.is_normalized());
		return (x + y).normalize();
	}

	template<typename T>
	const svec<T> svec<T>::mid(const svec<T>& y) const noexcept {
		const auto& x = *static_cast<svec<T>*>(this);
		return mid(x, y);
	}

	template<typename T>
	const svec<T> svec<T>::reflect() const noexcept {
		return { -(*this)[0], -(*this)[1], (*this)[2] };
	}

	template<typename T>
	const std::tuple<bool, svec<T>> svec<T>::refract(T etai, T etao) const noexcept {
		const svec& wi = *this;

		auto sin_sthetai = sin_stheta();
		auto etai_etao = etai / etao;
		auto k = etai_etao * etai_etao * std::abs(sin_sthetai);

		if (k >= 1) // total reflection
			return { false, svec{} };

		auto cos_sthetao = std::sqrt(1 - k);

		// x^2 + y^2 = sin_sthetao
		auto x = -wi[0] * etai_etao;
		auto y = -wi[1] * etai_etao;
		auto z = -sgn(wi[2]) * cos_sthetao;

		svec wo{ x,y,z };
		assert(wo.is_normalized());
		return { true, wo };
	}

	template<typename T>
	const std::tuple<bool, svec<T>> svec<T>::refract(T eta) const noexcept {
		return is_above() ? refract(eta, 1) : refract(1, eta);
	}

	template<typename T>
	const vec<T, 3> operator*(const mat<T, 3>& m, const svec<T>& sv) noexcept {
		static_assert(sv.is_normalized());
		return m * sv.cast_to<vec<T, 3>>();
	}
}
