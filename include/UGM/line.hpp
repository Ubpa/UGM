#pragma once

#include "triangle.hpp"
#include "bbox.hpp"

#include "Interfaces/ILine.hpp"

namespace Ubpa {
	template<typename T, size_t N>
	struct line;

	template<typename T, size_t N>
	struct SI_ImplTraits<line<T, N>> : IListTraits<IInOut, ILine> {
		using V = vec<T, N>;
		using P = point<T, N>;
		using F = T;
	};

	template<typename T, size_t N>
	struct line : SI<line<T, N>> {
		using SI<line<T, N>>::SI;

		line(const point<T, N>& p, const vec<T, N>& dir) noexcept { this->point = p; this->dir = dir; }

		void print(std::ostream& os = std::cout) const;

		// (isIntersect, (w, u, v), t)
		std::tuple<bool, std::array<T, 3>, T> intersect(const triangle<T, 3>& tri) const noexcept;
		// (isIntersect, t0, t1)
		std::tuple<bool, T, T> intersect(
			const bbox<T, N>& box,
			T tmin = std::numeric_limits<T>::max(),
			T tmax = std::numeric_limits<T>::min()) const noexcept;
		std::tuple<bool, T, T> intersect(
			const bbox<T, N>& box,
			const vec<T, N>& inv_dir,
			T tmin = std::numeric_limits<T>::max(),
			T tmax = std::numeric_limits<T>::min()) const noexcept;

	private:
		template<typename Base, typename Impl>
		friend struct IInOut;

		std::ostream& impl_out(std::ostream& os) const;
		std::istream& impl_in(std::istream& is);

		template<typename Base, typename Impl>
		friend struct IAffineRealSubspace;

		static line impl_move(const line& line, const point<T, N>& p) noexcept;
	};

	template<size_t N>
	using linef = line<float, N>;

	using linef1 = linef<1>;
	using linef2 = linef<2>;
	using linef3 = linef<3>;

	template<size_t N>
	using lined = line<double, N>;

	using lined1 = lined<1>;
	using lined2 = lined<2>;
	using lined3 = lined<3>;

	// maybe error in editor, but no compile error
	static_assert(sizeof(linef3) == 6 * sizeof(float));
}

#include "details/line.inl"
