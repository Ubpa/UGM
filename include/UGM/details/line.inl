#pragma once

namespace Ubpa {
	template<typename T, size_t N>
	const line<T, N> line<T, N>::impl_move(const line& line, const point<T, N>& p) noexcept {
		return { p, line.dir };
	}

	template<typename T, size_t N>
	void line<T, N>::print(std::ostream& os) const {
		os << "[ point : " << this->point << " ]" << std::endl
			<< "[  dir  : " << this->dir << " ]" << std::endl;
	}

	template<typename T, size_t N>
	std::ostream& line<T, N>::impl_out(std::ostream& os) const {
		os << this->point << " " << this->dir;
		return os;
	}

	template<typename T, size_t N>
	std::istream& line<T, N>::impl_in(std::istream& is) {
		is >> this->point >> this->dir;
		return is;
	}

	template<typename T, size_t N>
	const std::tuple<bool, std::array<T, 3>, T> line<T, N>::intersect(const triangle<T, 3>& tri) const noexcept {
		static_assert(N == 3);
#ifdef UBPA_UGM_USE_SIMD
		// about 58 instructions
		if constexpr (std::is_same_v<T, float>) {
			vecf4 d{ this->dir.data() };
			pointf4 v0{ tri[0].data() };
			pointf4 v1{ tri[1].data() };
			pointf4 v2{ tri[2].data() };

			const vecf4 e1 = v1 - v0;
			const vecf4 e2 = v2 - v0;

			const vecf4 e1_x_d = e1.v3_cross(d);
			const float denominator = e1_x_d.v3_dot(e2).get<0>();

			if (denominator == 0) // parallel
				return { false, std::array<T, 3>{ZERO<T>}, ZERO<T> };

			const float inv_denominator = ONE<T> / denominator;

			pointf4 p{ this->point.data() };
			const vecf4 s = p - v0;

			const vecf4 e2_x_s = e2.v3_cross(s);
			const float r1 = e2_x_s.v3_dot(d).get<0>();
			const float u = r1 * inv_denominator;
			if (u < 0 || u > 1)
				return { false, std::array<T, 3>{ZERO<T>}, ZERO<T> };

			const float r2 = e1_x_d.v3_dot(s).get<0>();
			const float v = r2 * inv_denominator;
			if (v < 0 || v > 1)
				return { false, std::array<T, 3>{ZERO<T>}, ZERO<T> };

			const float u_plus_v = u + v;
			if (u_plus_v > 1)
				return { false, std::array<T, 3>{ZERO<T>}, ZERO<T> };

			const float r3 = e2_x_s.v3_dot(e1).get<0>();
			const float t = r3 * inv_denominator;

			return { true, std::array<T, 3>{ONE<T> -u_plus_v, u, v}, t };
		}
		else
#endif // UBPA_UGM_USE_SIMD
		{// about 103 instructions
			const auto& p = this->point;
			const auto& d = this->dir;
			const auto& v0 = tri[0];
			const auto& v1 = tri[1];
			const auto& v2 = tri[2];

			const auto e1 = v1 - v0;
			const auto e2 = v2 - v0;

			const auto e1_x_d = e1.cross(d);
			const auto denominator = e1_x_d.dot(e2);

			if (denominator == 0) // parallel
				return { false, std::array<T, 3>{ZERO<T>}, ZERO<T> };

			const auto inv_denominator = ONE<T> / denominator;

			const auto s = p - v0;

			const auto e2_x_s = e2.cross(s);
			const auto r1 = e2_x_s.dot(d);
			const auto u = r1 * inv_denominator;
			if (u < 0 || u > 1)
				return { false, std::array<T, 3>{ZERO<T>}, ZERO<T> };

			const auto r2 = e1_x_d.dot(s);
			const auto v = r2 * inv_denominator;
			if (v < 0 || v > 1)
				return { false, std::array<T, 3>{ZERO<T>}, ZERO<T> };

			const auto u_plus_v = u + v;
			if (u_plus_v > 1)
				return { false, std::array<T, 3>{ZERO<T>}, ZERO<T> };

			const auto r3 = e2_x_s.dot(e1);
			const auto t = r3 * inv_denominator;

			return { true, std::array<T, 3>{ONE<T> -u_plus_v, u, v}, t };
		}
	}

	template<typename T, size_t N>
	const std::tuple<bool, T, T> line<T, N>::intersect(const bbox<T, N>& box, T tmin, T tmax) const noexcept {
		return intersect(box, this->inv_dir(), tmin, tmax);
	}

	template<typename T, size_t N>
	const std::tuple<bool, T, T> line<T, N>::intersect(const bbox<T, N>& box, const vec<T, N>& invdir, T tmin, T tmax) const noexcept
	{
#ifdef UBPA_UGM_USE_SIMD
		if constexpr (std::is_same_v<T, float> && N == 3) {
			// 26 instructions, no loop
			auto sorigin = _mm_loadu_ps(this->point.data());
			auto sboxminP = _mm_loadu_ps(box.minP().data());
			auto sboxmaxP = _mm_loadu_ps(box.maxP().data());
			auto sinvdir = _mm_loadu_ps(invdir.data());
			auto sd0 = _mm_mul_ps(_mm_sub_ps(sboxminP, sorigin), sinvdir);
			auto sd1 = _mm_mul_ps(_mm_sub_ps(sboxmaxP, sorigin), sinvdir);
			sd0 = VecSwizzle(sd0, 0, 0, 1, 2);
			sd1 = VecSwizzle(sd1, 0, 0, 1, 2);
			sd0 = _mm_move_ss(sd0, _mm_set_ps1(tmin));
			sd1 = _mm_move_ss(sd1, _mm_set_ps1(tmax));
			const vecf4 stmin = _mm_min_ps(sd0, sd1); // const
			const vecf4 stmax = _mm_max_ps(sd0, sd1); // const
			float t0 = stmin.max_component(); // 5 instructions
			float t1 = stmax.min_component(); // 5 instructions
			return { t0 < t1, t0, t1 };
		}
		else
#endif
		{
			/*const auto& origin = this->point;
			const auto& boxminP = box.minP();
			const auto& boxmaxP = box.maxP();

			for (size_t i = 0; i < N; i++) {
				T t0 = (boxminP[i] - origin[i]) * inv_dir[i];
				T t1 = (boxmaxP[i] - origin[i]) * inv_dir[i];
				if (inv_dir[i] < 0)
					std::swap(t0, t1);

				tmin = std::max(t0, tmin);
				tmax = std::min(t1, tmax);
				if (tmax < tmin)
					return { false, ZERO<T>, ZERO<T> };
			}*/
		}

		return { true, tmin, tmax };
	}
}
