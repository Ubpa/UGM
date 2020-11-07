#pragma once

#include "point.h"
#include "vec.h"

#include "Interfaces/IPlane.h"

namespace Ubpa {
	template<typename T>
	struct ImplTraits<plane<T>> : IListTraits<IInOut, IPlane> {
		using V = vec<T, 3>;
		using P = point<T, 3>;
		using F = T;
	};

	template<typename T>
	struct plane : SI<plane<T>> {
		using SI<plane<T>>::SI;

		plane(const point<T, 3>& p, const vec<T, 3>& n) noexcept {
			this->point = p;
			this->normal = n;
		}

		void print(std::ostream& os = std::cout) const;

	private:
		template<typename Base, typename Impl>
		friend struct IInOut;
		std::ostream& impl_out(std::ostream& os) const;
		std::istream& impl_in(std::istream& is);
	};

	using planef = plane<float>;

	// maybe error in editor, but no compile error
	static_assert(sizeof(planef) == 6 * sizeof(float));
}

#include "details/plane.inl"
