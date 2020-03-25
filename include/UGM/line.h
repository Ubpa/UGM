#pragma once

#include "triangle.h"
#include "bbox.h"

#include "Interfaces/ILine.h"

namespace Ubpa {
	template<typename T, size_t N>
	struct line : SIIT_CRTP<TemplateList<IInOut, ILine>, line<T, N>, TypeList<Arg_Empty, T, vec<T, N>, point<T, N>>> {
		using Base = SIIT_CRTP<TemplateList<IInOut, ILine>, line<T, N>, TypeList<Arg_Empty, T, vec<T, N>, point<T, N>>>;
		using Base::Base;

		line(const point<T, N>& p, const vec<T, N>& dir) { this->init_ILine(p, dir); }

		void print(std::ostream& os = std::cout) const;

		// (isIntersect, (w, u, v), t)
		const std::tuple<bool, std::array<T, 3>, T> intersect(const triangle<T, 3>& tri) const noexcept;
		// (isIntersect, t0, t1)
		const std::tuple<bool, T, T> intersect(const bbox<T, N>& box,
			T tmin = std::numeric_limits<T>::max(),
			T tmax = std::numeric_limits<T>::min()) const noexcept;

	private:
		template<typename Base, typename Impl, typename ArgList>
		friend struct IInOut;

		std::ostream& impl_out(std::ostream& os) const;
		std::istream& impl_in(std::istream& is);

		template<typename Base, typename Impl, typename ArgList>
		friend struct IAffineRealSubspace;

		static const line impl_move(const line& line, const point<T, N>& p) noexcept {
			return { p, line.dir() };
		}
	};

	template<size_t N>
	using linef = line<float, N>;

	using linef1 = linef<1>;
	using linef2 = linef<2>;
	using linef3 = linef<3>;
}

#include "detail/line.inl"
