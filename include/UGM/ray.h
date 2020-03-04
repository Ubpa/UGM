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

		void print(std::ostream& os = std::cout) const;

		// (isIntersect, (w, u, v), t)
		const std::tuple<bool, val<T,3>, T> intersect_triangle(const point<T,3>& v0, const point<T,3>& v1, const point<T,3>& v2) const;

	private:
		template<typename Base, typename Impl, typename ArgList>
		friend struct IInOut;
		std::ostream& impl_out(std::ostream& os) const;
		std::istream& impl_in(std::istream& is);
	};

	template<size_t N>
	using rayf = ray<float, N>;
	using rayf2 = rayf<2>;
	using rayf3 = rayf<3>;
}

#include "detail/ray.inl"
