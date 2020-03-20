#pragma once

#include "point.h"
#include "vec.h"

#include <array>

#include "Interfaces/IArray/IEuclideanA.h"

namespace Ubpa {
	template<typename T, size_t N>
	struct plane : SIIT_CRTP<TemplateList<IInOut, IEuclideanA>, plane<T, N>, TypeList<TypeList<T, Size<N>>, T, vec<T, N>, point<T, N>>> {
		using Base = SIIT_CRTP<TemplateList<IInOut, IEuclideanA>, plane<T, N>, TypeList<TypeList<T, Size<N>>, T, vec<T, N>, point<T, N>>>;
		using Base::Base;

		plane(const point<T, N>& o, const vec<T, N>& d) : Base{ o, d } {}

		void print(std::ostream& os = std::cout) const;

		// (isIntersect, (w, u, v), t)
		const std::tuple<bool, std::array<T, 3>, T> intersect_triangle(const point<T, 3>& v0, const point<T, 3>& v1, const point<T, 3>& v2) const;

	private:
		template<typename Base, typename Impl, typename ArgList>
		friend struct IInOut;
		std::ostream& impl_out(std::ostream& os) const;
		std::istream& impl_in(std::istream& is);
	};

	template<size_t N>
	using planef = plane<float, N>;
	using planef1 = planef<1>;
	using planef2 = planef<2>;
	using planef3 = planef<3>;
}

#include "detail/plane.inl"
