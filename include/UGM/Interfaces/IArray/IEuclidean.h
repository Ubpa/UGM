#pragma once

#include "IArrayAdd.h"
#include "IArrayScalarMul.h"
#include "../ILinear.h"
#include "../IInnerProduct.h"
#include "../INorm.h"

namespace Ubpa {
	template<typename Base, typename Impl, typename T, typename N>
	struct IEuclidean : SIVT_CRTP<TemplateList<INorm, IInnerProduct, ILinear, IArrayScalarMul, IArrayAdd>, Base, Impl, T, N> {
		using SIVT_CRTP<TemplateList<INorm, IInnerProduct, ILinear, IArrayScalarMul, IArrayAdd>, Base, Impl, T, N>::SIVT_CRTP;

	private:
		template<typename Base, typename Impl, typename T, typename N>
		friend struct IInnerProduct;

		static T ImplDot(const Impl& x, const Impl& y) noexcept {
			T rst = x[0] * y[0];
			for (size_t i = 1; i < N::value; i++)
				rst += x[i] * y[i];
			return rst;
		}

	public:
		T norm2() const noexcept {
			const Impl& x = static_cast<const Impl&>(*this);
			return ImplDot(x, x);
		}

	private:
		template<typename Base, typename Impl, typename T, typename N>
		friend struct INorm;
		T ImplNorm() const noexcept {
			return std::sqrt(norm2());
		}

	public:
		// radian
		static T angle(const Impl& x, const Impl& y) noexcept {
			T xNorm = x.norm();
			T yNorm = y.norm();
			T xyNorm = xNorm * yNorm;
			assert(xyNorm != static_cast<T>(0));
			return Impl::dot(x, y) / xyNorm;
		}

		T angle(const Impl& y) const noexcept {
			auto& x = static_cast<const Impl&>(*this);
			return angle(x, y);
		}
	};
}