#pragma once

#include "IArray.h"
#include "../IAffine.h"

namespace Ubpa {
	// euclidean affine space
	template<typename Base, typename ImplP, typename ArgList>
	struct IEuclideanA : SIVT_CRTP<TemplateList<IAffine, IArray>, Base, ImplP, ArgList> {
		using T = At_t<ArgList, 0>;
		using N = At_t<ArgList, 1>;
		using ImplV = At_t<ArgList, 2>;

		// TODO: static_assert(ImplV contain IEuclideanV)

		using SIVT_CRTP<TemplateList<IAffine, IArray>, Base, ImplP, ArgList>::SIVT_CRTP;

	private:
		template<typename Base, typename ImplP, typename ArgList>
		friend struct IAffine;

		const ImplP impl_affine_add(const ImplV& v) const noexcept {
			auto& p = static_cast<const ImplP&>(*this);
			ImplP rst{};
			for (size_t i = 0; i < N::value; i++)
				rst[i] = p[i] + v[i];
			return rst;
		}

		ImplP& impl_affine_add_to_self(const ImplV& v) noexcept {
			auto& p = static_cast<ImplP&>(*this);
			for (size_t i = 0; i < N::value; i++)
				p[i] += v[i];
			return p;
		}

		const ImplV impl_affine_minus(const ImplP& y) const noexcept {
			auto& x = static_cast<const ImplP&>(*this);
			ImplV rst{};
			for (size_t i = 0; i < N::value; i++)
				rst[i] = x[i] - y[i];
			return rst;
		}
	};
}