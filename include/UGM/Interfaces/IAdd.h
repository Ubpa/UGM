#pragma once

#include <UTemplate/SI.h>
#include "IArray.h"

namespace Ubpa {
	template<typename Base, typename Impl, typename T, typename N>
	struct IAdd : SIVT_CRTP<TemplateList<IArray>, Base, Impl, T, N>  {
		using SIVT_CRTP<TemplateList<IArray>, Base, Impl, T, N>::SIVT_CRTP;

		const Impl operator+(const Impl& y) const noexcept {
			auto& x = static_cast<const Impl&>(*this);
			Impl rst{};
			for (size_t i = 0; i < N::value; i++)
				rst[i] = x[i] + y[i];
			return rst;
		}

		Impl& operator+=(const Impl& y) noexcept {
			auto& x = static_cast<Impl&>(*this);
			for (size_t i = 0; i < N::value; i++)
				x[i] += y[i];
			return x;
		}

		const Impl operator-() const noexcept {
			auto& x = static_cast<const Impl&>(*this);
			Impl rst{};
			for (size_t i = 0; i < N::value; i++)
				rst[i] = -x[i];
			return rst;
		}

		const Impl operator-(const Impl& y) const noexcept {
			auto& x = static_cast<const Impl&>(*this);
			Impl rst{};
			for (size_t i = 0; i < N::value; i++)
				rst[i] = x[i] - y[i];
			return rst;
		}

		Impl& operator-=(const Impl& y) noexcept {
			auto& x = static_cast<Impl&>(*this);
			for (size_t i = 0; i < N::value; i++)
				x[i] -= y[i];
			return x;
		}
	};
}
