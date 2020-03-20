#pragma once

#include "IEuclideanAS.h"
#include "IEuclideanV.h"

#include "../IMetric.h"
#include "../IAffine.h"

namespace Ubpa {
	// euclidean affine space
	template<typename Base, typename ImplP, typename ArgList>
	struct IEuclideanA : SIVT_CRTP<TemplateList<IMetric, IAffine, IEuclideanAS, IArray>, Base, ImplP, ArgList> {
		static constexpr size_t N = Arg_N<ArgList>;
		using F = Arg_F<ArgList>;
		using ImplV = Arg_ImplV<ArgList>;

		static_assert(ExistInstance_v<typename ImplV::AllVBs, IEuclideanV>);
		static_assert(ImplV::N == N);

		using SIVT_CRTP<TemplateList<IMetric, IAffine, IEuclideanAS, IArray>, Base, ImplP, ArgList>::SIVT_CRTP;

		inline static F distance2(const ImplP& x, const ImplP& y) noexcept {
			return (x - y).norm2();
		}

		inline F distance2(const ImplP& y) const noexcept {
			auto& x = static_cast<const ImplP&>(*this);
			return distance2(x, y);
		}

	private:
		template<typename Base, typename Impl, typename ArgList>
		friend struct IEuclideanAS;

		ImplP& impl_get_point() noexcept {
			auto& x = static_cast<ImplP&>(*this);
			return x;
		}

		static const ImplP impl_move(const ImplP&, const ImplP& p) noexcept {
			return p;
		}

		template<typename Base, typename Impl, typename ArgList>
		friend struct IAffine;

		inline const ImplV impl_affine_minus(const ImplP& y) const noexcept {
			auto& x = static_cast<const ImplP&>(*this);
			ImplV rst;
			for (size_t i = 0; i < N; i++)
				rst = x[i] - y[i];
			return rst;
		}

		template<typename Base, typename Impl, typename ArgList>
		friend struct IMetric;

		inline static F impl_distance(const ImplP& x, const ImplP& y) noexcept {
			return std::sqrt(distance2(x, y));
		}
	};
}