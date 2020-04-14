#pragma once

#include "../IMul.h"
#include <UTemplate/SI.h>

namespace Ubpa {
	template<typename Base, typename Impl, typename ArgList>
	struct IArrayHadamardProduct : SIVT_CRTP<TemplateList<IMul>, Base, Impl, ArgList> {
		using F = Arg_F<ArgList>;
		using T = Arg_T<ArgList>;
		static constexpr size_t N = Arg_N<ArgList>;
		using SIVT_CRTP<TemplateList<IMul>, Base, Impl, ArgList>::SIVT_CRTP;
		using SIVT_CRTP<TemplateList<IMul>, Base, Impl, ArgList>::operator*=;
		using SIVT_CRTP<TemplateList<IMul>, Base, Impl, ArgList>::operator/;
		using SIVT_CRTP<TemplateList<IMul>, Base, Impl, ArgList>::operator/=;

		Impl& operator*=(const Impl& y) noexcept {
			auto& x = static_cast<Impl&>(*this);
#ifdef USE_XSIMD
			if constexpr (std::is_same_v<T, float> && N == 4) {
				auto sx = xsimd::load_aligned(x.data());
				auto sy = xsimd::load_aligned(y.data());
				sx *= sy;
				sx.store_aligned(x.data());
			}
			else
#endif // USE_XSIMD
			for (size_t i = 0; i < N; i++)
				x[i] *= y[i];
			return x;
		}

		const Impl operator/(const Impl& y) const noexcept {
			auto& x = static_cast<const Impl&>(*this);
			Impl rst{};
#ifdef USE_XSIMD
			if constexpr (std::is_same_v<T, float> && N == 4) {
				auto sx = xsimd::load_aligned(x.data());
				auto sy = xsimd::load_aligned(y.data());
				auto srst = sx / sy;
				srst.store_aligned(rst.data());
			}
			else
#endif // USE_XSIMD
			for (size_t i = 0; i < N; i++)
				rst[i] = x[i] / y[i];
			return rst;
		}

		Impl& operator/=(const Impl& y) noexcept {
			auto& x = static_cast<Impl&>(*this);
#ifdef USE_XSIMD
			if constexpr (std::is_same_v<T, float> && N == 4) {
				auto sx = xsimd::load_aligned(x.data());
				auto sy = xsimd::load_aligned(y.data());
				sx /= sy;
				sx.store_aligned(x.data());
			}
			else
#endif // USE_XSIMD
			for (size_t i = 0; i < N; i++)
				x[i] /= y[i];
			return x;
		}

		inline const Impl inverse() const noexcept {
			auto& x = static_cast<const Impl&>(*this);

			Impl rst{};

#ifdef USE_XSIMD
			if constexpr (std::is_same_v<T, float> && N == 4) {
				auto sx = xsimd::load_aligned(x.data());
				auto srst = 1.f / sx;
				srst.store_aligned(rst.data());
			}
			else
#endif // USE_XSIMD

				for (size_t i = 0; i < N; i++)
					rst[i] = static_cast<F>(1) / x[i];

			return rst;
		}

	private:
		template<typename Base, typename Impl, typename ArgList>
		friend struct IMul;

		inline const Impl impl_mul(const Impl& y) const noexcept {
			auto& x = static_cast<const Impl&>(*this);

			Impl rst{};

#ifdef USE_XSIMD
			if constexpr (std::is_same_v<T, float> && N == 4) {
				auto sx = xsimd::load_aligned(x.data());
				auto sy = xsimd::load_aligned(y.data());
				auto srst = sx * sy;
				srst.store_aligned(rst.data());
			}
			else
#endif // USE_XSIMD

			for (size_t i = 0; i < N; i++)
				rst[i] = x[i] * y[i];

			return rst;
		}
	};
}