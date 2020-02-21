#pragma once

#include "IQuat.h"
#include "../IMul.h"

namespace Ubpa {
	template<typename Base, typename Impl, typename ArgList>
	struct IQuatMul : SIVT_CRTP<TemplateList<IQuat, IMul>, Base, Impl, ArgList> {
		using T = Front_t<ArgList>;

		using SIVT_CRTP<TemplateList<IQuat, IMul>, Base, Impl, ArgList>::SIVT_CRTP;
		using SIVT_CRTP<TemplateList<IQuat, IMul>, Base, Impl, ArgList>::operator*;

		const point<T, 3> operator*(const point<T, 3>& p) const noexcept {
			auto& q = static_cast<const Impl&>(*this);
			//return (q * Impl(p) * q.inverse()).imag;
			vec<T, 3> pV(p);
			return point<T, 3>(
				(q.real * q.real - q.imag.norm2()) * pV
				+ static_cast<T>(2) * (q.imag.dot(pV) * q.imag
					+ q.real * q.imag.cross(pV)
					));
		}

	private:
		template<typename Base, typename Impl, typename ArgList>
		friend struct IMul;

		const Impl impl_mul(const Impl& y) const noexcept {
			auto& x = static_cast<const Impl&>(*this);
			Impl rst{};
			rst.real = x.real * y.real - x.imag.dot(y.imag);
			rst.imag = x.real * y.imag + y.real * x.imag + x.imag.cross(y.imag);
			return rst;
		}

		const Impl impl_inverse() const noexcept {
			auto& x = static_cast<const Impl&>(*this);
			return { x.real, -x.imag };
		}
	};
}
