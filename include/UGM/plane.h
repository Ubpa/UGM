#pragma once

#include "point.h"
#include "vec.h"

#include "Interfaces/IPlane.h"

namespace Ubpa {
	template<typename T>
	struct plane : SI<TemplateList<IInOut, IPlane>, plane<T>, Arg_Empty, T, vec<T, 3>, point<T, 3>> {
		using SI<TemplateList<IInOut, IPlane>, plane<T>, Arg_Empty, T, vec<T, 3>, point<T, 3>>::SI;

		plane(const point<T, 3>& p, const vec<T, 3>& n) noexcept { this->init_IPlane(p, n); }

		inline void print(std::ostream& os = std::cout) const;

	private:
		template<typename Base, typename Impl, typename ArgList>
		friend struct IInOut;
		inline std::ostream& impl_out(std::ostream& os) const;
		inline std::istream& impl_in(std::istream& is);
	};

	using planef = plane<float>;

	// maybe error in editor, but no compile error
	static_assert(sizeof(planef) == 6 * sizeof(float));
}

#include "detail/plane.inl"
