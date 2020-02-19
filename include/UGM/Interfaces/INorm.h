#pragma once

#include "IInnerProduct.h"

namespace Ubpa {
	template<typename Base, typename Impl, typename T, typename N>
	struct INorm : SIVT_CRTP<TemplateList<IInnerProduct>, Base, Impl, T, N> {
		using SIVT_CRTP<TemplateList<IInnerProduct>, Base, Impl, T, N>::SIVT_CRTP;

		T Norm2() const noexcept {
			auto& x = static_cast<const Impl&>(*this);
			return x.Dot(x);
		}

		T Norm() const noexcept {
			return std::sqrt(Norm2());
		}

		const Impl Normalize() const noexcept {
			auto& x = static_cast<const Impl&>(*this);
			T norm = Norm();
			assert(norm > static_cast<T>(0));
			return x / norm;
		}

		Impl& NormalizeSelf() noexcept {
			auto& x = static_cast<Impl&>(*this);
			T norm = Norm();
			assert(norm > static_cast<T>(0));
			return x /= norm;
		}

		// radian
		static T Angle(const Impl& x, const Impl& y) noexcept {
			T xNorm = x.Norm();
			T yNorm = y.Norm();
			T xyNorm = xNorm * yNorm;
			assert(xyNorm != static_cast<T>(0));
			return Impl::Dot(x, y) / xyNorm;
		}

		T Angle(const Impl& y) const noexcept {
			auto& x = static_cast<const Impl&>(*this);
			return Angle(x, y);
		}
	};
}