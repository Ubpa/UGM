#pragma once

#include "IEuclideanV.h"

namespace Ubpa {
	template<typename Base, typename Impl, typename ArgList>
	struct ICross : Base {
		using IList = TemplateList<IEuclideanV>;
		using Base::Base;

		static_assert(Arg_N<ArgList> == 3);
		using F = Arg_F<ArgList>;

		static const Impl cross(const Impl& x, const Impl& y) noexcept {
			return Impl{
				x[1] * y[2] - x[2] * y[1],
				x[2] * y[0] - x[0] * y[2],
				x[0] * y[1] - x[1] * y[0]
			};
		}

		const Impl cross(const Impl& y) const noexcept {
			auto& x = static_cast<const Impl&>(*this);
			return cross(x, y);
		}

		static F sin_theta(const Impl& x, const Impl& y) noexcept {
			F xN = x.norm();
			F yN = y.norm();
			F xyN = xN * yN;
			assert(xyN > 0);
			return cross(x, y).norm() / xyN;
		}

		F sin_theta(const Impl& y) const noexcept {
			auto& x = static_cast<const Impl&>(*this);
			return sin_theta(x, y);
		}
	};
}
