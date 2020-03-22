#pragma once

#include "point.h"
#include "vec.h"

#include "Interfaces/IPlane.h"

namespace Ubpa {
	template<typename T>
	struct plane : SIIT_CRTP<TemplateList<IInOut, IPlane>, plane<T>, TypeList<Arg_Empty, T, vec<T, 3>, point<T, 3>>> {

		plane(const point<T, 3>& p, const vec<T, 3>& n) noexcept { this->init_IPlane(p, n); }

		void print(std::ostream& os = std::cout) const;

	private:
		template<typename Base, typename Impl, typename ArgList>
		friend struct IInOut;
		std::ostream& impl_out(std::ostream& os) const;
		std::istream& impl_in(std::istream& is);
	};

	using planef = plane<float>;
}

#include "detail/plane.inl"
