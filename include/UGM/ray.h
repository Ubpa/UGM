#pragma once

#include "point.h"
#include "vec.h"

#include <array>

#include "Interfaces/IArray/IEuclideanLine.h"

namespace Ubpa {
	template<typename T, size_t N>
	struct ray : SIIT_CRTP<TemplateList<IInOut, IEuclideanLine>, ray<T, N>, TypeList<TypeList<T, Size<N>>, T, vec<T, N>, point<T, N>>> {
		using Base = SIIT_CRTP<TemplateList<IInOut, IEuclideanLine>, ray<T, N>, TypeList<TypeList<T, Size<N>>, T, vec<T, N>, point<T, N>>>;

		T tmin;
		T tmax;

		ray(const point<T, N>& o, const vec<T, N>& d,
			T tmin = EPSILON<T>, T tmax = std::numeric_limits<T>::max())
			: Base{ o, d }, tmin{ tmin }, tmax{ tmax } {}

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
	using rayf = ray<float, N>;
	using rayf1 = rayf<1>;
	using rayf2 = rayf<2>;
	using rayf3 = rayf<3>;
}

#include "detail/ray.inl"
