#pragma once

#include "IEuclideanV.h"

#include "../IMetric.h"
#include "../IAffine.h"

namespace Ubpa {
	// euclidean affine space
	template<typename Base, typename Point>
	struct IEuclideanA : Base {
		using Base::Base;

		using T = ImplTraits_T<Point>;
		static constexpr size_t N = ImplTraits_N<Point>;
		using F = ImplTraits_F<Point>;
		using Vector = ImplTraits_V<Point>;

		static_assert(Vector::template IsContain<IEuclideanV>());
		static_assert(Vector::N == N);

		inline static F distance2(const Point& x, const Point& y) noexcept {
			return (x - y).norm2();
		}

		inline F distance2(const Point& y) const noexcept {
			auto& x = static_cast<const Point&>(*this);
			return distance2(x, y);
		}

	private:
		template<typename Base, typename Impl>
		friend struct IAffineSubspace;

		inline const Point impl_affine_subspace_add(const Vector& v) const noexcept {
			auto& p = static_cast<const Point&>(*this);
#ifdef UBPA_USE_XSIMD
			if constexpr (std::is_same_v<T, float> && N == 4)
				return p.get() + v.get();
			else
#endif // UBPA_USE_XSIMD
			{
				Point rst;
				for (size_t i = 0; i < N; i++)
					rst[i] = p[i] + v[i];
				return rst;
			}
		}

		inline Point& impl_affine_subspace_add_to_self(const Vector& v) noexcept {
			auto& p = static_cast<Point&>(*this);
#ifdef UBPA_USE_XSIMD
			if constexpr (std::is_same_v<T, float> && N == 4)
				p.get() += v.get();
			else
#endif // UBPA_USE_XSIMD
			for (size_t i = 0; i < N; i++)
				p[i] += v[i];
			return p;
		}

		inline const Point impl_affine_subspace_minus(const Vector& v) const noexcept {
			auto& p = static_cast<const Point&>(*this);
#ifdef UBPA_USE_XSIMD
			if constexpr (std::is_same_v<T, float> && N == 4)
				return p.get() - v.get();
			else
#endif // UBPA_USE_XSIMD
			{
				Point rst;
				for (size_t i = 0; i < N; i++)
					rst[i] = p[i] - v[i];
				return rst;
			}
		}

		inline Point& impl_affine_subspace_minus_to_self(const Vector& v) noexcept {
			auto& p = static_cast<Point&>(*this);
#ifdef UBPA_USE_XSIMD
			if constexpr (std::is_same_v<T, float> && N == 4)
				p.get() -= v.get();
			else
#endif // UBPA_USE_XSIMD
			for (size_t i = 0; i < N; i++)
				p[i] -= v[i];
			return p;
		}

		template<typename Base, typename Impl>
		friend struct IAffine;

		inline const Vector impl_affine_minus(const Point& y) const noexcept {
			auto& x = static_cast<const Point&>(*this);
#ifdef UBPA_USE_XSIMD
			if constexpr (std::is_same_v<T, float> && N == 4)
				return x.get() - y.get();
			/* // no benefits
			else if constexpr (std::is_same_v<T, float> && N == 3) {
				Vector rst;
				auto sx = xsimd::load_unaligned(x.data());
				auto sy = xsimd::load_unaligned(y.data());
				auto srst = sx - sy;
				return { srst[0], srst[1],srst[2] };
			}*/
			else
#endif // UBPA_USE_XSIMD
			{
				Vector rst;
				for (size_t i = 0; i < N; i++)
					rst[i] = x[i] - y[i];
				return rst;
			}
		}

		template<typename Base, typename Impl>
		friend struct IMetric;

		inline static F impl_distance(const Point& x, const Point& y) noexcept {
			return std::sqrt(distance2(x, y));
		}
	};

	InterfaceTraits_Regist(IEuclideanA,
		IMetric, IAffine, IArray);
}
