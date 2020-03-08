#pragma once

namespace Ubpa {
	template<typename T, size_t N>
	bbox<T,N>::bbox()
		: Base{ point<T, N>{std::numeric_limits<T>::max()},
		point<T, N>{std::numeric_limits<T>::lowest()} } {}

	template<typename T, size_t N>
	bbox<T, N>::bbox(const point<T, N>& minP, const point<T, N>& maxP)
		: Base{ minP, maxP } {}

	template<typename T, size_t N>
	const bbox<T, N> bbox<T, N>::minmax(const point<T, N>& p0, const point<T, N>& p1) {
		return { point<T, N>::min(p0, p1), point<T, N>::max(p0, p1) };
	}

	template<typename T, size_t N>
	bool bbox<T, N>::is_valid() const {
		for(size_t i = 0; i < N; i++) {
			if(maxP()[i] < minP()[i])
				return false;
		}
		return true;
	}

	template<typename T, size_t N>
	const point<T, N> bbox<T, N>::corner(size_t idx) const {
		assert(idx < std::pow(2, N));
		point<T, N> rst;
		for (size_t i = 0; i < N; i++) {
			rst[i] = (*this)[idx % 2][i];
			idx >>= 1;
		}
		return rst;
	}

	template<typename T, size_t N>
	const point<T, N> bbox<T, N>::center() const {
		return point<T, N>::mid(minP(), maxP());
	}

	template<typename T, size_t N>
	const T bbox<T, N>::area() const {
		static_assert(N == 2 || N == 3);
		if (!is_valid())
			return static_cast<T>(0);
		const auto d = diagonal();
		if constexpr (N == 2)
			return d[0] * d[1];
		else
			return static_cast<T>(2)* (d[0] * d[1] + d[0] * d[2] + d[1] * d[2]);
	}

	template<typename T, size_t N>
	T bbox<T, N>::volume() const {
		static_assert(N == 3);
		if (!is_valid())
			return static_cast<T>(0);
		const auto d = diagonal();
		return d[0] * d[1] * d[2];
	}

	template<typename T, size_t N>
	size_t bbox<T, N>::max_extent() const {
		const auto d = diagonal();
		return d.max_dim();
	}

	template<typename T, size_t N>
	const point<T, N> bbox<T, N>::lerp(const val<T, N>& t) const {
		point<T, N> rst{};
		for (size_t i = 0; i < N; i++)
			rst[i] = Ubpa::lerp(minP()[i], maxP()[i], t[i]);
		return rst;
	}

	template<typename T, size_t N>
	const vec<T, N> bbox<T, N>::offset(const point<T, N>& p) const {
		vec<T, N> o = p - minP();
		const auto d = diagonal();
		for (size_t i = 0; i < N; i++)
			o[i] /= d[i];
		return o;
	}

	template<typename T, size_t N>
	const bbox<T, N> bbox<T, N>::combine(const bbox& rhs) const {
		return { point<T, N>::min(minP(), rhs.minP()), point<T, N>::max(maxP(), rhs.maxP()) };
	}

	template<typename T, size_t N>
	bbox<T, N>& bbox<T, N>::combine_with(const bbox& rhs) {
		minP() = point<T, N>::min(minP(), rhs.minP());
		maxP() = point<T, N>::max(maxP(), rhs.maxP());
		return *this;
	}

	template<typename T, size_t N>
	const bbox<T, N> bbox<T, N>::combine(const point<T, N>& p) const {
		return { point<T, N>::min(minP(), p), point<T, N>::max(maxP(), p) };
	}

	template<typename T, size_t N>
	bbox<T, N>& bbox<T, N>::combine_with(const point<T, N>& p) {
		minP() = point<T, N>::min(minP(), p);
		maxP() = point<T, N>::max(maxP(), p);
		return *this;
	}

	template<typename T, size_t N>
	const bbox<T, N> bbox<T, N>::intersect(const bbox& lhs, const bbox& rhs) {
		return { point<T, N>::max(lhs.minP(), rhs.minP()),point<T, N>::min(lhs.maxP(), rhs.maxP()) };
	}

	template<typename T, size_t N>
	bbox<T, N>& bbox<T, N>::intersect_with(const bbox& rhs) {
		minP() = point<T, N>::max(minP(), rhs.minP());
		maxP() = point<T, N>::min(maxP(), rhs.maxP());
		return *this;
	}
}
