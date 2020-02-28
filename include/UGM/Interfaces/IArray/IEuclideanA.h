#pragma once

#include "../IAffine.h"
#include "IEuclideanV.h"

namespace Ubpa {
	// euclidean affine space
	template<typename Base, typename ImplP, typename ArgList>
	struct IEuclideanA : SIVT_CRTP<TemplateList<IAffine, IArray>, Base, ImplP, ArgList> {
		static constexpr size_t N = Arg_N<ArgList>;
		using ImplV = Arg_ImplV<ArgList>;

		static_assert(ExistInstance_v<typename ImplV::AllVBs, IEuclideanV>);
		static_assert(ImplV::N == N);

		using SIVT_CRTP<TemplateList<IAffine, IArray>, Base, ImplP, ArgList>::SIVT_CRTP;

	private:
		template<typename Base, typename ImplP, typename ArgList>
		friend struct IAffine;

		inline const ImplP impl_affine_add(const ImplV& v) const noexcept {
			auto& p = static_cast<const ImplP&>(*this);
			ImplP rst{};
			for (size_t i = 0; i < N; i++)
				rst[i] = p[i] + v[i];
			return rst;
		}

		inline ImplP& impl_affine_add_to_self(const ImplV& v) noexcept {
			auto& p = static_cast<ImplP&>(*this);
			for (size_t i = 0; i < N; i++)
				p[i] += v[i];
			return p;
		}

		inline const ImplV impl_affine_minus(const ImplP& y) const noexcept {
			auto& x = static_cast<const ImplP&>(*this);
			ImplV rst{};
			for (size_t i = 0; i < N; i++)
				rst[i] = x[i] - y[i];
			return rst;
		}
	};
}