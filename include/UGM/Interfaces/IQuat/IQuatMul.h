#pragma once

#include "IQuat.h"
#include "../IMul.h"

namespace Ubpa {
	template<typename Base, typename Impl, typename T>
	struct IQuatMul : SIVT_CRTP<TemplateList<IQuat, IMul>, Base, Impl, T> {
		using SIVT_CRTP<TemplateList<IQuat, IMul>, Base, Impl, T>::SIVT_CRTP;
		using SIVT_CRTP<TemplateList<IQuat, IMul>, Base, Impl, T>::operator*;

		const point<T, 3> operator*(const point<T, 3>& p) const noexcept {
			auto& q = static_cast<const Impl&>(*this);

			// slow
			//return point<T,3>((q * Impl(p) * q.inverse()).imag());

			// fast
			vec<T, 3> pV(p);
			T r = q.real();
			const auto& im = q.imag();
			return point<T, 3>((r * r - im.norm2()) * pV
				+ 2 * (im.dot(pV) * im + r * im.cross(pV) ));
		}

	private:
		template<typename Base, typename Impl, typename T>
		friend struct IMul;

		const Impl impl_mul(const Impl& y) const noexcept {
			auto& x = static_cast<const Impl&>(*this);
			T real = x.real() * y.real() - x.imag().dot(y.imag());
			vec<T,3> imag = x.real() * y.imag() + y.real() * x.imag() + x.imag().cross(y.imag());
			return { imag, real };
		}


		const Impl impl_inverse() const noexcept {
			auto& x = static_cast<const Impl&>(*this);
			return { -x.imag(), x.real() };
		}
	};
}
