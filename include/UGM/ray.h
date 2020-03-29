#pragma once

#include "line.h"

namespace Ubpa {
	template<typename T, size_t N>
	struct ray : SIIT_CRTP<TemplateList<IInOut, ILine>, ray<T, N>, TypeList<Arg_Empty, T, vec<T, N>, point<T, N>>> {
		using Base = SIIT_CRTP<TemplateList<IInOut, ILine>, ray<T, N>, TypeList<Arg_Empty, T, vec<T, N>, point<T, N>>>;

		T tmin;
		T tmax;

		ray(const point<T, N>& p, const vec<T, N>& dir,
			T tmin = EPSILON<T>, T tmax = std::numeric_limits<T>::max()) noexcept
			: tmin{ tmin }, tmax{ tmax } { this->init_ILine(p, dir); }

		const line<T, N> to_line() const noexcept;

		void print(std::ostream& os = std::cout) const;

		// (isIntersect, (w, u, v), t)
		const std::tuple<bool, std::array<T, 3>, T> intersect(const triangle<T, 3>& tri) const noexcept;
		// (isIntersect, t0, t1)
		const std::tuple<bool, T, T> intersect(const bbox<T, N>& box) const noexcept;

		// intersect with standard sphere (center: [0, 0, 0], radius: 1)
		// (isIntersect, t)
		const std::tuple<bool, T> intersect_std_sphere() const noexcept;

		// intersect with standard square (center: [0, 0, 0], side length: 2, normal: [0, 1, 0])
		// (isIntersect, t, hit point(x,z))
		const std::tuple<bool, T, point<T, 2>> intersect_std_square() const noexcept;

		// intersect with standard disk (center: [0, 0, 0], radius: 1, normal: [0, 1, 0])
		// (isIntersect, t, hit point(x,z))
		const std::tuple<bool, T, point<T, 2>> intersect_std_disk() const noexcept;

	private:
		template<typename Base, typename Impl, typename ArgList>
		friend struct IInOut;

		std::ostream& impl_out(std::ostream& os) const;
		std::istream& impl_in(std::istream& is);

		template<typename Base, typename Impl, typename ArgList>
		friend struct IAffineRealSubspace;

		static const ray impl_move(const ray& r, const point<T, N>& p) noexcept;
	};

	template<size_t N>
	using rayf = ray<float, N>;
	using rayf1 = rayf<1>;
	using rayf2 = rayf<2>;
	using rayf3 = rayf<3>;

	// maybe error in editor, but no compile error
	static_assert(sizeof(rayf3) == 8 * sizeof(float));
}

#include "detail/ray.inl"
