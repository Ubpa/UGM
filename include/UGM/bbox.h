#pragma once

#include "point.h"
#include "vec.h"

#include <limits>

namespace Ubpa {
	template<typename T, size_t N>
	struct bbox : SIIT_CRTP<TemplateList<IArrayUtil>, bbox<T, N>, TypeList<TypeList<point<T, N>, Size<2>>, T>> {
		using Base = SIIT_CRTP<TemplateList<IArrayUtil>, bbox<T, N>, TypeList<TypeList<point<T, N>, Size<2>>, T>>;
		using Base::Base;
		
		bbox()
			: Base{ point<T, N>{std::numeric_limits<T>::max()},
			point<T, N>{std::numeric_limits<T>::lowest()} } {}

		bbox(const point<T, N>& minP, const point<T, N>& maxP)
			: Base{minP, maxP} {}

		static const bbox minmax(const point<T, N>& p0, const point<T, N>& p1) {
			return { point<T, N>::min(p0, p1), point<T, N>::max(p0, p1) };
		}

		point<T, N>& minP() noexcept { return (*this)[0]; }
		const point<T, N>& minP() const noexcept { return (*this)[0]; }
		point<T, N>& maxP() noexcept { return (*this)[1]; }
		const point<T, N>& maxP() const noexcept { return (*this)[1]; }

		bool is_valid() const { return maxP()[0] >= minP()[0] && maxP()[1] >= minP()[1] && maxP()[2] >= minP()[2]; }

		const point<T, N> corner(size_t idx) const {
			assert(idx < std::pow(2, N));
			point<T, N> rst;
			for (size_t i = 0; i < N; i++) {
				rst[i] = (*this)[idx % 2][i];
				idx >>= 1;
			}
			return rst;
		}

		const point<T, N> center() const {
			return point<T, N>::mid(minP(), maxP());
		}

		const vec<T, N> diagonal() const { return maxP() - minP(); }

		const T area() const {
			static_assert(N == 2 || N == 3);
			if (!is_valid())
				return static_cast<T>(0);
			const auto d = diagonal();
			if constexpr (N == 2)
				return d[0] * d[1];
			else
				return static_cast<T>(2)* (d[0] * d[1] + d[0] * d[2] + d[1] * d[2]);
		}

		T volume() const {
			static_assert(N == 3);
			if (!is_valid())
				return static_cast<T>(0);
			const auto d = diagonal();
			return d[0] * d[1] * d[2];
		}

		size_t max_extent() const {
			const auto d = diagonal();
			return d.max_dim();
		}

		using Base::lerp;
		const point<T, N> lerp(const val<T, N>& t) {
			point<T, N> rst{};
			for (size_t i = 0; i < N; i++)
				rst[i] = Ubpa::lerp(minP()[i], maxP()[i], t[i]);
			return rst;
		}

		const vec<T, N> offset(const point<T, N>& p) const {
			vec<T, N> o = p - minP();
			const auto d = diagonal();
			for (size_t i = 0; i < N; i++)
				o[i] /= d[i];
			return o;
		}

		const bbox combine(const bbox& rhs) const {
			return { point<T, N>::min(minP(), rhs.minP()), point<T, N>::max(maxP(), rhs.maxP()) };
		}

		bbox& combine_with(const bbox& rhs) {
			minP() = point<T, N>::min(minP(), rhs.minP());
			maxP() = point<T, N>::max(maxP(), rhs.maxP());
			return *this;
		}

		const bbox combine(const point<T, N>& p) const {
			return { point<T, N>::min(minP(), p), point<T, N>::max(maxP(), p) };
		}

		bbox& combine_with(const point<T, N>& p) {
			minP() = point<T, N>::min(minP(), p);
			maxP() = point<T, N>::max(maxP(), p);
			return *this;
		}

		static const bbox intersect(const bbox& lhs, const bbox& rhs) {
			return { point<T, N>::max(lhs.minP(), rhs.minP()),point<T, N>::min(lhs.maxP(), rhs.maxP()) };
		}

		bbox& intersect_with(const bbox& rhs) {
			minP() = point<T, N>::max(minP(), rhs.minP());
			maxP() = point<T, N>::min(maxP(), rhs.maxP());
			return *this;
		}
	};

	template<size_t N>
	using bboxf = bbox<float, N>;

	using bboxf1 = bboxf<1>;
	using bboxf2 = bboxf<2>;
	using bboxf3 = bboxf<3>;
	using bboxf4 = bboxf<4>;
}
