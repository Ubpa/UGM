#pragma once

namespace Ubpa {
	template<typename F, size_t N>
	bbox<F,N>::bbox() noexcept
		: Base{ point<F, N>{std::numeric_limits<F>::max()},
		point<F, N>{std::numeric_limits<F>::lowest()} } {}

	template<typename F, size_t N>
	bbox<F, N>::bbox(const point<F, N>& minP, const point<F, N>& maxP) noexcept
		: Base{ minP, maxP } {}

	template<typename F, size_t N>
	const bbox<F, N> bbox<F, N>::minmax(const point<F, N>& p0, const point<F, N>& p1) noexcept {
		return { point<F, N>::min(p0, p1), point<F, N>::max(p0, p1) };
	}

	template<typename F, size_t N>
	point<F, N>& bbox<F, N>::minP() noexcept { return (*this)[0]; }

	template<typename F, size_t N>
	const point<F, N>& bbox<F, N>::minP() const noexcept { return (*this)[0]; }

	template<typename F, size_t N>
	point<F, N>& bbox<F, N>::maxP() noexcept { return (*this)[1]; }

	template<typename F, size_t N>
	const point<F, N>& bbox<F, N>::maxP() const noexcept { return (*this)[1]; }

	template<typename F, size_t N>
	bool bbox<F, N>::is_valid() const noexcept {
		for(size_t i = 0; i < N; i++) {
			if(maxP()[i] < minP()[i])
				return false;
		}
		return true;
	}

	template<typename F, size_t N>
	const point<F, N> bbox<F, N>::corner(size_t idx) const noexcept {
		assert(idx < std::pow(2, N));
		point<F, N> rst;
		for (size_t i = 0; i < N; i++) {
			rst[i] = (*this)[idx % 2][i];
			idx >>= 1;
		}
		return rst;
	}

	template<typename F, size_t N>
	const point<F, N> bbox<F, N>::center() const noexcept {
		return point<F, N>::mid(minP(), maxP());
	}

	template<typename F, size_t N>
	const vec<F, N> bbox<F, N>::diagonal() const noexcept { return maxP() - minP(); };

	template<typename F, size_t N>
	const F bbox<F, N>::area() const noexcept {
		static_assert(N == 2 || N == 3);
		if (!is_valid())
			return ZERO<F>;
		const auto d = diagonal();
		if constexpr (N == 2)
			return d[0] * d[1];
		else
			return static_cast<F>(2)* (d[0] * d[1] + d[0] * d[2] + d[1] * d[2]);
	}

	template<typename F, size_t N>
	F bbox<F, N>::volume() const noexcept {
		static_assert(N == 3);
		if (!is_valid())
			return ZERO<F>;
		const auto d = diagonal();
		return d[0] * d[1] * d[2];
	}

	template<typename F, size_t N>
	size_t bbox<F, N>::max_extent() const noexcept {
		const auto d = diagonal();
		return d.max_dim();
	}

	template<typename F, size_t N>
	const point<F, N> bbox<F, N>::lerp(const val<F, N>& t) const noexcept {
		point<F, N> rst{};
		for (size_t i = 0; i < N; i++)
			rst[i] = Ubpa::lerp(minP()[i], maxP()[i], t[i]);
		return rst;
	}

	template<typename F, size_t N>
	const vec<F, N> bbox<F, N>::offset(const point<F, N>& p) const noexcept {
		vec<F, N> o = p - minP();
		const auto d = diagonal();
		for (size_t i = 0; i < N; i++)
			o[i] /= d[i];
		return o;
	}

	template<typename F, size_t N>
	const bbox<F, N> bbox<F, N>::combine(const bbox& rhs) const noexcept {
		return { point<F, N>::min(minP(), rhs.minP()), point<F, N>::max(maxP(), rhs.maxP()) };
	}

	template<typename F, size_t N>
	bbox<F, N>& bbox<F, N>::combine_to_self(const bbox& rhs) noexcept {
		minP() = point<F, N>::min(minP(), rhs.minP());
		maxP() = point<F, N>::max(maxP(), rhs.maxP());
		return *this;
	}

	template<typename F, size_t N>
	const bbox<F, N> bbox<F, N>::combine(const point<F, N>& p) const noexcept {
		return { point<F, N>::min(minP(), p), point<F, N>::max(maxP(), p) };
	}

	template<typename F, size_t N>
	bbox<F, N>& bbox<F, N>::combine_to_self(const point<F, N>& p) noexcept {
		minP() = point<F, N>::min(minP(), p);
		maxP() = point<F, N>::max(maxP(), p);
		return *this;
	}

	template<typename F, size_t N>
	const bbox<F, N> bbox<F, N>::intersect(const bbox& lhs, const bbox& rhs) noexcept {
		return { point<F, N>::max(lhs.minP(), rhs.minP()),point<F, N>::min(lhs.maxP(), rhs.maxP()) };
	}

	template<typename F, size_t N>
	bbox<F, N>& bbox<F, N>::intersect_to_self(const bbox& rhs) noexcept {
		minP() = point<F, N>::max(minP(), rhs.minP());
		maxP() = point<F, N>::min(maxP(), rhs.maxP());
		return *this;
	}
}
