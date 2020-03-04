#pragma once

#include "val.h"
#include "point.h"
#include "vec.h"

namespace Ubpa {
	template<typename T, size_t N>
	struct ray : SIIT_CRTP<TemplateList<IInOut>, ray<T,N>, TypeList<T,Size<N>>> {
		using SIIT_CRTP<TemplateList<IInOut>, ray<T, N>, TypeList<T, Size<N>>>::SIIT_CRTP;

		point<T, N> o;
		vec<T, N> d;

		ray(const point<T, N>& o, const vec<T, N>& d) : o(o), d(d) {}

		const point<T, N> operator()(T t) const noexcept { return o + t * d; }
		const point<T, N> at(T t) const noexcept { return o + t * d; }

		void print(std::ostream& os = std::cout) const {
			os << "[ o : " << o << " ]" << std::endl
				<< "[ d : " << d << " ]" << std::endl;
		}

		// isIntersect, [w, u, v], t
		const std::tuple<bool, val<T,3>, T> intersect_triangle(const point<T,3>& v0, const point<T,3>& v1, const point<T,3>& v2) const;

	private:
		template<typename Base, typename Impl, typename ArgList>
		friend struct IInOut;

		std::ostream& impl_out(std::ostream& os) const {
			os << o << " " << d;
			return os;
		}

		std::istream& impl_in(std::istream& is) {
			is >> o >> d;
			return is;
		}
	};

	template<size_t N>
	using rayf = ray<float, N>;
	using rayf2 = rayf<2>;
	using rayf3 = rayf<3>;

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
